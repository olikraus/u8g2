/*
  MT-12864K driver test.

  This test runs the MT-12864K display driver without any hardware:
  a fake I2C byte callback records every I2C transfer (address + bytes),
  and the recorded trace is checked against the expected protocol.

  Build: make
  Run:   ./main  (prints the trace and the test result)
*/

#include "u8g2.h"
#include <stdio.h>
#include <string.h>

#define MAX_TRANSFERS 512
#define MAX_TDATA 256

struct trace_entry
{
  uint8_t adr;
  uint16_t cnt;
  uint8_t data[MAX_TDATA];
};

static struct trace_entry trace[MAX_TRANSFERS];
static int trace_cnt = 0;

static int in_transfer = 0;
static uint8_t cur_adr = 0;
static int cur_len = 0;

static void trace_close(void)
{
  if ( in_transfer && trace_cnt < MAX_TRANSFERS )
  {
    trace[trace_cnt].adr = cur_adr;
    trace[trace_cnt].cnt = (uint16_t)cur_len;
    trace_cnt++;
  }
  in_transfer = 0;
  cur_len = 0;
}

static uint8_t u8x8_byte_mt12864k_trace(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  uint8_t *data;
  int i;
  switch(msg)
  {
    case U8X8_MSG_BYTE_START_TRANSFER:
      trace_close();
      cur_adr = u8x8_GetI2CAddress(u8x8);
      in_transfer = 1;
      break;
    case U8X8_MSG_BYTE_END_TRANSFER:
      trace_close();
      break;
    case U8X8_MSG_BYTE_SEND:
      data = (uint8_t *)arg_ptr;
      for( i = 0; i < arg_int && cur_len < MAX_TDATA; i++ )
	trace[trace_cnt].data[cur_len++] = data[i];
      break;
    case U8X8_MSG_BYTE_SET_DC:
    case U8X8_MSG_BYTE_INIT:
      break;
    default:
      return 0;
  }
  return 1;
}

static unsigned long delay_ms_total = 0;

static uint8_t u8x8_gpio_mt12864k_test(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  switch(msg)
  {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:
      break;
    case U8X8_MSG_DELAY_MILLI:
      delay_ms_total += arg_int;
      break;
    default:
      break;
  }
  return 1;
}

static int check_transfer(int idx, uint8_t adr, const uint8_t *data, int cnt)
{
  int i;
  if ( idx >= trace_cnt )
  {
    printf("  FAIL: missing transfer %d (only %d recorded)\n", idx, trace_cnt);
    return 0;
  }
  if ( trace[idx].adr != adr )
  {
    printf("  FAIL: transfer %d address 0x%02x, expected 0x%02x\n", idx, trace[idx].adr, adr);
    return 0;
  }
  if ( trace[idx].cnt != cnt )
  {
    printf("  FAIL: transfer %d length %d, expected %d\n", idx, trace[idx].cnt, cnt);
    return 0;
  }
  for( i = 0; i < cnt; i++ )
    if ( trace[idx].data[i] != data[i] )
    {
      printf("  FAIL: transfer %d byte %d: 0x%02x, expected 0x%02x\n", idx, i, trace[idx].data[i], data[i]);
      return 0;
    }
  return 1;
}

static void print_trace(void)
{
  int i, j;
  printf("recorded transfers: %d\n", trace_cnt);
  for( i = 0; i < trace_cnt; i++ )
  {
    printf("  %3d: adr 0x%02x  len %3d:", i, trace[i].adr, trace[i].cnt);
    for( j = 0; j < trace[i].cnt && j < 24; j++ )
      printf(" %02x", trace[i].data[j]);
    if ( trace[i].cnt > 24 )
      printf(" ...");
    printf("\n");
  }
}

u8g2_t u8g2;

