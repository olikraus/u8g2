/*

  inoupdate.c
  
  U8g2 Example Updater

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>




char inoupdate_start[] = "// Please UNCOMMENT one of the contructor lines below";
char inoupdate_end[] = "// End of constructor list";


/*
  copy file from source_file_name to dest_file_name
*/
int file_copy(const char *source_file_name, const char *dest_file_name)
{
  int ch;
  FILE *source_fp;
  FILE *dest_fp;
  
  source_fp = fopen(source_file_name, "r");
  dest_fp = fopen(dest_file_name, "w");
  
  if ( source_fp == NULL || dest_fp == NULL )
    return 0;
  
  while( ( ch = fgetc(source_fp) ) != EOF )
    fputc(ch, dest_fp);
  
  fclose(source_fp);
  fclose(dest_fp);
  
  return 1;
}


/*
  Insert file "insertname" between lines "start_line" and "end_line" of file "filename"
*/
int insert_into_file(const char *filename, const char *insertname, const char *start_line, const char *end_line)
{
  int ch;
  int is_found = 0;
  static char line[1024*4];
  const char *tmpname = "tmp.h";
  FILE *source_fp;
  FILE *dest_fp;
  FILE *insert_fp;
  
  if ( file_copy(filename, tmpname) == 0 )
    return 0;

  source_fp = fopen(tmpname, "r");
  dest_fp = fopen(filename, "w");
  insert_fp = fopen(insertname, "r");

  if ( source_fp == NULL || dest_fp == NULL || insert_fp == NULL )
    return 0;
  
  for(;;)
  {
    if ( fgets(line, 1024*4, source_fp) == NULL )
      break;
    if ( strncmp(line, start_line, strlen(start_line)) == 0 )
    {
      is_found = 1;
      fputs(line, dest_fp);

      while( ( ch = fgetc(insert_fp) ) != EOF )
	fputc(ch, dest_fp);
      
      fputs("\n", dest_fp);
      
      for(;;)
      {
	if ( fgets(line, 1024*4, source_fp) == NULL )
	  break;
	if ( strncmp(line, end_line, strlen(end_line)) == 0 )
	{
	  fputs(line, dest_fp);
	  break;
	}
      }
    }
    else
    {
      fputs(line, dest_fp);
    }
  }

  fclose(insert_fp);
  fclose(source_fp);
  fclose(dest_fp);
  
  unlink(tmpname);
  
  if ( is_found )
    printf("patched %s\n", filename);

  return 1;
}



int main(int argc, char **argv)
{
  char *replacefile;
  char *inoupdate_start;
  char *inoupdate_end;
  if ( argc <= 4 )
  {
    printf("%s replacefilename \"start-text\" \"end-text\" [.ino files]\n", argv[0]);
    return 1;
  }
  argv++;
  replacefile = *argv;
  argv++;
  inoupdate_start = *argv;
  argv++;
  inoupdate_end = *argv;
  argv++;
  
  
  while( *argv != NULL )
  {
    insert_into_file(*argv, replacefile, inoupdate_start, inoupdate_end);
    argv++;
  }
  return 0;
}

