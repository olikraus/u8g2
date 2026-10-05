/* 2026-10-04：按唯一边界像素验证普通矩形的颜色、旋转、裁剪及分页缓冲行为。 */
#include "u8g2.h"
#include <stdio.h>
#include <string.h>

#define WIDTH 64
#define HEIGHT 32
#define GUARD 16
#define CAPACITY (WIDTH * HEIGHT / 8)

static const u8x8_display_info_t display_info = {
  .tile_width = WIDTH / 8,
  .tile_height = HEIGHT / 8,
  .pixel_width = WIDTH,
  .pixel_height = HEIGHT
};

/* 2026-10-04：仅初始化真实绘图缓冲，不初始化或访问显示硬件。 */
static void setup(u8g2_t *g, uint8_t *buffer, uint8_t pages,
                  u8g2_draw_ll_hvline_cb layout, const u8g2_cb_t *rotation)
{
  memset(g, 0, sizeof(*g));
  u8g2_GetU8x8(g)->display_info = &display_info;
  u8g2_SetupBuffer(g, buffer, pages, layout, rotation);
}

/* 2026-10-04：一个像素宽、三个像素高的 XOR 矩形应产生三个连续像素。 */
static int reproduce(void)
{
  u8g2_t g;
  uint8_t buffer[CAPACITY] = { 0 };
  unsigned a, b, c;
  setup(&g, buffer, HEIGHT / 8, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);
  u8g2_SetDrawColor(&g, 2);
  u8g2_DrawFrame(&g, 3, 2, 1, 3);
  a = (buffer[3] >> 2) & 1;
  b = (buffer[3] >> 3) & 1;
  c = (buffer[3] >> 4) & 1;
  printf("frame 1x3 XOR: expected 111, actual %u%u%u\n", a, b, c);
  return a != 1 || b != 1 || c != 1;
}

/* 2026-10-04：直接按几何边界枚举一次像素，避免复制生产函数的四条边分解。 */
static int check_case(uint8_t pages, uint8_t row,
                      u8g2_draw_ll_hvline_cb layout, const u8g2_cb_t *rotation,
                      unsigned color, unsigned clip,
                      u8g2_uint_t x, u8g2_uint_t y,
                      u8g2_uint_t w, u8g2_uint_t h)
{
  u8g2_t actual, expected;
  uint8_t a[CAPACITY + 2 * GUARD], e[CAPACITY + 2 * GUARD];
  uint8_t initial[CAPACITY + 2 * GUARD];
  unsigned i, dx, dy;
  for (i = 0; i < sizeof(a); i++)
    a[i] = (uint8_t)(0xa5 ^ (i * 17));
  memcpy(e, a, sizeof(a));
  memcpy(initial, a, sizeof(a));
  setup(&actual, a + GUARD, pages, layout, rotation);
  setup(&expected, e + GUARD, pages, layout, rotation);
  u8g2_SetBufferCurrTileRow(&actual, row);
  u8g2_SetBufferCurrTileRow(&expected, row);
  if (clip)
  {
    u8g2_SetClipWindow(&actual, 2, 3, actual.width - 2, actual.height - 3);
    u8g2_SetClipWindow(&expected, 2, 3, expected.width - 2, expected.height - 3);
  }
  u8g2_SetDrawColor(&actual, (uint8_t)color);
  u8g2_SetDrawColor(&expected, (uint8_t)color);
  u8g2_DrawFrame(&actual, x, y, w, h);
  for (dy = 0; dy < h; dy++)
    for (dx = 0; dx < w; dx++)
      if (dx == 0 || dy == 0 || dx + 1 == w || dy + 1 == h)
        u8g2_DrawPixel(&expected, (u8g2_uint_t)(x + dx), (u8g2_uint_t)(y + dy));
  if (memcmp(a, e, sizeof(a)) != 0)
  {
    printf("FAIL pages=%u row=%u color=%u clip=%u x=%u y=%u w=%u h=%u\n",
           pages, row, color, clip, x, y, w, h);
    return 1;
  }
  if (memcmp(a, initial, GUARD) != 0 ||
      memcmp(a + GUARD + WIDTH * pages, initial + GUARD + WIDTH * pages,
             sizeof(a) - GUARD - WIDTH * pages) != 0)
  {
    printf("FAIL buffer guard: pages=%u row=%u color=%u clip=%u x=%u y=%u w=%u h=%u\n",
           pages, row, color, clip, x, y, w, h);
    return 1;
  }
  if (color == 2)
  {
    u8g2_DrawFrame(&actual, x, y, w, h);
    if (memcmp(a, initial, sizeof(a)) != 0)
    {
      puts("FAIL repeated XOR did not restore buffer");
      return 1;
    }
  }
  return 0;
}

