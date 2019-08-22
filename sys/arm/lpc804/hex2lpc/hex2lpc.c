/*

  hex2lpc.c
  
  Upload Intel Hex File to LPC8xx devices
  
  Features:
    - Calculates the checksum at 0x001c in the vector table
    - Part ID detection (if no hex file is specified on the commandline)

  ToDo:
    Consider https://github.com/ChristianVisintin/termiWin for Win compatibility

  Other Tools:
    - https://sourceforge.net/projects/lpc21isp/    	Last version from 2014, LPC804 not supported
    - http://www.windscooting.com/softy/mxli.html	not tested, not sure whether LPC804 is supported
    - http://www.flashmagictool.com/				GUI
    - http://git.techno-innov.fr/?p=lpctools			LPC804 not part of the definitions, but could be added

*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <time.h>

/* this defines the number of bytes, which should be sent before any further read happens */
/* it seems that there is a echo with the LPC devices, so that all data sent to the LPC */
/* is always sent back. This means, while writing, the same number of bytes are sent back */
/* so in general, we could write one byte and read one byte, but this is slow */
/* setting UART_BLOCKSIZE to 4 will write 4 bytes and then read 4 bytes, which is much faster */
#define UART_BLOCKSIZE 4

/* forward declarations */
int fmem_store_data(unsigned long adr, unsigned long cnt, unsigned char *data);
void uart_show_in_buf(void);


/*================================================*/
/* error procedure */
void err(char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);
  vprintf(fmt, va);
  printf("\n");
  va_end(va);

  printf("Last com data:\n");
  uart_show_in_buf();
}

/* user msg */
void msg(char *fmt, ...)
{
	va_list va;
	va_start(va, fmt);
	vprintf(fmt, va);
	printf("\n");
	va_end(va);
}


/*================================================*/
/* flash memory management */

/* mb = memory block */
struct _mb_struct
{
	unsigned long adr;	/* adr for "data" inside the target controller */
	unsigned long cnt;	/* number of bytes in "data" */
	unsigned char *data;	/* pointer to some data */
};
typedef struct _mb_struct mb_struct;

mb_struct *mb_open(void)
{
	mb_struct *mb = (mb_struct *)malloc(sizeof(mb_struct));
	if ( mb != NULL )
	{
		mb->adr = 0UL;
		mb->cnt = 0UL;
		mb->data = NULL;
		return mb;
	}
	return err("fmem: mem block init error"), NULL;
}

void mb_close(mb_struct *mb)
{
	if ( mb == NULL )
		return;
	mb->cnt = 0UL;
	if ( mb->data != NULL )
		free(mb->data);
	free(mb->data);
}

/* set size of the memory block, existing data will be preserved, returns 0 for error */
int mb_set_data_size(mb_struct *mb, unsigned long cnt)
{
	if ( mb->data == NULL )
	{
		mb->data = (unsigned char *)malloc(cnt);
		if ( mb->data == NULL )
			return err("fmem: mem data alloc error"), 0;
	}
	else
	{
		void *ptr;
		ptr = realloc(mb->data , cnt);
		if ( ptr == NULL )
			return err("fmem: mem data realloc error"), 0;
		mb->data = (unsigned char *)ptr;
	}
	
	mb->cnt = cnt;
	return 1;
}

mb_struct **fmem_mb_list = NULL;
unsigned long fmem_mb_list_cnt = 0UL;
unsigned long fmem_mb_list_max = 0UL;
#define FMEM_EXPAND 32

int fmem_init(void)
{
	fmem_mb_list = (mb_struct **)malloc(sizeof(mb_struct *)*FMEM_EXPAND);
	if ( fmem_mb_list == NULL )
		return err("fmem: mem block init error"), 0;
	fmem_mb_list_cnt = 0UL;
	fmem_mb_list_max = 0UL;
	return 1;
}

/* (internal function, only called by fmem_add_data) */
int fmem_expand(void)
{
	void *ptr;
	ptr = realloc(fmem_mb_list, (fmem_mb_list_max+FMEM_EXPAND)*sizeof(mb_struct *));
	if ( ptr == NULL )
		return err("fmem: mem block list expand error"), 0;
	fmem_mb_list = (mb_struct **)ptr;
	fmem_mb_list_max+= FMEM_EXPAND;
	return 1;
}

/* add a new data block to the flash memory manager */
/* (internal function, only called by fmem_store_data) */
int fmem_add_data(unsigned long adr, unsigned long cnt, unsigned char *data)
{
	mb_struct *mb = mb_open();
	if ( mb != NULL )
	{
		if ( mb_set_data_size(mb, cnt) != 0 )
		{
			memcpy(mb->data, data, cnt);
			while( fmem_mb_list_cnt >= fmem_mb_list_max )
				if ( fmem_expand() == 0 )
				{
					mb_close(mb);
					return 0;
				}
				
			fmem_mb_list[fmem_mb_list_cnt] = mb;
			fmem_mb_list_cnt++;
			return 1;
		}
	}
	return 0;
}

/* same as fmem_add_data, but it tries to expand the last data block of possible */
/* idea is to reduce the number of independent data blocks */
int fmem_store_data(unsigned long adr, unsigned long cnt, unsigned char *data)
{
	if ( fmem_mb_list_cnt > 0 )
	{
		mb_struct *mb = fmem_mb_list[fmem_mb_list_cnt-1];
		if ( mb->adr + mb->cnt == adr )
		{
			unsigned long old_cnt = mb->cnt;
			if ( mb_set_data_size(mb, mb->cnt + cnt) == 0 )
				return 0;
			memcpy(mb->data + old_cnt, data, cnt);
			return 1;
		}
	}
	return fmem_add_data(adr, cnt, data);
}

void fmem_show(void)
{
	unsigned long i;
	mb_struct *mb;
	for( i = 0; i < fmem_mb_list_cnt; i++ )
	{
		mb = fmem_mb_list[i];
		printf("%lu/%lu: adr=0x%08lx cnt=%lu\n", i+1, fmem_mb_list_cnt, mb->adr, mb->cnt);
	}
}

