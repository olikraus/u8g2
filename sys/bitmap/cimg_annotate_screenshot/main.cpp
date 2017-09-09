#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <assert.h>
#include <u8g2.h>
#define cimg_display 0 // Prevent CImg from including Xlib and other unneeded stuff
#include <CImg.h>

using namespace cimg_library;

const char * OUT_DIR = "output";

u8g2_t u8g2;

// Convert the buffer of an u8g2 bitmap device to a CImg bitmap
template<typename T>
CImg<T> u8x8_bitmap_to_cimg(u8g2_t *u8g2, unsigned int size_c, T *fg_color, T *bg_color) {
  uint16_t width = u8g2_GetDisplayWidth(u8g2);
  uint16_t height = u8g2_GetDisplayHeight(u8g2);
  u8x8_t *u8x8 = u8g2_GetU8x8(u8g2);

  CImg<T> img(width, height, /* size_z */ 1, size_c);
  for (uint16_t y = 0; y < height; ++y) {
    for (uint16_t x = 0; x < width; ++x) {
      T *color = u8x8_GetBitmapPixel(u8x8, x, y) ? fg_color : bg_color;
      img.draw_point(x, y, color);
    }
  }
  return img;
}

static uint8_t black[] = {0, 0, 0};
static uint8_t white[] = {255, 255, 255};
static uint8_t grey[] = {128, 128, 128};
static uint8_t red[] = {255, 0, 0};

using Font = CImgList<uint8_t>;
const float OPAQUE = 1;
const unsigned SOLID_LINE = ~0U;
const uint8_t SCREENSHOT_BORDER = 2;
const uint8_t SCREENSHOT_SCALE = 3;
const float ARROW_ANGLE = 30;
const float ARROW_LENGTH = 2 * SCREENSHOT_SCALE + 1; // +1 makes for a more (but not necessarily perfect) symmetrical triangle
auto ANNOTATE_FONT = Font::font(8 * SCREENSHOT_SCALE);

// Draw a non-filled rectangle, using the specified line width (in pixels)
template<typename T>
void draw_outline(CImg<T>& img, int x, int y, int w, int h, const T* color, int line_width = 1) {
  for (int i = 0; i < line_width; ++i)
    img.draw_rectangle(x + i, y + i, x + w - 1 - i, y + h - 1 - i, color, OPAQUE, SOLID_LINE);
}

enum { ALIGN_LEFT = 0, ALIGN_CENTER = 1, ALIGN_RIGHT = 2};
enum { ALIGN_TOP = 0, ALIGN_MIDDLE = 4, ALIGN_BOTTOM = 8};

// Draw aligned text
template<typename T>
void draw_text(CImg<T>& img, int x, int y, const T* color, Font& font, unsigned align, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buf[100];
  int ret = vsnprintf(buf, sizeof(buf), fmt, ap);
  assert(ret >= 0 && (size_t)ret < sizeof(buf));

  if (align) {
    auto text = CImg<uint8_t>().draw_text(0, 0, "%s", color, 0, OPAQUE, font, buf);
    if (align & ALIGN_RIGHT)
      x -= text.width();
    else if (align & ALIGN_CENTER)
      x -= text.width() / 2;
    if (align & ALIGN_BOTTOM)
      y -= text.height();
    else if (align & ALIGN_MIDDLE)
      y -= text.height() / 2;
  }
  img.draw_text(x, y, "%s", color, 0, OPAQUE, font, buf);

  va_end(ap);
}

enum {TOP, RIGHT, BOTTOM, LEFT};
struct Annotation {
  // Will be called to add annotations to the screenshot
  virtual void annotate(CImg<uint8_t>& /*img*/) const { }

  // Margins are in screenshot pixels, will be scaled later
  // Top, right, bottom, left (like CSS)
  size_t margin[4] = {0, 0, 0, 0};
};