int main(void)
{
  uint8_t *buf;
  uint8_t d;
  uint8_t d2[2];
  int ok = 1;
  int i, page, t;
  int cnt_cmd, cnt_data_l, cnt_data_r, data_bytes_l, data_bytes_r;

  printf("MT-12864K driver test (fake I2C trace)\n");
  printf("======================================\n\n");

  u8g2_Setup_mt12864k_i2c_128x64_f(&u8g2, &u8g2_cb_r0, u8x8_byte_mt12864k_trace, u8x8_gpio_mt12864k_test);

  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);

  printf("after init + power on:\n");

  /* 1. reset pulse through the control register (0x3B -> 8-bit 0x076) */
  d = 0x0fe;	/* ^RST asserted (bit 0 = 0), all other bits = 1 */
  if ( !check_transfer(0, 0x076, &d, 1) ) ok = 0;
  d = 0x0ff;	/* ^RST released (bit 0 = 1) */
  if ( !check_transfer(1, 0x076, &d, 1) ) ok = 0;
  if ( delay_ms_total < 11 )
  {
    printf("  FAIL: reset timing too short (delay_ms_total=%lu)\n", delay_ms_total);
    ok = 0;
  }

  /* 2. init sequence sent to both controllers:
        extended commands 0x0d (Mode Set EXT=1), 0x02 (power save PS=0),
        0x0c (Mode Set EXT=0) followed by 0xc0 (start line) */
  d2[0] = 0x00; d2[1] = 0x0d;
  if ( !check_transfer(2, 0x078, d2, 2) ) ok = 0;	/* left controller 0x3C */
  d2[1] = 0x02;
  if ( !check_transfer(3, 0x078, d2, 2) ) ok = 0;
  d2[1] = 0x0c;
  if ( !check_transfer(4, 0x078, d2, 2) ) ok = 0;
  d2[1] = 0x0c0;
  if ( !check_transfer(5, 0x078, d2, 2) ) ok = 0;
  d2[1] = 0x0d;
  if ( !check_transfer(6, 0x07a, d2, 2) ) ok = 0;	/* right controller 0x3D */
  d2[1] = 0x02;
  if ( !check_transfer(7, 0x07a, d2, 2) ) ok = 0;
  d2[1] = 0x0c;
  if ( !check_transfer(8, 0x07a, d2, 2) ) ok = 0;
  d2[1] = 0x0c0;
  if ( !check_transfer(9, 0x07a, d2, 2) ) ok = 0;

  /* 3. display on (0x3f) on both controllers */
  d2[0] = 0x00; d2[1] = 0x03f;
  if ( !check_transfer(10, 0x078, d2, 2) ) ok = 0;
  if ( !check_transfer(11, 0x07a, d2, 2) ) ok = 0;

  /* 4. full buffer: left half 0x55, right half 0xaa, then send */
  buf = u8g2_GetBufferPtr(&u8g2);
  for( page = 0; page < 8; page++ )
  {
    memset(buf + page*128, 0x55, 64);
    memset(buf + page*128 + 64, 0xaa, 64);
  }

  printf("\nsending buffer...\n");
  u8g2_SendBuffer(&u8g2);

  /* verify the draw phase:
     for each of the 8 page rows, the driver must emit for each chip:
       set column command (0x00,0x40)
       set page command    (0x00,0xb8|page)
       and the 64 data bytes (0x40 control byte + 64 bytes) split into
       24+24+16 byte I2C transfers
     left chip data must be 0x55, right chip data must be 0xaa
  */
  cnt_cmd = 0;
  cnt_data_l = 0;
  cnt_data_r = 0;
  data_bytes_l = 0;
  data_bytes_r = 0;
  t = 12;	/* transfers 0..11 were init + power on */
  for( page = 0; page < 8; page++ )
  {
    int chip;
    for( chip = 0; chip < 2; chip++ )
    {
      uint8_t adr = (chip == 0) ? 0x078 : 0x07a;
      uint8_t col_cmd[2] = { 0x00, 0x040 };
      uint8_t page_cmd[2] = { 0x00, 0x0b8 | page };
      int bytes_sent = 0;

      if ( !check_transfer(t,   adr, col_cmd, 2) ) ok = 0;
      if ( !check_transfer(t+1, adr, page_cmd, 2) ) ok = 0;
      t += 2;

      while( bytes_sent < 64 )
      {
	int n = 64 - bytes_sent;
	if ( n > 24 ) n = 24;
	if ( trace[t].adr != adr )
	{
	  printf("  FAIL: data block %d address 0x%02x, expected 0x%02x\n", t, trace[t].adr, adr);
	  ok = 0;
	}
	if ( trace[t].cnt != n+1 || trace[t].data[0] != 0x040 )
	{
	  printf("  FAIL: data block %d header (len=%d, first=0x%02x)\n", t, trace[t].cnt, trace[t].data[0]);
	  ok = 0;
	}
	for( i = 1; i <= n; i++ )
	  if ( trace[t].data[i] != (chip == 0 ? 0x55 : 0xaa) )
	  {
	    printf("  FAIL: data block %d byte %d value 0x%02x\n", t, i, trace[t].data[i]);
	    ok = 0;
	    break;
	  }
	if ( chip == 0 )
	{
	  cnt_data_l++;
	  data_bytes_l += n;
	}
	else
	{
	  cnt_data_r++;
	  data_bytes_r += n;
	}
	bytes_sent += n;
	t++;
      }
      cnt_cmd += 2;
    }
  }

  if ( t != trace_cnt )
  {
    printf("  FAIL: unexpected transfer count (%d, expected %d)\n", trace_cnt, t);
    ok = 0;
  }
  if ( cnt_cmd != 32 || cnt_data_l != 24 || cnt_data_r != 24 )
  {
    printf("  FAIL: transfer mix cmd=%d data_l=%d data_r=%d\n", cnt_cmd, cnt_data_l, cnt_data_r);
    ok = 0;
  }
  if ( data_bytes_l != 512 || data_bytes_r != 512 )
  {
    printf("  FAIL: data bytes left=%d right=%d\n", data_bytes_l, data_bytes_r);
    ok = 0;
  }

  printf("\n");
  print_trace();

  printf("\n");
  if ( ok )
  {
    printf("ALL TESTS PASSED\n");
    return 0;
  }
  printf("TEST FAILED\n");
  return 1;
}