/*
  copy memory from fmem to a target buffer
  any data, which is NOT in the fmem will be set to 0xff
  return  1 if data has been copied to buf (intersection with the ihex data)
*/
int fmem_copy(unsigned long adr, unsigned long size, unsigned char *buf)
{
  unsigned long i, j;
  mb_struct *mb;
  int r = 0;
  
  memset(buf, 0xff, size);
  
  j = 0;
  while( j < size )
  {
    for( i = 0; i < fmem_mb_list_cnt; i++ )
    {
	    mb = fmem_mb_list[i];
	    if ( adr >= mb->adr && adr < mb->adr+mb->cnt )
	    {
	      /*
	      assert(j < size);
	      assert(adr  >= mb->adr);
	      assert(adr - mb->adr < mb->cnt );
	      */
	      buf[j] = mb->data[adr - mb->adr];
	      r = 1;
	    }
    }
    adr++;
    j++;
  }
  return r;
}

unsigned char fmem_get_byte(unsigned long adr)
{
    unsigned long i;
    mb_struct *mb;
    for( i = 0; i < fmem_mb_list_cnt; i++ )
    {
      mb = fmem_mb_list[i];
      if ( adr >= mb->adr && adr < mb->adr+mb->cnt )
      {
	//printf("adr: %08lx data: %x\n",adr,  mb->data[adr - mb->adr]);
	return mb->data[adr - mb->adr];
      }
    }
    return 0xff;
}

void fmem_set_byte(unsigned long adr, unsigned char val)
{
    unsigned long i;
    mb_struct *mb;
    for( i = 0; i < fmem_mb_list_cnt; i++ )
    {
      mb = fmem_mb_list[i];
      if ( adr >= mb->adr && adr < mb->adr+mb->cnt )
      {
	//printf("adr: %08lx data: %x\n",adr,  mb->data[adr - mb->adr]);
	mb->data[adr - mb->adr] = val;
      }
    }
}

/*================================================*/
/* intel hex parser */

#define IHEX_LINE_LEN 1024
#define IHEX_DATA_LEN 300
FILE *ihex_fp;
char ihex_line[IHEX_LINE_LEN];
int ihex_pos = 0;
int ihex_curr_line = 0;

unsigned long ihex_line_bytes;
unsigned long ihex_line_extsegadr = 0UL;	/* from typ 02 */
unsigned long ihex_line_extlinadr = 0UL;		/* from typ 04 */
unsigned long ihex_line_adr;
unsigned long ihex_line_type;
unsigned char ihex_line_data[IHEX_DATA_LEN];

/* calculate the current address */
unsigned long ihex_get_curr_adr(void)
{
	unsigned long adr;
	adr = 0UL;
	adr += ihex_line_extlinadr<<16;
	adr += ihex_line_extsegadr<<16;
	adr += ihex_line_adr;
	return adr;
}

/* read one line from the stream (global variable), return 0 for error or EOF */
int ihex_read_line(void)
{
	if ( ihex_fp == NULL )
		return err("ihex line %lu: internal error (file ptr)", ihex_curr_line), 0;
	if ( fgets(ihex_line, IHEX_LINE_LEN, ihex_fp) == NULL )
		return 0;
	ihex_line[IHEX_LINE_LEN-1] = '\0';
	ihex_pos = 0;
	ihex_curr_line++;
	return 1;
}

/* get a char from the internal line buffer and goto to next char */
unsigned int ihex_getc(void)
{
	unsigned int c;
	c = ihex_line[ihex_pos];
	if ( c != '\0' )
		ihex_pos++;
	return c;
}

unsigned int ihex_getchex(void)
{
	unsigned int c = ihex_getc();
	if ( c < '0' )
		return 0;
	if ( c <= '9' )
		return c - '0';
	if ( c < 'A' )
		return 0;
	if ( c <= 'F' )
		return c - 'A' + 10U;
	if ( c < 'a' )
		return 0;
	if ( c <= 'f' )
		return c - 'a' + 10U;
	return 0;
		
}

unsigned long ihex_gethex(int size)
{
	unsigned long v = 0;
	
	while( size > 0 )
	{
		v *= 16;
		v += ihex_getchex();
		size--;
	}
	return v;
}

/* parse line in global buffer ihex_line, return 0 on error, return 1 for eof, otherwise 2 */
int ihex_parse_line(void)
{
	unsigned long i;
	unsigned long sum = 0;
	if ( ihex_getc() != ':' )
		return 0;
	
	ihex_line_bytes = ihex_gethex(2);
	sum += ihex_line_bytes;
	ihex_line_adr = ihex_gethex(4);
	sum += ihex_line_adr & 0x0ff;
	sum += (ihex_line_adr>>8) & 0x0ff;
	ihex_line_type = ihex_gethex(2);
	sum += ihex_line_type & 0x0ff;
	switch(ihex_line_type)
	{
		case 0: 	/* data record */
			
			for( i = 0; i <= ihex_line_bytes /* includes checksum */; i++ )
			{
				ihex_line_data[i] = ihex_gethex(2);
				sum += ihex_line_data[i];
				// printf("%02lx: %02x/%02lx\n", i, ihex_line_data[i], sum);
			}
			if ( (sum & 0x0ff) != 0 ) 
				return err("ihex line %lu: checksum error", ihex_curr_line), 0;		/* checksum error */
			/* call flash memory management */
			if ( fmem_store_data( ihex_get_curr_adr(), ihex_line_bytes, ihex_line_data) == 0 )
				return err("ihex line %lu: store memory error", ihex_curr_line), 0;
			break;
		case 1:	/* end of file */
			return 1;
		case 2:	/* extended segment adr */
			ihex_line_extsegadr = ihex_gethex(4);
			break;
		case 3:	/* start extended segment adr */
			/* ignored */
			break;
		case 4: /* linear adr */
			ihex_line_extlinadr = ihex_gethex(4);
			break;
		case 5:	/* start linear adr */
			/* ignored */
			break;
		default:
			return err("ihex line %lu: unknown type %lu", ihex_curr_line, ihex_line_type), 0;	/* unknown type */
	}
	return 2;
}

int ihex_read_fp(void)
{
	if ( fmem_init() == 0 )
		return 0;
	for(;;)
	{
		if ( ihex_read_line() == 0 )
			break;
		if ( ihex_parse_line() == 0 )
			return 0;
	}
	return 1;
}

/*
  read intel hex file into the flash memory manager.
  after this, use the following functions:

    int fmem_copy(unsigned long adr, unsigned long size, unsigned char *buf)
    unsigned char fmem_get_byte(unsigned long adr)
    void fmem_set_byte(unsigned long adr, unsigned char val)

*/
int ihex_read_file(const char *filename)
{
	ihex_fp = fopen(filename, "rb");
	if ( ihex_fp == NULL )
		return err("ihex: open error with file '%s'", filename), 0;
	msg("intel hex file %s", filename);
	if ( ihex_read_fp() == 0 )
	{
		fclose(ihex_fp);
		return 0;
	}
	fclose(ihex_fp);
	return 1;
}

