/*

  intersection.c
  
  coverage: gcc -fprofile-arcs -ftest-coverage intersection.c

  new intersection with asymetric boundaries
  warning: the case v0==v1 is not checked.
  

*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

uint8_t array[256];

void clear(void)
{
  int i;
  for( i = 0; i < 256; i++ )
    array[i] = 0;
}

void line(uint8_t from, uint8_t to, uint8_t value)
{
  uint8_t i = from;
  for(;;)
  {
    if ( i == to )
      break;
    array[i] |= value;
    i++;
  }
}

void show(void)
{
  int i = 0;
  for(;;)
  {
    if ( i == 256 )
      break;
    if ( array[i] == 0 ) printf(".");  
    if ( array[i] == 1 ) printf("=");  
    if ( array[i] == 2 ) printf("|");  
    if ( array[i] == 3 ) printf("#");  
    i++;
    if ( (i % 64) == 0 ) printf("\n");
    
  }
  printf("\n");
}

uint8_t a1 = 20;
uint8_t a2 = 40;

int is_array_intersection(void)
{
  int i;
  for( i = 0; i < 256; i++ )
    if ( array[i] >= 3 )
      return 1;
  return 0;
}

#define PLA_SIZE 512
int pla[PLA_SIZE];

void conditioninit(void)
{
  int i;
  for( i = 0; i < PLA_SIZE; i++ )
    pla[i] = 0;
}

void conditionshow(void)
{
  int i, j;
  for( i = 0; i < PLA_SIZE; i++ )
  {
    for( j = 8; j >= 0; j-- )
    {
      printf("%d", (i>>j)&1);
    }
    /*
    if ( pla[i] == 0 )
      printf(" -\n");
    else
      printf(" %d\n", pla[i]);
    */
    
    if ( pla[i] == 0 )
      printf(" 2\n");
    else
      printf(" %d\n", pla[i]-1);
    
  }
}

int conditionpattern(uint8_t b1, uint8_t b2)
{
  int c, p;
  
  p = 0;
  
  c = b1 < a1 ? 1 : 0;
  p <<= 1;
  p |= c;

  c = b1 <= a1 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  c = b1 < a2 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  c = b1 <= a2 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  c = b2 > a1 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  c = b2 >= a1 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  c = b2 > a2 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  c = b2 >= a2 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  c = b1 <= b2 ? 1 : 0;
  p <<= 1;
  p |= c;
  
  return p;
}

int is_math_intersection_original(uint8_t b1, uint8_t b2)
{
  uint8_t c1, c2, c3;
  c1 = b1 <= a2;
  c2 = b2 >= a1;
  c3 = b1 > b2;
  
  if ( c1 && c2 )
    return 1;
  if ( c1 && c3 )
    return 1;
  if ( c2 && c3 )
    return 1;
  
  return 0;
  
}

int is_math_intersection(uint8_t b1, uint8_t b2)
{
  uint8_t c1, c2, c3, tmp;
  c1 = b1 < a2;	//  c1 = b1 <= a2;
  c2 = b2 > a1;	// c2 = b2 >= a1;
  c3 = b1 > b2;	// c3 = b1 > b2;
  
  tmp = c1;
  c1 &= c2;
  c2 &= c3;
  c3 &= tmp;
  c1 |= c2;
  c1 |= c3;
  return c1;

/*  
  if ( c1 && c2 )
    return 1;
  if ( c1 && c3 )
    return 1;
  if ( c2 && c3 )
    return 1;
  
  return 0;  
  */
}

uint8_t u8g_is_intersection_math(uint8_t a0, uint8_t a1, uint8_t v0, uint8_t v1)
{
  uint8_t c1, c2, c3, tmp;
  c1 = v0 < a1;	// c1 = v0 <= a1;
  c2 = v1 > a0;	// c2 = v1 >= a0;
  c3 = v0 > v1;	// c3 = v0 > v1;
  
  tmp = c1;
  c1 &= c2;
  c2 &= c3;
  c3 &= tmp;
  c1 |= c2;
  c1 |= c3;
  return c1 & 1;
}

/*
  version with asymetric boundaries.
  a1 and v1 are excluded
  v0 == v1 is not support end return 1
*/
uint8_t u8g_is_intersection_decision_tree(uint8_t a0, uint8_t a1, uint8_t v0, uint8_t v1)
{
  if ( v0 < a1 )		// v0 <= a1
  {
    if ( v1 > a0 )	// v1 >= a0
    {
      return 1;
    }
    else
    {
      if ( v0 > v1 )	// v0 > v1
      {
	return 1;
      }
      else
      {
	return 0;
      }
    }
  }
  else
  {
    if ( v1 > a0 )	// v1 >= a0
    {
      if ( v0 > v1 )	// v0 > v1
      {
	return 1;
      }
      else
      {
	return 0;
      }
    }
    else
    {
      return 0;
    }
  }
}



void check(uint8_t b1, uint8_t b2)
{
  int intersection, p;
  clear();
  line(a1, a2, 1);
  line(b1, b2, 2);
  //show();
  intersection = is_array_intersection();
  p = conditionpattern(b1, b2); 
  pla[p] |= intersection+1;
  /*
  if ( p == 0 || p == 7)
    printf("%02x: [%d %d] [%d %d] array_intersection:%d\n", p, a1, a2, b1, b2, intersection);
  */
  /*
  if ( is_math_intersection(b1, b2) != intersection)
  {
    printf("%02x: [%d %d] [%d %d] is_math_intersection:%d != %d failed\n", p, a1, a2, b1, b2, intersection, is_math_intersection(b1, b2));
    exit(0);
  }
  */
  if ( u8g_is_intersection_decision_tree(a1,a2,b1,b2)  != intersection )
  {
    printf("%02x: [%d %d] [%d %d] u8g_is_intersection_decision_tree:%d != %d failed\n", p, a1, a2, b1, b2, intersection, u8g_is_intersection_decision_tree(a1,a2,b1,b2));
    exit(0);
  }
  if ( u8g_is_intersection_math(a1,a2,b1,b2)  != intersection )
  {
    printf("%02x: [%d %d] [%d %d] u8g_is_intersection_math:%d != %d failed\n", p, a1, a2, b1, b2, intersection, u8g_is_intersection_math(a1,a2,b1,b2));
    exit(0);
  }
  
}

#define STEP 1
void check_size(uint8_t size)
{
  int i;
  uint8_t b1;
  uint8_t b2;
  b1 = 0;
  for( i = 0; i <= 256; i+=STEP )
  {
    b2 = b1;
    b2 += size;
    check(b1, b2);
    b1 += STEP;
  }
}

void check_all(void)
{
  uint8_t size;
  for( size =1; size < 255; size++ )
  {
    printf("size=%d\n", size);
    check_size(size);
  }
}

void main(void)
{
  /*
  assert( u8g_is_intersection_decision_tree(4, 6, 7, 9) == 0 );
  assert( u8g_is_intersection_decision_tree(4, 6, 6, 9) != 0 );
  assert( u8g_is_intersection_decision_tree(6, 9, 4, 6) != 0 );
  assert( u8g_is_intersection_decision_tree(7, 9, 4, 6) == 0 );
  */
  conditioninit();
  check_all();
  /*
  check_size(7);
  check_size(17);
  check_size(20);
  check_size(21);
  check_size(37);
  check_size(78);
  */
  //conditionshow();
}


