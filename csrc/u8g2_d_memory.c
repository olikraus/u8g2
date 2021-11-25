/* u8g2_d_memory.c */
/* generated code, codebuild, u8g2 project */

#include "u8g2.h"

#ifdef U8G2_USE_ONE_STATIC_BUFF
static uint8_t buf[1024];
#endif // U8G2_USE_ONE_STATIC_BUFF

uint8_t *u8g2_m_16_4_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_4_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_4_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 4;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[512];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 4;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1024];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_255_2_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2040];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_255_2_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[4080];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_255_2_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[4080];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_9_5_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[72];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_9_5_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[144];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_9_5_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 5;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[360];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 5;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_4_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[64];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_4_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_4_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 4;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 4;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_16_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[64];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_16_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_16_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 16;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1024];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 16;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_12_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[96];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_12_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_12_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 12;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1152];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 12;
  return buf;
  #endif
}
uint8_t *u8g2_m_10_16_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[80];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_10_16_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_10_16_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 16;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1280];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 16;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_16_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_16_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_16_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 16;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2048];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 16;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_20_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_20_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_20_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 20;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[3200];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 20;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[512];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2048];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_13_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[104];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_13_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[208];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_13_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[832];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_6_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[64];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_6_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_6_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 6;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[384];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 6;
  return buf;
  #endif
}
uint8_t *u8g2_m_6_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[48];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_6_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[96];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_6_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[384];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_2_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[96];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_2_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_2_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_12_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_12_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_16_12_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 12;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1536];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 12;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_4_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_4_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[512];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_4_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 4;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1024];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 4;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[96];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[768];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_4_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_4_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[384];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_4_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 4;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[768];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 4;
  return buf;
  #endif
}
uint8_t *u8g2_m_50_30_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[400];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_50_30_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[800];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_50_30_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 30;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[12000];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 30;
  return buf;
  #endif
}
uint8_t *u8g2_m_18_21_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[144];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_18_21_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[288];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_18_21_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 21;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[3024];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 21;
  return buf;
  #endif
}
uint8_t *u8g2_m_11_6_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[88];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_11_6_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[176];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_11_6_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 6;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[528];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 6;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_9_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[96];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_9_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_12_9_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 9;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[864];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 9;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[384];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1536];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[240];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[480];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1920];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_15_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[240];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_15_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[480];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_15_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 15;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[3600];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 15;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_16_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[240];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_16_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[480];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_16_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 16;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[3840];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 16;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_16_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_16_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_16_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 16;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2560];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 16;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_12_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[192];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_12_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[384];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_24_12_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 12;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2304];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 12;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_13_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_13_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_13_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 13;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2080];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 13;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_20_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[240];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_20_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[480];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_30_20_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 20;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[4800];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 20;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_16_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_16_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[512];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_16_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 16;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[4096];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 16;
  return buf;
  #endif
}
uint8_t *u8g2_m_40_30_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_40_30_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[640];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_40_30_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 30;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[9600];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 30;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1280];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_17_4_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[136];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_17_4_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[272];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_17_4_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 4;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[544];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 4;
  return buf;
  #endif
}
uint8_t *u8g2_m_17_8_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[136];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_17_8_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[272];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_17_8_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 8;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1088];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 8;
  return buf;
  #endif
}
uint8_t *u8g2_m_48_17_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[384];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_48_17_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[768];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_48_17_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 17;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[6528];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 17;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_12_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_12_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_12_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 12;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1920];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 12;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_20_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[256];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_20_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[512];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_32_20_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 20;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[5120];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 20;
  return buf;
  #endif
}
uint8_t *u8g2_m_22_13_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[176];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_22_13_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[352];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_22_13_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 13;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2288];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 13;
  return buf;
  #endif
}
uint8_t *u8g2_m_19_4_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[152];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_19_4_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[304];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_19_4_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 4;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[608];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 4;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_10_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_10_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_10_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 10;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1600];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 10;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_4_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_4_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_4_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 4;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[640];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 4;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_17_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[160];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_17_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[320];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_20_17_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 17;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[2720];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 17;
  return buf;
  #endif
}
uint8_t *u8g2_m_22_9_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[176];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_22_9_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[352];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_22_9_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 9;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[1584];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 9;
  return buf;
  #endif
}
uint8_t *u8g2_m_25_25_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[200];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_25_25_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[400];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_25_25_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 25;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[5000];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 25;
  return buf;
  #endif
}
uint8_t *u8g2_m_37_16_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[296];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_37_16_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[592];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_37_16_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 16;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[4736];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 16;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_1_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[64];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_1_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[128];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_8_1_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[64];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_4_1_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[32];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_4_1_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[64];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_4_1_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[32];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_1_1_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[8];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_1_1_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[16];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_1_1_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[8];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_48_30_1(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 1;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[384];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 1;
  return buf;
  #endif
}
uint8_t *u8g2_m_48_30_2(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 2;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[768];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 2;
  return buf;
  #endif
}
uint8_t *u8g2_m_48_30_f(uint8_t *page_cnt)
{
  #ifdef U8G2_USE_DYNAMIC_ALLOC
  *page_cnt = 30;
  return 0;
  #else
  #ifndef U8G2_USE_ONE_STATIC_BUFF
  static uint8_t buf[11520];
  #endif // !U8G2_USE_ONE_STATIC_BUFF
  *page_cnt = 30;
  return buf;
  #endif
}
/* end of generated code */