/*================================================*/
const char *lpc_error_string[] =
{
  "CMD_SUCCESS",
  "INVALID_COMMAND",
  "SRC_ADDR_ERROR",
  "DST_ADDR_ERROR",
  "SRC_ADDR_NOT_MAPPED",
  "DST_ADDR_NOT_MAPPED",  /* 5 */
  "COUNT_ERROR",
  "INVALID_SECTOR/INVALID_PAGE",	/* 7 */
  "SECTOR_NOT_BLANK",
  "SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION",
  "COMPARE_ERROR", /* 10 */
  "BUSY",
  "PARAM_ERROR",
  "ADDR_ERROR",
  "ADDR_NOT_MAPPED",
  "CMD_LOCKED", /* 15, 0x0F */
  "INVALID_CODE",
  "INVALID_BAUD_RATE",
  "INVALID_STOP_BIT",
  "CODE_READ_PROTECTION_ENABLED", /* 0x013 */
  "internal hex2lpc error", /* 20, 0x014 */
  "USER_CODE_CHECKSUM", /* 0x015 */
  "unknown error", /* 0x016 */
  "EFRO_NO_POWER", /* 0x017 */
  "FLASH_NO_POWER", /* 0x018 */
  "unknown error", /* 0x019 */
  "unknown error", /* 0x01A */
  "FLASH_NO_CLOCK", /* 0x01B */
  "REINVOKE_ISP_CONFIG", /* 0x01C */
  "NO_VALID_IMAGE", /* 0x01D */
  "unknown error", /* 0x01E */
  "unknown error", /* 0x01F */  
  "FLASH_ERASE_PROGRAM", /* 0x020 */
  "INVALID_PAGE",	/* 21 */
  "unknown error", /* 0x022 */
};

/*================================================*/
/* uart connection  */

int uart_fd = 0;
int uart_show_isp_cmd = 0;
struct termios uart_io;
/* in_buf should be large enough to read a complete sector with some additional overhead */
#define UART_IN_BUF_LEN (1024*48)
unsigned char uart_in_buf[UART_IN_BUF_LEN];
unsigned long uart_in_pos = 0;
unsigned long wait_time_in_clk_ticks = CLOCKS_PER_SEC/4;
unsigned long uart_in_first_0x0a_pos = 0x0ffffffff;

int uart_is_synchronized = 0;

void uart_add_in_buf(unsigned char c)
{
  if ( uart_in_pos < UART_IN_BUF_LEN )
  {
    if ( uart_in_first_0x0a_pos == 0x0ffffffff && c == 0x0a )
      uart_in_first_0x0a_pos = uart_in_pos;
    uart_in_buf[uart_in_pos++] = c;
  }
}

void uart_reset_in_buf(void)
{
  uart_in_first_0x0a_pos = 0x0ffffffff;
  uart_in_pos = 0;
}

void uart_show_buf(const unsigned char *buf, unsigned int buf_size)
{
  unsigned long i,j;
  i = 0;
  while( i < buf_size )
  {
    printf("%3ld/%02lx: ", i, i);
    for( j = 0; j < 16; j++ )
    {
      if ( i + j < buf_size )
	printf("%02x ", buf[i+j]);
      else
	printf("   ");
    }

    for( j = 0; j < 16; j++ )
    {
      if ( i + j < buf_size && buf[i+j] >= 32 && buf[i+j] <= 127 )
	printf("%c", buf[i+j]);
      else
	printf(".");
    }
    printf("\n");
    i += j;
  }
}


void uart_show_in_buf(void)
{
  uart_show_buf(uart_in_buf, uart_in_pos);

  /*
  unsigned long i,j;
  i = 0;
  while( i < uart_in_pos )
  {
    printf("%3ld/%02lx: ", i, i);
    for( j = 0; j < 16; j++ )
    {
      if ( i + j < uart_in_pos )
	printf("%02x ", uart_in_buf[i+j]);
      else
	printf("   ");
    }

    for( j = 0; j < 16; j++ )
    {
      if ( i + j < uart_in_pos && uart_in_buf[i+j] >= 32 && uart_in_buf[i+j] <= 127 )
	printf("%c", uart_in_buf[i+j]);
      else
	printf(".");
    }
    printf("\n");
    i += j;
  }
  */
}

int uart_get_result_code_after_first_0x0a(void)
{
  if ( uart_in_first_0x0a_pos == 0x0ffffffff )
    return 20;
  return atoi((const char *)uart_in_buf+uart_in_first_0x0a_pos+1);
}

/* get the LPC error/result code from the end of the buffer */
int uart_get_result_code_from_buf_end(void)
{
  int code = 0;
  unsigned long i = uart_in_pos;
  if ( i == 0 )
    return -1;
  i--;
  if ( uart_in_buf[i] == '\n' || uart_in_buf[i] == '\r' )
  {
    if ( i == 0 )
      return -1;
    i--;
  }
  if ( uart_in_buf[i] == '\n' || uart_in_buf[i] == '\r' )
  {
    if ( i == 0 )
      return -1;
    i--;
  }
  if ( uart_in_buf[i] >= '0' && uart_in_buf[i] <= '9' )
  {
    code = uart_in_buf[i] - '0';
    if ( i == 0 )
      return code;
    i--;
    if ( uart_in_buf[i] >= '0' && uart_in_buf[i] <= '9' )
    {
      code = (uart_in_buf[i] - '0')*10 + code;
      if ( i == 0 )
	return code;
      i--;
    }
    if ( uart_in_buf[i] < 32 )
      return code;
  }
  return -1;
  
}