/* 2026-10-04：覆盖正尺寸退化边界、正常矩形、越界位置及两种缓冲布局；零尺寸不在既有契约内。 */
int main(int argc, char **argv)
{
  static const u8g2_cb_t *rotations[] = {
    U8G2_R0, U8G2_R1, U8G2_R2, U8G2_R3, U8G2_MIRROR, U8G2_MIRROR_VERTICAL
  };
  static const u8g2_draw_ll_hvline_cb layouts[] = {
    u8g2_ll_hvline_vertical_top_lsb, u8g2_ll_hvline_horizontal_right_lsb
  };
  static const uint8_t page_sizes[] = { 1, 2, 4 };
  static const u8g2_uint_t widths[] = { 1, 2, 3, 7 };
  static const u8g2_uint_t heights[] = { 1, 2, 3, 9 };
  unsigned l, r, p, row, color, clip, w, h, pos, cases = 0, skipped = 0;
  if (argc == 2 && strcmp(argv[1], "--mirror-page") == 0)
    return check_case(1, 0, u8g2_ll_hvline_vertical_top_lsb,
                      U8G2_MIRROR_VERTICAL, 0, 0, 3, 5, 1, 1);
  if (reproduce())
    return 1;
  if (argc == 2 && strcmp(argv[1], "--repro") == 0)
    return 0;
  for (l = 0; l < sizeof(layouts) / sizeof(layouts[0]); l++)
    for (r = 0; r < sizeof(rotations) / sizeof(rotations[0]); r++)
      for (p = 0; p < sizeof(page_sizes) / sizeof(page_sizes[0]); p++)
        for (row = 0; row < HEIGHT / 8; row += page_sizes[p])
          for (color = 0; color < 3; color++)
            for (clip = 0; clip < 2; clip++)
              for (w = 0; w < sizeof(widths) / sizeof(widths[0]); w++)
                for (h = 0; h < sizeof(heights) / sizeof(heights[0]); h++)
                  for (pos = 0; pos < 6; pos++)
                  {
                    u8g2_uint_t logical_width = r == 1 || r == 3 ? HEIGHT : WIDTH;
                    u8g2_uint_t logical_height = r == 1 || r == 3 ? WIDTH : HEIGHT;
                    u8g2_uint_t x = pos == 0 ? 0 : pos == 1 ? 3 :
                      pos == 2 ? logical_width - 1 : pos == 3 ? logical_width :
                      pos == 4 ? logical_width + 2 : (u8g2_uint_t)-2;
                    u8g2_uint_t y = pos == 0 ? 0 : pos == 1 ? 5 :
                      pos == 2 ? logical_height - 1 : pos == 3 ? logical_height :
                      pos == 4 ? logical_height + 2 : (u8g2_uint_t)-2;
                    /* 2026-10-04：垂直镜像分页的原版保护区失败另测记录，不纳入矩形修复范围。 */
                    if (rotations[r] == U8G2_MIRROR_VERTICAL && page_sizes[p] < HEIGHT / 8)
                    {
                      skipped++;
                      continue;
                    }
                    if (check_case(page_sizes[p], (uint8_t)row, layouts[l], rotations[r],
                                   color, clip, x, y, widths[w], heights[h]))
                    {
                      printf("layout=%u rotation=%u\n", l, r);
                      return 1;
                    }
                    cases++;
                  }
  printf("PASS %u frame cases, buffer guards and repeated XOR\n", cases);
  printf("SKIP %u vertical-mirror paged cases: separate baseline guard failure\n", skipped);
  return 0;
}