int translate_x(float x, const Annotation& ann) {
  return (x + ann.margin[LEFT]) * SCREENSHOT_SCALE + SCREENSHOT_BORDER;
}
int translate_y(float y, const Annotation& ann) {
  return (y + ann.margin[TOP]) * SCREENSHOT_SCALE + SCREENSHOT_BORDER;
}
int translate_wh(float wh) {
  return wh * SCREENSHOT_SCALE;
}

void save_screenshot(const char *dir, const char* name, const Annotation& ann = Annotation()) {
  char filename[PATH_MAX];
  int ret = snprintf(filename, sizeof(filename), "%s/%s.png", dir, name);
  assert(ret >= 0 && (size_t)ret < sizeof(filename));

  auto screenshot = u8x8_bitmap_to_cimg<uint8_t>(&u8g2, 3, black, white);
  screenshot.resize(screenshot.width() * SCREENSHOT_SCALE, screenshot.height() * SCREENSHOT_SCALE);

  // Create a bigger output image
  size_t width = screenshot.width() + 2*SCREENSHOT_BORDER + (ann.margin[LEFT] + ann.margin[RIGHT]) * SCREENSHOT_SCALE;
  size_t height = screenshot.height() + 2*SCREENSHOT_BORDER + (ann.margin[TOP] + ann.margin[BOTTOM]) * SCREENSHOT_SCALE;
  auto output = CImg<uint8_t>(width, height, 1, 3, /* value for all channels */ 255);

  // Draw the screenshot in the center
  output.draw_image(translate_x(0, ann), translate_y(0, ann), 0, 0, screenshot);

  // Draw a 1px border around the screenshot, just inside the
  // SCREENSHOT_BORDER area (leaving a bit of white space between
  // the border and the screenshot).
  draw_outline(output, ann.margin[LEFT] * SCREENSHOT_SCALE, ann.margin[TOP] * SCREENSHOT_SCALE, screenshot.width() + 2 * SCREENSHOT_BORDER, screenshot.height() + 2 * SCREENSHOT_BORDER, grey);

  ann.annotate(output);

  output.save_png(filename);

  printf("Generated %s\n", filename);
}

struct DescribeFigures : public Annotation {
  DescribeFigures() {
    this->margin[RIGHT] = 30;
    this->margin[LEFT] = 30;
    this->margin[TOP] = 10;
    this->margin[BOTTOM] = 10;
  }

  virtual void annotate(CImg<uint8_t>& img) const {
    // Add annotations. Coordinates are converted by translate_x/y from
    // original screenshot pixels to the output screenshot pixels
    draw_text(img, translate_x(132, *this), translate_y(15, *this), red, ANNOTATE_FONT, ALIGN_LEFT|ALIGN_MIDDLE,  "Box");
    img.draw_arrow(translate_x(130, *this), translate_y(15, *this), translate_x(116, *this), translate_y(15, *this), red, OPAQUE, ARROW_ANGLE, ARROW_LENGTH);

    draw_text(img, translate_x(-4, *this), translate_y(10, *this), red, ANNOTATE_FONT, ALIGN_RIGHT|ALIGN_MIDDLE,  "Circle");
    img.draw_arrow(translate_x(-3, *this), translate_y(10, *this), translate_x(8, *this), translate_y(10, *this), red, OPAQUE, ARROW_ANGLE, ARROW_LENGTH);
  }
};

int main() {
  mkdir(OUT_DIR, 0755);

  u8g2_SetupBitmap(&u8g2, &u8g2_cb_r0, 128, 64);
  u8x8_InitDisplay(u8g2_GetU8x8(&u8g2));
  u8x8_SetPowerSave(u8g2_GetU8x8(&u8g2), 0);
  u8g2_SetFont(&u8g2, u8g2_font_helvB08_tr);

  u8g2_ClearBuffer(&u8g2);
  u8g2_DrawCircle(&u8g2, 15, 10, 5, U8G2_DRAW_ALL);
  u8g2_DrawBox(&u8g2, 100, 10, 10, 10);
  u8g2_SendBuffer(&u8g2);
  save_screenshot(OUT_DIR, "plain_screenshot");
  save_screenshot(OUT_DIR, "annotated_screenshot", DescribeFigures());

  return 0;
}