/*
	values for baud are:
		B1200
		B1800
		B2400
		B4800
		B9600
		B19200
		B38400
		B57600
		B115200
		B230400
*/
int uart_open(const char *device, int baud)
{
	uart_is_synchronized = 0;
	uart_reset_in_buf();
	
	switch(baud)
	{
	  case B57600:
	  case B115200:
	  case B230400:
	    wait_time_in_clk_ticks = CLOCKS_PER_SEC/8;
	    break;
	}
	
  
	uart_fd=open(device, O_RDWR|O_NONBLOCK|O_NDELAY);
	if ( uart_fd < 0 )
		return perror(device), err("uart: %s error", device), 0;
	if ( tcgetattr(uart_fd, &uart_io) < 0 )
		return close(uart_fd), err("uart: tcgetattr error"), 0;
	
	uart_io.c_iflag = IGNBRK | IGNPAR;
	uart_io.c_oflag = 0;
	uart_io.c_cflag = CS8|CREAD|CLOCAL;
	uart_io.c_lflag = 0;		/* none canonical mode */
	uart_io.c_cc[VMIN] = 0;	/* make chars available immeadiatly in none canonical mode */
	uart_io.c_cc[VTIME] = 0;
	
	
	if ( cfsetispeed(&uart_io, baud) < 0 )
		return close(uart_fd), err("uart: cfsetispeed error"), 0;
	if ( cfsetospeed(&uart_io, baud) < 0 )
		return close(uart_fd), err("uart: cfsetospeed error"), 0;
	if ( cfsetspeed(&uart_io, baud) < 0 )
		return close(uart_fd), err("uart: cfsetspeed error"), 0;

	if ( tcsetattr(uart_fd, TCSANOW, &uart_io) < 0 )
		return close(uart_fd), err("uart: tcsetattr error"), 0;
	

	return 1;	
}

int uart_read_byte(void)
{
	ssize_t cnt;
	unsigned char buf[2];
	if ( uart_fd < 0 )
		return -1;
	cnt = read(uart_fd, buf, 1);
	if ( cnt == 0 )
		return -1;
	uart_add_in_buf(buf[0]);
	return buf[0];
}

void uart_read_bytes(void)
{
	ssize_t i, cnt;
	unsigned char buf[UART_BLOCKSIZE+2];
	if ( uart_fd < 0 )
		return;
	cnt = read(uart_fd, buf, UART_BLOCKSIZE);
	for( i = 0; i < cnt; i++)
	  uart_add_in_buf(buf[i]);
}

int uart_send_byte(int c)
{
	unsigned char buf[1];
	buf[0] = c;
	if ( uart_fd >= 0 )
	{
		write(uart_fd, buf, 1);
		uart_read_byte();
	}
	return 1;
}

int uart_send_bytes(int cnt, const unsigned char *buf)
{
  int i = 0;
  if ( uart_fd >= 0 )
  {
    while( i < cnt )
    {
      if ( i + UART_BLOCKSIZE <= cnt )
      {
	write(uart_fd, buf+i, UART_BLOCKSIZE);
	uart_read_bytes();
	/*
	for( j = 0; j < UART_BLOCKSIZE; j++ )
	  uart_read_byte();
	*/
	i+=UART_BLOCKSIZE;
      }
      else
      {
	write(uart_fd, buf+i, 1);
	uart_read_byte();
	i++;
      }
    }
  }
  return 1;
}

int uart_send_cnt_bytes(int cnt, unsigned char data)
{
  unsigned char buf[UART_BLOCKSIZE];
  int i = 0;
  int j;
  for( j = 0; j < UART_BLOCKSIZE; j++ )
    buf[j] = data;
    
  if ( uart_fd >= 0 )
  {
    while( i < cnt )
    {
      if ( i + UART_BLOCKSIZE <= cnt )
      {
	write(uart_fd, buf, UART_BLOCKSIZE);
	for( j = 0; j < UART_BLOCKSIZE; j++ )
	  uart_read_byte();
	i+=UART_BLOCKSIZE;
      }
      else
      {
	write(uart_fd, buf, 1);
	uart_read_byte();
	i++;
      }
    }
  }
  return 1;
}


int uart_send_str(const char *str)
{
  if ( uart_show_isp_cmd )
    printf("ISP Cmd:%s", str);
  return uart_send_bytes(strlen(str), (const unsigned char *)str);
  
/*  
	int i, len = strlen(str);
	for( i = 0; i < len; i++ )
		if ( uart_send_byte(str[i]) == 0 )
			return 0;
	return 1;
  */
}

#ifdef USE_TIME
void uart_read_more(void)
{
	time_t start;
	time_t curr;
	int c;
	unsigned long received_chars;

	time(&start);
	received_chars = 0;
	for(;;)
	{
		time(&curr);
		if ( start + 2 <= curr )
			break;
		c =uart_read_byte();
		if ( c >= 0 )
		{
			// printf("[%lu %d %c]\n", received_chars, c, c < ' ' ? ' ' : c);
			time(&start);
			received_chars++;
		}
	}
	// printf("received_chars=%lu\n", received_chars);
}
#else
void uart_read_more(void)
{
	clock_t start;
	clock_t curr;
	//clock_t wait = wait_time_in_clk_ticks;
	int c;
	unsigned long received_chars;

	start = clock();
	received_chars = 0;
	for(;;)
	{
		curr = clock();
		if ( start + wait_time_in_clk_ticks <= curr )
			break;
		c =uart_read_byte();
		if ( c >= 0 )
		{
			// printf("[%lu %d %c]\n", received_chars, c, c < ' ' ? ' ' : c);
			start = clock();			/* reset clock */
			//wait = wait_time_in_clk_ticks/4;	/* wait lesser once another byte was read */
			received_chars++;
		}
	}
	// printf("received_chars=%lu\n", received_chars);
}
#endif 

/*================================================*/
/* LPC communication  */


/*
	set the uart_is_synchronized flag
	return the flag value.
*/
#define UART_STR_SYNCHRONIZED "Synchronized"
#define UART_STR_12000 "12000"

/* return 0 if not in sync */
int uart_send_startup(void)
{
	uart_reset_in_buf();
	uart_send_str(UART_STR_SYNCHRONIZED "\r\n");
	uart_read_more();
	if ( strncmp((const char *)uart_in_buf, UART_STR_SYNCHRONIZED, strlen(UART_STR_SYNCHRONIZED)) == 0 )
	{
		/* yes, synchronized, send clock speed */
		uart_reset_in_buf();
		uart_send_str(UART_STR_12000 "\r\n");
		uart_read_more();		
		if ( strncmp((const char *)uart_in_buf, UART_STR_12000, strlen(UART_STR_12000)) == 0 )
		{
			/* yes, synchronized */
			return 1;
		}
	}
	return 0;
}


