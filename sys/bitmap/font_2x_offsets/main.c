/* Verify native 2x text against an independently enlarged native 1x bitmap.
   No display hardware or graphics libraries are required. */
#include "u8g2.h"
#include <stdio.h>
#include <string.h>

#define WIDTH 320
#define HEIGHT 128
#define BUFFER_SIZE (WIDTH * HEIGHT / 8)

static u8g2_t canvas;
static uint8_t buffer[BUFFER_SIZE], reference[BUFFER_SIZE];
static unsigned cases, failures;
static const u8x8_display_info_t display_info = {
  .tile_width = WIDTH / 8, .tile_height = HEIGHT / 8,
  .pixel_width = WIDTH, .pixel_height = HEIGHT
};

static unsigned pixel(const uint8_t *frame, unsigned x, unsigned y)
{
  return (frame[(y / 8) * WIDTH + x] >> (y % 8)) & 1;
}

static void set_clip(unsigned scale, unsigned clip)
{
#ifdef U8G2_WITH_CLIP_WINDOW_SUPPORT
  u8g2_SetMaxClipWindow(&canvas);
  if (clip) u8g2_SetClipWindow(&canvas, 12*scale, 8*scale, 148*scale, 56*scale);
#else
  (void)scale;
  (void)clip;
#endif
}

static int save_bitmap(const char *prefix, const char *label, unsigned expected)
{
  char path[1024];
  unsigned x, y;
  FILE *file;
  snprintf(path, sizeof(path), "%s-%s-%s.pgm", prefix, label,
           expected ? "expected" : "actual");
  file = fopen(path, "wb");
  if (file == NULL) { perror(path); return 0; }
  fprintf(file, "P5\n%d %d\n255\n", WIDTH, HEIGHT);
  for (y = 0; y < HEIGHT; y++)
    for (x = 0; x < WIDTH; x++)
      if (fputc((expected ? pixel(reference, x / 2, y / 2) :
                          pixel(buffer, x, y)) ? 0 : 255, file) == EOF)
      { fclose(file); return 0; }
  return fclose(file) == 0;
}

/* api: 0 = DrawGlyph, 1 = DrawStr, 2 = DrawUTF8. */
static u8g2_uint_t draw(unsigned scale, unsigned api, unsigned x, unsigned y,
                       uint16_t glyph, const char *text)
{
  if (api == 0)
    return scale == 1 ? u8g2_DrawGlyph(&canvas, x, y, glyph) :
                        u8g2_DrawGlyphX2(&canvas, x, y, glyph);
  if (api == 1)
    return scale == 1 ? u8g2_DrawStr(&canvas, x, y, text) :
                        u8g2_DrawStrX2(&canvas, x, y, text);
  return scale == 1 ? u8g2_DrawUTF8(&canvas, x, y, text) :
                      u8g2_DrawUTF8X2(&canvas, x, y, text);
}

static void compare(const char *label, const uint8_t *font, unsigned api,
                    uint16_t glyph, const char *text, unsigned x, unsigned y,
                    unsigned position, unsigned opaque, unsigned clip)
{
  unsigned px, py, mismatches = 0;
  u8g2_uint_t advance;
  u8g2_SetFont(&canvas, font);
  if (position == 0) u8g2_SetFontPosBaseline(&canvas);
  else if (position == 1) u8g2_SetFontPosTop(&canvas);
  else if (position == 2) u8g2_SetFontPosBottom(&canvas);
  else u8g2_SetFontPosCenter(&canvas);
  u8g2_SetFontMode(&canvas, opaque ? 0 : 1);
  u8g2_SetDrawColor(&canvas, opaque ? 0 : 1);
  memset(buffer, opaque ? 255 : 0, sizeof(buffer));
  set_clip(1, clip);
  advance = draw(1, api, x, y, glyph, text);
  memcpy(reference, buffer, sizeof(reference));
  memset(buffer, opaque ? 255 : 0, sizeof(buffer));
  set_clip(2, clip);
  if (draw(2, api, 2*x, 2*y, glyph, text) != 2*advance)
    mismatches++;
  for (py = 0; py < HEIGHT; py++)
    for (px = 0; px < WIDTH; px++)
      if (pixel(buffer, px, py) != pixel(reference, px / 2, py / 2))
        mismatches++;
  cases++;
  if (mismatches != 0)
  {
    if (failures < 8)
      fprintf(stderr, "%s glyph=%u pos=%u opaque=%u clip=%u at=(%u,%u): %u differences\n",
              label, glyph, position, opaque, clip, x, y, mismatches);
    failures++;
  }
}

int main(int argc, char **argv)
{
  static const struct { const char *name; const uint8_t *font; } fonts[] = {
    {"helvR12", u8g2_font_helvR12_tf},
    {"helvB18", u8g2_font_helvB18_tf},
    {"timR18", u8g2_font_timR18_tf},
    {"lubI18", u8g2_font_lubI18_tf}
  };
  static const unsigned anchors[][2] = {{32, 32}, {0, 1}, {155, 63}};
  /* Explicit UTF-8 bytes keep the test independent of compiler input encoding. */
  static const char *week[] = {"\xe6\x97\xa5", "\xe4\xb8\x80", "\xe4\xba\x8c",
    "\xe4\xb8\x89", "\xe5\x9b\x9b", "\xe4\xba\x94", "\xe5\x85\xad"};
  unsigned font, glyph, position, opaque, anchor, clip, api, day;
  canvas.u8x8.display_info = &display_info;
  u8g2_SetupBuffer(&canvas, buffer, HEIGHT / 8,
                   u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);
  for (font = 0; font < sizeof(fonts) / sizeof(fonts[0]); font++)
    for (position = 0; position < 4; position++)
      for (opaque = 0; opaque < 2; opaque++)
        for (anchor = 0; anchor < 3; anchor++)
          for (clip = 0; clip < 2; clip++)
          {
#ifndef U8G2_WITH_CLIP_WINDOW_SUPPORT
            if (clip) continue;
#endif
            for (glyph = 32; glyph < 127; glyph++)
              compare(fonts[font].name, fonts[font].font, 0, glyph, NULL,
                      anchors[anchor][0], anchors[anchor][1], position, opaque, clip);
            for (api = 1; api <= 2; api++)
              compare(fonts[font].name, fonts[font].font, api, 0, "jfgpQ()_/A",
                      anchors[anchor][0], anchors[anchor][1], position, opaque, clip);
          }
  for (day = 0; day < 7; day++)
  {
    char text[80], label[16];
    snprintf(text, sizeof(text), "2026\xe5\xb9\xb4" "10\xe6\x9c\x88%u\xe6\x97\xa5 "
             "\xe6\x98\x9f\xe6\x9c\x9f%s", 18 + day, week[day]);
    snprintf(label, sizeof(label), "weekday-%u", day);
    for (position = 0; position < 4; position++)
      for (opaque = 0; opaque < 2; opaque++)
        compare(label, u8g2_font_wqy12_t_gb2312, 2, 0, text, 8, 8,
                position, opaque, 0);
    /* Leave a transparent top-aligned case in the buffers for visual evidence. */
    compare(label, u8g2_font_wqy12_t_gb2312, 2, 0, text, 8, 8, 1, 0, 0);
    if (argc == 2 && (!save_bitmap(argv[1], label, 0) || !save_bitmap(argv[1], label, 1)))
      return 2;
  }
  printf("%u cases, %u failures\n", cases, failures);
  return failures != 0;
}
