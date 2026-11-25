# Native 2x glyph offset regression

This headless test compares `u8g2_DrawGlyphX2()`, `u8g2_DrawStrX2()` and
`u8g2_DrawUTF8X2()` with an independently doubled bitmap produced by their
native 1x counterparts. Every output pixel and the returned advance must match.
It requires only a C compiler and make; no SDL, display hardware or firmware
project is needed.

From this directory:

```sh
make test
```

With MinGW on Windows:

```sh
make CC=gcc EXEEXT=.exe test
```

The matrix covers printable ASCII in Helvetica, Times and italic Lucida fonts,
UTF-8 Chinese weekday titles in `u8g2_font_wqy12_t_gb2312`, baseline/top/bottom/
center positioning, transparent and opaque text, display-edge clipping and
explicit clip windows. Font direction and display rotation are left at zero,
as the native 2x API does not implement font-direction rotation.

The original decoder scaled bitmap dimensions and advance but not glyph
bearings: `target_x += x` and `target_y -= 2*h+y`. Scaling both bearings restores
the relative placement of glyphs, including the vertically centered strokes
in Chinese weekdays such as Monday and Tuesday.

Against upstream master `d6c8499c5f2707cac8eccd09fd8f677d12b17977`, the test reports
`18687 cases, 9743 failures`. With the offset fix it reports
`18687 cases, 0 failures`.

![Native 2x weekday text before and after scaling the glyph offsets](comparison.png)

The left column is the unmodified upstream decoder; the right column is the
fixed decoder and matches the independently doubled 1x raster pixel for pixel.

To save actual and expected weekday images, pass an output filename prefix
whose parent directory already exists:

```sh
./font_2x_offsets /tmp/before
# Rebuild with the fix, then:
./font_2x_offsets /tmp/after
```

On Windows use `font_2x_offsets.exe` and a suitable output prefix. The program
writes seven pairs of 320x128 grayscale PGM files per run. It still exits
unsuccessfully when comparisons fail, including in capture mode. The images
use the library's native 2x API directly; the expected files are generated
from the 1x raster, not by a second implementation of the 2x decoder.