int uart_synchronize(int is_retry_quiet)
{
	if ( is_retry_quiet == 0 )
		msg("send '?'");
	uart_reset_in_buf();
	uart_send_str("?");
	uart_read_more();
	
	if ( uart_in_pos == 0 )
	{
		if ( is_retry_quiet == 0 )
			msg("no response");
		uart_is_synchronized = 0;
		
	}
	else
	{
		/* echo received, this could mean, that the device is already syncronized */
		if ( uart_in_buf[0] == '?' )
		{
			if ( is_retry_quiet == 0 )
				msg("echo received, try startup sequence");
			/* the input was echoed, looks like it is synchronized */
			/* to ensure proper operation, the startup sequence is repeated */
			
			uart_is_synchronized = uart_send_startup();
		}
		else
		{
			/* check if the controller did response with the synchronized command */
			if ( strncmp((const char *)uart_in_buf, UART_STR_SYNCHRONIZED, strlen(UART_STR_SYNCHRONIZED)) == 0 )
			{
				/* "Synchronized" recevied, send startup */
				msg("controller synchronize request");
				uart_is_synchronized = uart_send_startup();
			}
			else
			{
				if ( is_retry_quiet == 0 )
					msg("incorrect response from controller");
				uart_is_synchronized = 0;				
			}			
		}
	}
	if ( uart_is_synchronized != 0 )
		msg("controller is in sync");
	if ( is_retry_quiet == 0 )
	{
		if ( uart_is_synchronized == 0 )
			msg("controller is NOT in sync");
	}
	return uart_is_synchronized;
}

/*
  Read from controller memory. Result will be placed in uart_in_buf
  Return value contains the start index in uart_in_buf or is -1 if the read has failed
*/
long uart_read_from_adr(unsigned long adr, unsigned long cnt)
{
  char s[32];
  if ( cnt > UART_IN_BUF_LEN-64 )
    return err("wrong args for read memory"), -1;
  sprintf(s, "R %lu %lu\r\n", adr, cnt);
  uart_reset_in_buf();
  uart_send_str(s);
  uart_read_more();
  if ( uart_in_pos < 3+cnt )
    return err("read memory failed (too less data)"), -1;
  
  /* check for success code */
  if ( uart_in_buf[uart_in_pos-cnt-3] != '0'  )
    return err("read memory failed (illegal return code)"), -1;
  
  //printf("read operation, uart_in_pos = %lu, result stats at %ld\n", uart_in_pos, uart_in_pos-cnt);
  return uart_in_pos-cnt;
}

int uart_echo_off(void)
{
  int i;
  int result_code;
  
  uart_reset_in_buf();
  uart_send_str("A 0\r\n");

  for( i = 0; i < 4*5; i++)
  {
    uart_read_more();
    
    result_code = uart_get_result_code_from_buf_end();
    if ( result_code > 0 )
      return err("echo off failed (%d)", result_code), 0;
    else if ( result_code == 0 )
      return 1;
    msg("echo off in progress (%d)", i);
  }
  return err("echo off timeout"), 0;
}

int uart_echo_on(void)
{
  int i;
  int result_code;
  
  uart_reset_in_buf();
  uart_send_str("A 1\r\n");
  
  for( i = 0; i < 4*5; i++)
  {
    uart_read_more();
    
    result_code = uart_get_result_code_from_buf_end();
    if ( result_code > 0 )
      return err("echo on failed (%d)", result_code), 0;
    else if ( result_code == 0 )
      return 1;
    msg("echo on in progress (%d)", i);
  }
  return err("echo on timeout"), 0;
}

/*
  read part number, also returned as result
*/
unsigned long uart_read_part_numer(void)
{
  unsigned long id, i;
  uart_reset_in_buf();
  uart_send_str("J\r\n");
  uart_read_more();
  if ( uart_in_pos < 5 )
    return err("read part number failed (too less data)"), 0;

  uart_in_buf[uart_in_pos] = '\0';
  
  /* check for success code */
  i = 0;
  while( i < uart_in_pos )
  {
    if ( uart_in_buf[i] == '0' )
    {
      i++;
      while( uart_in_buf[i] == '\r' || uart_in_buf[i] == '\n')
      {
	i++;
      }
      id = strtoul((const char *)uart_in_buf+i, NULL, 10);
      return id;
    }
    i++;
  }
  return err("read part number failed (illegal return code)"), 0;
  
}


/*================================================*/
/* LPC type identification */

struct _lpc_struct
{
  char *name;
  unsigned long part_id;
  unsigned long flash_adr;		/* start address of the internal flash memory */
  unsigned long flash_size;		/* total size of the flash memory */
  unsigned long sector_size;		/* size of a sector flash_size/sector_size gives the number of sectors, sector_size must be power of 2 */
  unsigned long ram_buf_adr;	/* start address of the RAM buffer for writing, check manual for a suitable place */
  unsigned long ram_buf_size;	/* must be power of 2 and must be smaller than or equal to sector_size and bigger than one flash page */
};
typedef struct _lpc_struct lpc_struct;

/*
  for LPC81x and LPC82x, the stack is located at 0x1000 0270 and grows downwards.
  LPC81x: sector size=1024 bytes, page size=64 bytes

*/

lpc_struct lpc_list[] = {
/* name, 				part_id, 		flash_adr, 	flash_size, sec_size, 	ram_adr,		ram_size	 */
/* condition: (a) sec_size >= ram_size (b) ram_adr+ram_size <= highest sram address+1 */
  
{"LPC824M201JHI33", 	0x00008241,	0x00000000,	0x8000,	0x0400,	0x10000300,	0x0400 },
{"LPC822M201JHI33", 	0x00008221,	0x00000000,	0x8000,	0x0400,	0x10000300,	0x0400 },
{"LPC824M201JDH20", 	0x00008242,	0x00000000,	0x8000,	0x0400,	0x10000300,	0x0400 },
{"LPC822M201JDH20", 	0x00008222,	0x00000000,	0x4000,	0x0400,	0x10000300,	0x0400 },
  
{"LPC810M021FN8", 	0x00008100, 	0x00000000, 	0x1000,	0x0400,	0x10000300,	0x0100 },
{"LPC811M001JDH16", 	0x00008110, 	0x00000000, 	0x2000,	0x0400,	0x10000300,	0x0100 },
{"LPC812M101JDH16", 	0x00008120, 	0x00000000, 	0x4000,	0x0400,	0x10000300,	0x0100 },
{"LPC812M101JD20", 	0x00008121, 	0x00000000, 	0x4000,	0x0400,	0x10000300,	0x0100 },
{"LPC812M101JDH20", 	0x00008122, 	0x00000000, 	0x4000,	0x0400,	0x10000300,	0x0100 },
{"LPC812M101JTB16", 	0x00008122, 	0x00000000, 	0x4000,	0x0400,	0x10000300,	0x0100 },

// LP804: The stack of UART ISP commands is located at address 0x1000 03A8. The maximum stack usage is 640 bytes (0x280) and grows downwards.
// RAM end is at 0x10001000, so probably ram_size could be 0x0200 or 0x0400


{"LPC804M101JBD64", 0x00008040, 	0x00000000, 	0x8000,	0x0400,	0x10000500,	0x0400 },
{"LPC804M101JDH20", 0x00008041, 	0x00000000, 	0x8000,	0x0400,	0x10000500,	0x0400 },
{"LPC804M101JDH24", 0x00008042, 	0x00000000, 	0x8000,	0x0400,	0x10000500,	0x0400 },
{"LPC804M111JDH24", 0x00008043, 	0x00000000, 	0x8000,	0x0400,	0x10000500,	0x0400 },
{"LPC804M101JHI33", 0x00008044, 	0x00000000, 	0x8000,	0x0400,	0x10000500,	0x0400 }


};

unsigned long lpc_part_id = 0;
lpc_struct *lpc_part = NULL;

lpc_struct *lpc_find_by_part_id(unsigned long part_id)
{
  int i;
  for( i = 0; i < sizeof(lpc_list)/sizeof(*lpc_list); i++ )
  {
    if ( part_id == lpc_list[i].part_id )
      return lpc_list+i;
  }
  return NULL;
}

/*
  unlock 
*/
int lpc_unlock(void)
{
  int result_code;

  if ( lpc_part == NULL )
    return 0;

  uart_reset_in_buf();
  uart_send_str("U 23130\r\n");
  uart_read_more();
  //uart_show_in_buf();
  result_code = uart_get_result_code_from_buf_end();
  if ( result_code != 0 )
    return err("unlock failed (%d)", result_code), 0;
  return 1;

} 	   	


/*
  requires, that lpc_part has been set correctly
*/
int lpc_prepare_sectors(unsigned long start_sector, unsigned long end_sector)
{
  int i;
  char s[32];
  int result_code;

  if ( lpc_part == NULL )
    return 0;
  
  msg("flash prepare for sector %lu-%lu (0x%08lx-0x%08lx)", 
    start_sector, 
    end_sector,
    lpc_part->flash_adr+start_sector*lpc_part->sector_size,
    lpc_part->flash_adr+end_sector*lpc_part->sector_size+lpc_part->sector_size-1);
  
  sprintf(s, "P %lu %lu\r\n", start_sector, end_sector);
  uart_reset_in_buf();
  uart_send_str(s);

  for( i = 0; i < 4*5; i++)
  {
    uart_read_more();
    
    result_code = uart_get_result_code_from_buf_end();
    if ( result_code > 0 )
      return err("flash prepare for sectors failed (%d)", result_code), 0;
    else if ( result_code == 0 )
      return 1;
    msg("flash prepare for sectors in progress (%d)", i);
  }
  return err("flash prepare for sectors timeout"), 0;
  
  //uart_show_in_buf();
  
  return 1;
  
}

/*
  requires, that lpc_part has been set correctly
  Also prepares sectors
*/
int lpc_erase_all(void)
{
  int i;
  char s[32];
  unsigned long sector_cnt;
  int result_code;

  if ( lpc_part == NULL )
    return 0;

  sector_cnt = lpc_part->flash_size / lpc_part->sector_size;
  sector_cnt--;
  sprintf(s, "E 0 %lu\r\n", sector_cnt);
  
  lpc_prepare_sectors(0, sector_cnt);

  msg("flash erase");
  
  uart_reset_in_buf();
  uart_send_str(s);

  for( i = 0; i < 4*5; i++)
  {
    uart_read_more();
    
    result_code = uart_get_result_code_from_buf_end();
    if ( result_code > 0 )
      return err("flash erase failed (%d)", result_code), 0;
    else if ( result_code == 0 )
    return 0;
    {
      return 1;
    }
    msg("flash erase in progress (%d)", i);
  }
  return err("flash erase timeout"), 0;
  
}


/*
  sub-proc of the download/flash/verify procedure: download data to ram
  the global variable lpc_part must be set for this procedure
  the size of the page is available in lpc_part->ram_buf_size
  if 'size' is smaller than lpc_part->ram_buf_size, remaining bytes are filled with "0x0ff"
*/
int lpc_page_download_to_ram(unsigned long size, unsigned char *buf)
{
  int i;
  char s[32];
  int result_code;

  if ( lpc_part == NULL )
    return 0;

  msg("page download to RAM (size=%ld)", size);
  
  sprintf(s, "W %lu %lu\r\n", lpc_part->ram_buf_adr, lpc_part->ram_buf_size);
  uart_reset_in_buf();
  uart_send_str(s);
  
  /* wait for success code */
  for( i = 0; i < 4*5; i++)
  {
    uart_read_more();
    
    result_code = uart_get_result_code_from_buf_end();
    if ( result_code > 0 )
      return err("'page download to RAM' failed (%d)", result_code), 0;
    else if ( result_code == 0 )
      break;
    msg("'page download to RAM' in progress (%d)", i);
  }
      
  if ( result_code > 0 )
    return err("page download failure (%d)", result_code), 0;

  uart_reset_in_buf();

  //printf("download data:\n");
  //uart_show_buf(buf, size);

  
  if ( size < lpc_part->ram_buf_size)
  {
    uart_send_bytes(size, buf);
    uart_send_cnt_bytes(lpc_part->ram_buf_size-size, 0x0ff);
  }
  else
  {
    uart_send_bytes(lpc_part->ram_buf_size, buf);
  }
  
  uart_read_more();
  uart_read_more();
  //uart_show_in_buf();

  
  if ( memcmp(buf, uart_in_buf, size) != 0 )
    return err("page data download error (size=%ld)", size), 0;
  
  
  return 1;  
}

/*
  compare the content at adr with the content at the ram buffer
  This will always compare the complete ram buffer. This is ok, because
  during download of the data, RAM buffer is filled with 0x0ff.
  Also flash procedure will always flash the complete buffer.

  Problem: this seems to succeed always...???
*/
int lpc_page_quick_compare(unsigned long adr)
{
  char s[48];
  int result_code;
  msg("compare %lu bytes at 0x%08x", lpc_part->ram_buf_size, adr);
  sprintf(s, "M %lu %lu %lu\r\n", lpc_part->ram_buf_adr, adr, lpc_part->ram_buf_size);
  uart_reset_in_buf();
  uart_send_str(s);
  uart_read_more();
  //uart_show_in_buf();
  result_code = uart_get_result_code_after_first_0x0a();
  if ( result_code > 0 )
    return err("compare failure (%d)", result_code), 0;
  return 1;
}

/*
  compare the page data (size & buf) with the data in the controller at "adr"
  adr can be lpc_part->ram_buf_adr or the flash rom adr.
  data is loaded from the controller and compared with the data at "buf".
  only "size" bytes are compared.

  0: failed or page not equal
*/
int lpc_page_compare(unsigned long adr, unsigned long size, unsigned char *buf)
{
  char s[32];

  if ( lpc_part == NULL )
    return 0;

  msg("verify with data at 0x%08x", adr);
  
  sprintf(s, "R %lu %lu\r\n", adr, lpc_part->ram_buf_size);
  uart_reset_in_buf();
  uart_send_str(s);
  uart_read_more();
  
  if ( uart_in_pos < lpc_part->ram_buf_size )
    return err("page verify failure (too less data)"), 0;

  if ( memcmp(buf, uart_in_buf+uart_in_pos-lpc_part->ram_buf_size,size) != 0 )
    return err("page verify failed (not equal)"), 0;

  return 1;
}

/*
  flash the data from the controller ram into flash memory
  the RAM address is defined by lpc_part->ram_buf_adr
  the size of the RAM block is always lpc_part->ram_buf_size

*/
int lpc_page_flash(unsigned long dest_adr)
{
  int i;
  char s[48];
  int result_code;

  if ( lpc_part == NULL )
    return 0;

  msg("flash RAM page to adr 0x%08x", dest_adr);
  
  sprintf(s, "C %lu %lu %lu\r\n", dest_adr, lpc_part->ram_buf_adr, lpc_part->ram_buf_size);
  uart_reset_in_buf();
  uart_send_str(s);

  for( i = 0; i < 4*5; i++)
  {
    uart_read_more();
    
    result_code = uart_get_result_code_from_buf_end();
    if ( result_code > 0 )
      return err("page flash failed (%d)", result_code), 0;
    else if ( result_code == 0 )
      return 1;
    msg("page flash in progress (%d)", i);
  }
  return err("page flash timeout"), 0;
  
  
}


/*
  write a single page (a part of a sector)
  page size is defined by lpc_part->ram_buf_size
  'size' must be smaller or equal to lpc_part->ram_buf_size
  if 'size' is smaller than lpc_part->ram_buf_size, remaining bytes are filled with "0x0ff"

  precondition: erase flash before executing this procedure 
*/
int lpc_page_write_flash_verify(unsigned long size, unsigned char *buf, unsigned long dest_adr)
{
  unsigned long sector;

  
  if ( lpc_part == NULL )
    return 0;
  
  /* calculate the number of the sector, later used for the prepare command */
  sector = dest_adr / lpc_part->sector_size;

  /* prepare the sector */
  if ( lpc_prepare_sectors(sector, sector) == 0 )
    return 0;

  /* download the page (a fraction of the sector) to RAM */
  if ( lpc_page_download_to_ram(size, buf) == 0 )
    return 0;

  /* prepare the sector */
  if ( lpc_prepare_sectors(sector, sector) == 0 )
    return 0;


  /* flash the page into the prepared sector */
  if ( lpc_page_flash(dest_adr) == 0 )
    return 0;
  
  /* check the content of the controller RAM with the newly flashed area */
  if ( lpc_page_quick_compare(dest_adr) == 0 )
    return 0;
  
  /* check, whether the page has been written correctly, by reading the data back to the PC */
  /* this is not required any more */
  if ( lpc_page_compare(dest_adr, size, buf) == 0 )
    return 0;

  return 1;
}

/*
  flash an ihex file
*/
int lpc_flash_ihex(void)
{
  unsigned long page_cnt;
  unsigned char *buf;
  
  if ( lpc_part == NULL )
    return 0;
  
  if ( lpc_erase_all() == 0 )
    return 0;
  
  buf = (unsigned char *)malloc(lpc_part->ram_buf_size);
  if ( buf == NULL )
    return err("page alloc failure"), 0;
  
  page_cnt = lpc_part->flash_size / lpc_part->ram_buf_size;
  
  while( page_cnt > 0 )
  {
    page_cnt--;
    if ( fmem_copy(lpc_part->flash_adr + page_cnt * lpc_part->ram_buf_size, lpc_part->ram_buf_size, buf) != 0 )
    {
      msg("flash page %03d at address 0x%08x", page_cnt, lpc_part->flash_adr + page_cnt * lpc_part->ram_buf_size);
      if ( lpc_page_write_flash_verify(lpc_part->ram_buf_size, buf, lpc_part->flash_adr + page_cnt * lpc_part->ram_buf_size) == 0 )
      {
	return free(buf), 0;	
      }
    }
    else
    {
      //msg("flash page %03d at address 0x%08x is skipped (no data in ihex file)", page_cnt, lpc_part->flash_adr + page_cnt * lpc_part->ram_buf_size);
    }
  }
  
  msg("success");
  return 1;
}

void arm_calculate_vector_table_crc(void)
{
  int i;
  uint32_t crc;
  uint32_t vector;
  uint32_t adr;

  /* calculate the crc */
  crc = 0UL;
  adr = 0UL;
  for( i = 0; i < 7; i++ )
  {
    vector = (unsigned long)fmem_get_byte(adr++);
    vector |= ((unsigned long)fmem_get_byte(adr++)) << 8;
    vector |= ((unsigned long)fmem_get_byte(adr++)) << 16;
    vector |= ((unsigned long)fmem_get_byte(adr++)) << 24;
    crc += vector;
  }
  crc = 0-crc;
  
  /* put found crc in vector */
  vector = (unsigned long)fmem_get_byte(adr++);
  vector |= ((unsigned long)fmem_get_byte(adr++)) << 8;
  vector |= ((unsigned long)fmem_get_byte(adr++)) << 16;
  vector |= ((unsigned long)fmem_get_byte(adr++)) << 24;
  
  msg("LPC crc calculated = 0x%08lx, crc found = 0x%08lx", crc, vector);
      
  /* overwrite crc in the ihex file */
  adr = 7*4;
  fmem_set_byte(adr++, (crc>>0) & 255);
  fmem_set_byte(adr++, (crc>>8) & 255);
  fmem_set_byte(adr++, (crc>>16) & 255);
  fmem_set_byte(adr++, (crc>>24) & 255);
}

unsigned long arm_get_reset_vector(void)
{
  unsigned long vector;
  unsigned long adr = 0x04;
  
  vector = (unsigned long)fmem_get_byte(adr++);
  vector |= ((unsigned long)fmem_get_byte(adr++)) << 8;
  vector |= ((unsigned long)fmem_get_byte(adr++)) << 16;
  vector |= ((unsigned long)fmem_get_byte(adr++)) << 24;
  
  vector &= 0x0fffffffe;
  
  return vector;  
}

/*
  execute reset handler
*/
int lpc_exec_reset(void)
{
  int result_code;
  unsigned long reset_vector;
  char s[32];

  if ( lpc_part == NULL )
    return 0;

  reset_vector = arm_get_reset_vector();
  msg("execute reset handler at 0x%08x", reset_vector);
  sprintf(s, "G %lu T\r\n", reset_vector);
  uart_reset_in_buf();
  uart_send_str(s);
  uart_read_more();
  //uart_show_in_buf();
  result_code = uart_get_result_code_from_buf_end();
  if ( result_code > 0 )
    return err("exec reset handler failed (%d)", result_code), 0;
  return 1;

} 	   	


int lpc_load_and_flash_ihex(const char *ihex_name)
{
  if ( ihex_name == NULL )
  {
    return err("no ihex file"), 0;
  }

  if ( ihex_name[0] == '\0' )
  {
    return err("no ihex file"), 0;
  }
  
  
  if ( ihex_read_file(ihex_name) == 0 )
  {
    return err("ihex file read error"), 0;
  }
  
  arm_calculate_vector_table_crc();
  
  lpc_unlock();
	
  return lpc_flash_ihex();
}


/*================================================*/
/* commandline parser and main procedure */

int get_str_arg(char ***argv, int c, char **result)
{
  if ( (**argv)[0] == '-' )
  {
    if ( (**argv)[1] == c )
    {
      if ( (**argv)[2] == '\0' )
      {
	(*argv)++;
	*result = **argv;
      }
      else
      {
	*result = (**argv)+2;
      }
      (*argv)++;
      return 1;      
    }
  }
  return 0;
}


int get_num_arg(char ***argv, int c, unsigned long *result)
{
  if ( (**argv)[0] == '-' )
  {
    if ( (**argv)[1] == c )
    {
      if ( (**argv)[2] == '\0' )
      {
	(*argv)++;
	*result = strtoul((**argv), NULL, 10);
      }
      else
      {
	*result = strtoul((**argv)+2, NULL, 10);
      }
      (*argv)++;
      return 1;      
    }
  }
  return 0;
}

int is_arg(char ***argv, int c)
{
  if ( (**argv)[0] == '-' )
  {
    if ( (**argv)[1] == c )
    {
      (*argv)++;
      return 1;      
    }
  }
  return 0;
}

/*================================================*/
void help(void)
{
  printf("-h        Display this help\n");
  printf("-f <file> Load data from intel hex <file>\n");
  printf("-x        Execute ARM reset handler after upload\n");
  printf("          Note: Reset handler must set the stack pointer and restore SYSMEMREMAP\n");
  printf("-p <port> Use UART at <port> (default: '/dev/ttyUSB0')\n");
  printf("-s <n>    Set UART transfer speed, 0=9600 (default), 1=19200, 2=57600, 3=115200\n");
  printf("-i        Show ISP commands sent to the device\n");
}

/*================================================*/
/* main */

int main(int argc, char **argv)
{
  unsigned long speed = 0;
  int baud = B9600;
  int is_execute = 0;
  
  char *file = "";
  char *port = "/dev/ttyUSB0";
  
  argv++;
  /*
  if ( *argv == NULL )
  {
    help();
    exit(1);
  }
  */
  for(;;)
  {
    if ( *argv == NULL )
      break;
    if ( is_arg(&argv, 'h') != 0 )
    {
      help();
      exit(1);
    }
    else if ( is_arg(&argv, 'x') != 0 )
    {
      is_execute = 1;
    }
    else if ( is_arg(&argv, 'i') != 0 )
    {
      uart_show_isp_cmd = 1;
    }
    else if ( get_num_arg(&argv, 's', &speed) != 0 )
    {
    }
    else if ( get_str_arg(&argv, 'f', &file) != 0 )
    {      
    }
    else if ( get_str_arg(&argv, 'p', &port) != 0 )
    {      
    }
    else
    {
      help();
      exit(1);
    }
  }
  
  
  switch(speed)
  {
    default:
    case 0: baud = B9600; break;
    case 1: baud = B19200; break;
    case 2: baud = B57600; break;
    case 3: baud = B115200; break;
  }
  
  //fmem_show();
  if ( uart_open(port, baud) == 0 )
  {
	  for(;;)
	  {
		  msg("please connect controller to USB port, retry...");
		  sleep(2);
		  if ( uart_open(port, baud) != 0 )
			  break;
	  }
  }
  
  if ( uart_synchronize(0) == 0 )
  {
	  for(;;)
	  {
		  msg("please put controller into UART ISP mode, retry...");
		  if ( uart_synchronize(1) != 0 )
			  break;
	  }
  }

  /* echo on should be default, but it is enforced here... */
  //if ( uart_echo_on() == 0 )
  //  return 0;

  /* read part number */
  lpc_part_id = uart_read_part_numer();
  msg("received part id 0x%08x", lpc_part_id);
  
  /*  check if the part number is known */
  lpc_part = lpc_find_by_part_id(lpc_part_id);
  if ( lpc_part == NULL )
  {
    err("unknown controller");
    return 0;
  }
  
  msg("controller %s with %lu bytes flash memory", lpc_part->name, lpc_part->flash_size);
  
  
  if ( lpc_load_and_flash_ihex(file) == 0 )	/* unlock, erase and flash procedure */
    return 0;

  if ( is_execute != 0 )
    lpc_exec_reset();
  
  return 0;
}




