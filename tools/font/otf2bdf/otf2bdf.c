/*
 * Copyright 2008 Department of Mathematical Sciences, New Mexico State University
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * DEPARTMENT OF MATHEMATICAL SCIENCES OR NEW MEXICO STATE UNIVERSITY BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

#include <string.h>

#include <ft2build.h>
#include FT_GLYPH_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_TABLES_H

/*
 * Include the remapping support.
 */
#include "remap.h"

/**************************************************************************
 *
 * Macros.
 *
 **************************************************************************/

/*
 * The version of otf2bdf.
 */
#define OTF2BDF_VERSION "3.0"

/*
 * Set the default values used to generate a BDF font.
 */
#ifndef DEFAULT_PLATFORM_ID
#define DEFAULT_PLATFORM_ID 3
#endif

#ifndef DEFAULT_ENCODING_ID
#define DEFAULT_ENCODING_ID 1
#endif

#ifndef DEFAULT_POINT_SIZE
#define DEFAULT_POINT_SIZE 12
#endif

#ifndef DEFAULT_RESOLUTION
#define DEFAULT_RESOLUTION 100
#endif

/*
 * Used as a fallback for XLFD names where the character set/encoding can not
 * be determined.
 */
#ifndef DEFAULT_XLFD_CSET
#define DEFAULT_XLFD_CSET "-FontSpecific-0"
#endif

/*
 * nameID macros for getting strings from the OT font.
 */
#define BDFOTF_COPYRIGHT_STRING  0
#define BDFOTF_FAMILY_STRING     1
#define BDFOTF_SUBFAMILY_STRING  2
#define BDFOTF_UNIQUEID_STRING   3
#define BDFOTF_FULLNAME_STRING   4
#define BDFOTF_VENDOR_STRING     5
#define BDFOTF_POSTSCRIPT_STRING 6
#define BDFOTF_TRADEMARK_STRING  7

/*
 * String names for the string indexes. Used for error messages.
 */
static char *string_names[] = {
    "\"Copyright\"",
    "\"Family\"",
    "\"SubFamily\"",
    "\"Unique ID\"",
    "\"Full Name\"",
    "\"Vendor\"",
    "\"Postscript Name\"",
    "\"Trademark\""
};

#if 0
#define TTF_COPYRIGHT 0
#define TTF_TYPEFACE  1
#define TTF_PSNAME    6
#endif

#ifndef MAX
#define MAX(h,i) ((h) > (i) ? (h) : (i))
#endif

#ifndef MIN
#define MIN(l,o) ((l) < (o) ? (l) : (o))
#endif

/**************************************************************************
 *
 * General globals set from command line.
 *
 **************************************************************************/

/*
 * The program name.
 */
static char *prog;

/*
 * The flag indicating whether messages should be printed or not.
 */
static int verbose = 0;

/*
 * Flags used when loading glyphs.
 */
static int load_flags = FT_LOAD_DEFAULT;

/*
 * The default platform and encoding ID's.
 */
static int pid = DEFAULT_PLATFORM_ID;
static int eid = DEFAULT_ENCODING_ID;

/*
 * Default point size and resolutions.
 */
static int point_size = DEFAULT_POINT_SIZE;
static int hres = DEFAULT_RESOLUTION;
static int vres = DEFAULT_RESOLUTION;

/*
 * The user supplied foundry name to use in the XLFD name.
 */
static char *foundry_name = 0;

/*
 * The user supplied typeface name to use in the XLFD name.
 */
static char *face_name = 0;

/*
 * The user supplied weight name to use in the XLFD name.
 */
static char *weight_name = 0;

/*
 * The user supplied slant name to use in the XLFD name.
 */
static char *slant_name = 0;

/*
 * The user supplied width name to use in the XLFD name.
 */
static char *width_name = 0;

/*
 * The user supplied additional style name to use in the XLFD name.
 */
static char *style_name = 0;

/*
 * The user supplied spacing (p = proportional, c = character cell,
 * m = monospace).
 */
static int spacing = 0;

/*
 * The dash character to use in the names retrieved from the font.  Default is
 * the space.
 */
static int dashchar = ' ';

/*
 * Flag, bitmask, and max code for generating a subset of the glyphs in a font.
 */
static int do_subset = 0;
static unsigned short maxcode;
static unsigned long subset[2048];

/*
 * The flag that indicates the remapping table should be used to
 * reencode the font.
 */
static int do_remap = 0;

/**************************************************************************
 *
 * Internal globals.
 *
 **************************************************************************/

/*
 * Structure used for calculating the font bounding box as the glyphs are
 * generated.
 */
typedef struct {
    short minlb;
    short maxlb;
    short maxrb;
    short maxas;
    short maxds;
    short rbearing;
} bbx_t;

static bbx_t bbx;

/*
 * The buffer used to transfer the temporary file to the actual output file.
 */
#define OTF2BDF_IOBUFSIZ 8192
static char iobuf[OTF2BDF_IOBUFSIZ];

/*
 * The Units Per Em value used in numerous places.
 */
static FT_UShort upm;

/*
 * A flag indicating if a CMap was found or not.
 */
static FT_UShort nocmap;

/*
 * The scaling factor needed to compute the SWIDTH (scalable width) value
 * for BDF glyphs.
 */
static double swscale;

/**************************************************************************
 *
 * Platform and encoding table names.
 *
 **************************************************************************/

static char *platform_names[] = {
    "Apple Unicode", "Macintosh", "ISO", "Microsoft", "Unknown"
};
static int nplatform_names = sizeof(platform_names)/sizeof(platform_names[0]);

/*
 * Mac encoding names used when creating the BDF XLFD font name.
 */
static char *mac_encodings[] = {
    "-MacRoman-0",    "-MacJapanese-0",   "-MacChinese-0",   "-MacKorean-0",
    "-MacArabic-0",   "-MacHebrew-0",     "-MacGreek-0",     "-MacRussian-0",
    "-MacRSymbol-0",  "-MacDevanagari-0", "-MacGurmukhi-0",  "-MacGujarati-0",
    "-MacOriya-0",    "-MacBengali-0",    "-MacTamil-0",     "-MacTelugu-0",
    "-MacKannada-0",  "-MacMalayalam-0",  "-MacSinhalese-0", "-MacBurmese-0",
    "-MacKhmer-0",    "-MacThai-0",       "-MacLaotian-0",   "-MacGeorgian-0",
    "-MacArmenian-0", "-MacMaldivian-0",  "-MacTibetan-0",   "-MacMongolian-0",
    "-MacGeez-0",     "-MacSlavic-0",     "-MacVietnamese-0","-MacSindhi-0",
    "-MacUninterp-0"
};
static int nmac_encodings = sizeof(mac_encodings)/sizeof(mac_encodings[0]);

/*
 * ISO encoding names used when creating the BDF XLFD font name.
 */
static char *iso_encodings[] = {
    "-ASCII-0", "-ISO10646-1", "-ISO8859-1"
};
static int niso_encodings = sizeof(iso_encodings)/sizeof(iso_encodings[0]);

/*
 * Microsoft encoding names used when creating the BDF XLFD font name.
 */
static char *ms_encodings[] = {
    "-Symbol-0", "-ISO10646-1", "-ShiftJIS-0", "-GB2312.1980-0", "-Big5-0",
    "-KSC5601.1987-0", "-KSC5601.1992-0"
};
static int nms_encodings = sizeof(ms_encodings)/sizeof(ms_encodings[0]);

/*
 * The propery names for all the XLFD properties.
 */
static char *xlfd_props[] = {
    "FOUNDRY",
    "FAMILY_NAME",
    "WEIGHT_NAME",
    "SLANT",
    "SETWIDTH_NAME",
    "ADD_STYLE_NAME",
    "PIXEL_SIZE",
    "POINT_SIZE",
    "RESOLUTION_X",
    "RESOLUTION_Y",
    "SPACING",
    "AVERAGE_WIDTH",
    "CHARSET_REGISTRY",
    "CHARSET_ENCODING",
};

/**************************************************************************
 *
 * Freetype globals.
 *
 **************************************************************************/

static FT_Library library;
static FT_Face face;
static FT_Size_Metrics imetrics;
static TT_HoriHeader *horizontal;

/**************************************************************************
 *
 * Freetype related code.
 *
 **************************************************************************/

/*
 * A generic routine to get a name from the OT name table.  This routine
 * always looks for English language names and checks three possibilities:
 * 1. English names with the MS Unicode encoding ID.
 * 2. English names with the MS unknown encoding ID.
 * 3. English names with the Apple Unicode encoding ID.
 *
 * The particular name ID mut be provided (e.g. nameID = 0 for copyright
 * string, nameID = 6 for Postscript name, nameID = 1 for typeface name.
 *
 * If the `dash_to_space' flag is non-zero, all dashes (-) in the name will be
 * replaced with the character passed.
 *
 * Returns the number of bytes added.
 */
static int
otf_get_english_string(FT_Face face, int nameID, int dash_to_space,
                       char *name, int name_size)
{
    int j, encid;
    FT_UInt i, nrec;
    FT_SfntName sfntName;
    unsigned char *s;
    unsigned short slen;

    nrec = FT_Get_Sfnt_Name_Count(face);

    for (encid = 1, j = 0; j < 2; j++, encid--) {
        /*
         * Locate one of the MS English font names.
         */
        for (i = 0; i < nrec; i++) {
           FT_Get_Sfnt_Name(face, i, &sfntName);
           if (sfntName.platform_id == 3 &&
               sfntName.encoding_id == encid &&
               sfntName.name_id == nameID &&
               (sfntName.language_id == 0x0409 ||
                sfntName.language_id == 0x0809 ||
                sfntName.language_id == 0x0c09 ||
                sfntName.language_id == 0x1009 ||
                sfntName.language_id == 0x1409 ||
                sfntName.language_id == 0x1809)) {
               s = sfntName.string;
               slen = sfntName.string_len;
               break;
           }
        }

        if (i < nrec) {
            if (slen >> 1 >= name_size) {
                fprintf(stderr, "%s: warning: %s string longer than buffer. Truncating to %d bytes.\n", prog, string_names[nameID], name_size);
                slen = name_size << 1;
            }

            /*
             * Found one of the MS English font names.  The name is by
             * definition encoded in Unicode, so copy every second byte into
             * the `name' parameter, assuming there is enough space.
             */
            for (i = 1; i < slen; i += 2) {
                if (dash_to_space)
                  *name++ = (s[i] != '-') ? s[i] : ' ';
                else if (s[i] == '\r' || s[i] == '\n') {
                    if (s[i] == '\r' && i + 2 < slen && s[i + 2] == '\n')
                      i += 2;
                    *name++ = ' ';
                    *name++ = ' ';
                } else
                  *name++ = s[i];
            }
            *name = 0;
            return (slen >> 1);
        }
    }

    /*
     * No MS English name found, attempt to find an Apple Unicode English
     * name.
     */
    for (i = 0; i < nrec; i++) {
        FT_Get_Sfnt_Name(face, i, &sfntName);
        if (sfntName.platform_id == 0 && sfntName.language_id == 0 &&
            sfntName.name_id == nameID) {
            s = sfntName.string;
            slen = sfntName.string_len;
            break;
        }
    }

    if (i < nrec) {
        if (slen >> 1 >= name_size) {
            fprintf(stderr, "%s: warning: %s string longer than buffer. Truncating to %d bytes.\n", prog, string_names[nameID], name_size);
            slen = name_size << 1;
        }

        /*
         * Found the Apple Unicode English name.  The name is by definition
         * encoded in Unicode, so copy every second byte into the `name'
         * parameter, assuming there is enough space.
         */
        for (i = 1; i < slen; i += 2) {
            if (dash_to_space)
              *name++ = (s[i] != '-') ? s[i] : ' ';
            else if (s[i] == '\r' || s[i] == '\n') {
                if (s[i] == '\r' && i + 2 < slen && s[i + 2] == '\n')
                  i += 2;
                *name++ = ' ';
                *name++ = ' ';
            } else
              *name++ = s[i];
        }
        *name = 0;
        return (slen >> 1);
    }

    return 0;
}

/**************************************************************************
 *
 * Encoding table related functions.
 *
 **************************************************************************/

static char *
platform_name(short pid)
{
    return (pid < nplatform_names) ?
        platform_names[pid] : platform_names[nplatform_names - 1];
}

static char *
encoding_name(short pid, short eid)
{
    int nnames;
    char **names;

    switch (pid) {
      case 0: return "-ISO10646-1";
      case 1:
        nnames = nmac_encodings;
        names = mac_encodings;
        break;
      case 2:
        nnames = niso_encodings;
        names = iso_encodings;
        break;
      case 3:
        nnames = nms_encodings;
        names = ms_encodings;
        break;
      default: return "-Unknown-0";
    }

    return (eid < nnames) ? names[eid] : "-Unknown-0";
}

static char *spaces = "              ";

static void
print_encoding_table(void)
{
    int ncmaps, i, j;
    short pid, eid, lasteid;
    char *np, *platform, encoding[64];

    printf("Encoding tables available in the font:\n\n");
    printf("Platform%.*sEncoding\n", 6, spaces);
    printf("-------------------------------------------\n");
    printf("Default%.*sDefault%.*s(-pid %d -eid %d)\n",
           7, spaces, 7, spaces, DEFAULT_PLATFORM_ID, DEFAULT_ENCODING_ID);
    ncmaps = face->num_charmaps;
    for (lasteid = -1, i = 0; i < ncmaps; i++) {
        pid = face->charmaps[i]->platform_id;
        eid = face->charmaps[i]->encoding_id;
        platform = platform_name(pid);
        np = encoding_name(pid, eid);
        np++;
        for (j = 0; j < 63 && *np != '-'; np++, j++)
          encoding[j] = *np;
        encoding[j] = 0;
        /*
         * Typecast the result of 14-strlen(). This returns a size_t on
         * some platforms and causes a compilation warning.
         */
        printf("%s%.*s%s%.*s(-pid %hd -eid %hd)\n",
               platform, (int) (14 - strlen(platform)), spaces,
               encoding, (int) (14 - strlen(encoding)), spaces, pid, eid);
    }
}

/**************************************************************************
 *
 * General code.
 *
 **************************************************************************/

/*
 * Create an XLFD name.  Assumes there is enough space in the string passed
 * to fit a reasonably long XLFD name into, up to the 256 byte maximum.
 */
static void
make_xlfd_name(char *name, int name_size, FT_Long awidth, int ismono)
{
    FT_Long i;
    FT_ULong val;
    char *r, *e;
    double dr, dp;
    TT_OS2 *os2 = FT_Get_Sfnt_Table(face, ft_sfnt_os2);

    /*
     * Default the foundry name to "FreeType" in honor of the project and
     * because the foundry name is too difficult to automatically determine
     * from the names in TT fonts. But the user can provide his own.
     */
    if (foundry_name == 0) {
        (void) strcpy(name, "-FreeType");
        name += 9;
    } else {
        *(name++)='-';
        strcpy(name,foundry_name);
        name+=strlen(foundry_name);
    }

    /*
     * Add the typeface name from the font.  The fallback default will be
     * "Unknown".
     */
    *name++ = '-';
    if (face_name == 0) {
        if((i = otf_get_english_string(face, BDFOTF_FAMILY_STRING, dashchar,
                                       name, name_size)))
          name += i;
        else {
            (void) strcpy(name, "Unknown");
            name += 7;
        }
    } else {
        (void) strcpy(name, face_name);
        name += strlen(face_name);
    }

    /*
     * Add the weight name.  The default will be "Medium".
     */
    if (weight_name != 0) {
        sprintf(name, "-%s", weight_name);
        name += strlen(weight_name) + 1;
    } else {
        if (os2->fsSelection & 0x20) {
            (void) strcpy(name, "-Bold");
            name += 5;
        } else {
            (void) strcpy(name, "-Medium");
            name += 7;
        }
    }

    /*
     * Add the slant name.  The default will be 'R'.
     */
    if (slant_name) {
        sprintf(name, "-%s", slant_name);
        name += strlen(slant_name) + 1;
    } else {
        *name++ = '-';
        if (os2->fsSelection & 0x01)
          *name++ = 'I';
        else
          *name++ = 'R';
    }

    /*
     * Default the setwidth name to "Normal" but user can specify one.
     */
    if (width_name == 0) {
        (void) strcpy(name, "-Normal");
        name += 7;
    } else {
        *(name++)='-';
        (void) strcpy(name, width_name);
        name += strlen(width_name);
    }

    /*
     * Default the additional style name to NULL but user can specify one.
     */
    *name++ = '-';
    if (style_name != 0) {
        (void) strcpy(name, style_name);
        name += strlen(style_name);
    }

    /*
     * Determine the pixel size from the point size and resolution.
     */
    dr = (double) vres;
    dp = (double) (point_size * 10);
    val = (unsigned long) (((dp * dr) / 722.7) + 0.5);

    /*
     * Set the pixel size, point size, and resolution.
     */
    sprintf(name, "-%ld-%d-%d-%d", val, point_size * 10, hres, vres);
    name += strlen(name);

    switch (spacing) {
      case 'p': case 'P': spacing = 'P'; break;
      case 'm': case 'M': spacing = 'M'; break;
      case 'c': case 'C': spacing = 'C'; break;
      default: spacing = 0; break;
    }

    /*
     * Set the spacing.
     */
    if (!spacing)
      spacing = (ismono) ? 'M' : 'P';
    *name++ = '-';
    *name++ = spacing;

    /*
     * Add the average width.
     */
    sprintf(name, "-%ld", awidth);
    name += strlen(name);

    /*
     * Check to see if the remapping table specified a registry and encoding
     * and use those if they both exist.
     */
    otf2bdf_remap_charset(&r, &e);
    if (r != 0 && e != 0) {
        sprintf(name, "-%s-%s", r, e);
        return;
    }

    /*
     * If the cmap for the platform and encoding id was not found, or the
     * platform id is unknown, assume the character set registry and encoding
     * are the XLFD default.
     */
    if (nocmap || pid > 3)
      (void) strcpy(name, DEFAULT_XLFD_CSET);
    else {
        /*
         * Finally, determine the character set registry and encoding from the
         * platform and encoding ID.
         */
        switch (pid) {
          case 0:
            /*
             * Apple Unicode platform, so "-Apple-Unicode" is the default.
             */
            (void) strcpy(name, "-Apple-Unicode");
            break;
          case 1:
            /*
             * Macintosh platform, so choose from the Macintosh encoding
             * strings.
             */
            if (eid < 0 || eid >= nmac_encodings)
              (void) strcpy(name, DEFAULT_XLFD_CSET);
            else
              (void) strcpy(name, mac_encodings[eid]);
            break;
          case 2:
            /*
             * ISO platform, so choose from the ISO encoding strings.
             */
            if (eid < 0 || eid >= niso_encodings)
              (void) strcpy(name, DEFAULT_XLFD_CSET);
            else
              (void) strcpy(name, iso_encodings[eid]);
            break;
          case 3:
            /*
             * Microsoft platform, so choose from the MS encoding strings.
             */
            if (eid < 0 || eid >= nms_encodings)
              (void) strcpy(name, DEFAULT_XLFD_CSET);
            else
              (void) strcpy(name, ms_encodings[eid]);
            break;
        }
    }
}

static int
generate_font(FILE *out, char *iname, char *oname)
{
    int eof, ismono, i;
    FILE *tmp;
    FT_Short x, y, dwidth, swidth;
    FT_Short y_off, x_off;
    FT_Long sx, sy, ex, ey, wd, ht;
    FT_Long code, idx, ng, aw;
    FT_UShort remapped_code;
    unsigned char *bp;
    double dw;
    char *xp, xlfd[BUFSIZ];
    char *tmpdir, tmpfile[BUFSIZ];

    imetrics = face->size->metrics;
    horizontal = FT_Get_Sfnt_Table(face, ft_sfnt_hhea);

    /*
     * Clear the BBX.
     */
    memset((char *) &bbx, 0, sizeof(bbx_t));

    /*
     * Open a temporary file to store the bitmaps in until the exact number
     * of bitmaps are known.
     */
    if ((tmpdir = getenv("TMPDIR")) == 0)
      tmpdir = "/tmp";
    sprintf(tmpfile, "%s/otf2bdf%ld", tmpdir, (long) getpid());
    if ((tmp = fopen(tmpfile, "w")) == 0) {
        fprintf(stderr, "%s: unable to open temporary file '%s'.\n",
                prog, tmpfile);
        return -1;
    }

    /*
     * Calculate the scale factor for the SWIDTH field.
     */
    swscale = ((double) vres) * ((double) point_size);

    /*
     * Initialize the flag that tracks if the font is monowidth or not and
     * initialize the glyph width variable that is used for testing for a
     * monowidth font.
     */
    wd = 0xffff;
    ismono = 1;

    for (ng = code = 0, eof = 0, aw = 0; eof != EOF && code < 0xffff; code++) {

        /*
         * If a remap is indicated, attempt to remap the code.  If a remapped
         * code is not found, then skip generating the glyph.
         */
        remapped_code = (FT_UShort) code;
        if (do_remap && !otf2bdf_remap(&remapped_code))
          continue;

        /*
         * If a subset is being generated and the code is greater than the max
         * code of the subset, break out of the loop to avoid doing any more
         * work.
         */
        if (do_subset && remapped_code > maxcode)
          break;

        /*
         * If a subset is being generated and the index is not in the subset
         * bitmap, just continue.
         */
        if (do_subset &&
            !(subset[remapped_code >> 5] & (1 << (remapped_code & 31))))
          continue;

        if (nocmap) {
            if (code >= face->num_glyphs)

              /*
               * At this point, all the glyphs are done.
               */
              break;
            idx = code;
        } else
          idx = FT_Get_Char_Index(face, code);

        /*
         * If the glyph could not be loaded for some reason, or a subset is
         * being generated and the index is not in the subset bitmap, just
         * continue.
         */

        if (idx <= 0 || FT_Load_Glyph(face, idx, load_flags))
          continue;

        if (FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO))
          continue;

        /*
         * Determine the DWIDTH (device width, or advance width in TT terms)
         * and the SWIDTH (scalable width) values.
         */
        dwidth = face->glyph->metrics.horiAdvance >> 6;
        dw = (double) dwidth;
        swidth = (FT_Short) ((dw * 72000.0) / swscale);

        /*
         * Determine the actual bounding box of the glyph bitmap.  Do not
         * forget that the glyph is rendered upside down!
         */
        sx = sy = 0xffff;
        ex = ey = 0;
        bp = face->glyph->bitmap.buffer;
        for (y = 0; y < face->glyph->bitmap.rows; y++) {
            for (x = 0; x < face->glyph->bitmap.width; x++) {
                if (bp[x >> 3] & (0x80 >> (x & 7))) {
                    if (x < sx) sx = x;
                    if (x > ex) ex = x;
                    if (y < sy) sy = y;
                    if (y > ey) ey = y;
                }
            }
            bp += face->glyph->bitmap.pitch;
        }

        /*
         * If the glyph is actually an empty bitmap, set the size to 0 all
         * around.
         */
        if (sx == 0xffff && sy == 0xffff && ex == 0 && ey == 0)
          sx = ex = sy = ey = 0;
        else {
            /*
             * Adjust the end points.
             */
            ex++;
            ey++;
        }

        /*
         * Increment the number of glyphs generated.
         */
        ng++;

        /*
         * Test to see if the font is going to be monowidth or not by
         * comparing the current glyph width against the last one.
         */
        if (wd != 0xffff && ismono && (ex - sx) + 1 != wd)
          ismono = 0;

        /*
         * Adjust the font bounding box.
         */
        wd = ex - sx;
        ht = ey - sy;
        x_off = sx + face->glyph->bitmap_left;
        y_off = sy + face->glyph->bitmap_top - face->glyph->bitmap.rows;

        bbx.maxas = MAX(bbx.maxas, ht + y_off);
        bbx.maxds = MAX(bbx.maxds, -y_off);
        bbx.rbearing = wd + x_off;
        bbx.maxrb = MAX(bbx.maxrb, bbx.rbearing);
        bbx.minlb = MIN(bbx.minlb, x_off);
        bbx.maxlb = MAX(bbx.maxlb, x_off);

        /*
         * Add to the average width accumulator.
         */
        aw += wd;

        /*
         * Print the bitmap header.
         */
        fprintf(tmp, "STARTCHAR %04lX\nENCODING %ld\n", code,
                (long) remapped_code);
        fprintf(tmp, "SWIDTH %hd 0\n", swidth);
        fprintf(tmp, "DWIDTH %hd 0\n", dwidth);
        fprintf(tmp, "BBX %ld %ld %hd %hd\n", wd, ht, x_off, y_off);

        /*
         * Check for an error return here in case the temporary file system
         * fills up or the file is deleted while it is being used.
         */
        eof = fprintf(tmp, "BITMAP\n");

        bp = face->glyph->bitmap.buffer + (sy * face->glyph->bitmap.pitch);
        for (y = 0; eof != EOF && y < ey - sy; y++) {
            for (idx = 0, x = 0; eof != EOF && x < ex - sx; x++) {
                if (x > 0 && (x & 7) == 0) {
                    /*
                     * Print the next byte.
                     */
                    eof = fprintf(tmp, "%02lX", idx & 0xff);
                    idx = 0;
                }
                if (bp[(x+sx) >> 3] & (0x80 >> ((x+sx) & 7)))
                  idx |= (0x80 >> (x & 7));
            }
            bp += face->glyph->bitmap.pitch;
            if (eof != EOF)
              /*
               * Because of the structure of the loop, the last byte should
               * always be printed.
               */
              fprintf(tmp, "%02lX\n", idx & 0xff);
        }
        if (eof != EOF)
          fprintf(tmp, "ENDCHAR\n");
    }

    fclose(tmp);

    /*
     * If a write error occured, delete the temporary file and issue an error
     * message.
     */
    if (eof == EOF) {
        (void) unlink(tmpfile);
        fprintf(stderr, "%s: problem writing to temporary file '%s'.\n",
                prog, tmpfile);
        return -1;
    }

    /*
     * If no characters were generated, just unlink the temp file and issue a
     * warning.
     */
    if (ng == 0) {
        (void) unlink(tmpfile);
        fprintf(stderr, "%s: no glyphs generated from '%s'.\n", prog, iname);
        return -1;
    }

    /*
     * Reopen the temporary file so it can be copied to the actual output
     * file.
     */
    if ((tmp = fopen(tmpfile, "r")) == 0) {
        /*
         * Unable to open the file for read, so attempt to delete it and issue
         * an error message.
         */
        (void) unlink(tmpfile);
        fprintf(stderr, "%s: unable to open temporary file '%s' for read.\n",
                prog, tmpfile);
        return -1;
    }

    /*
     * Calculate the average width.
     */
    aw = (FT_Long) ((((double) aw / (double) ng) + 0.5) * 10.0);

    /*
     * Generate the XLFD font name.
     */
    make_xlfd_name(xlfd, sizeof(xlfd), aw, ismono);

    /*
     * Start writing the font out.
     */
    fprintf(out, "STARTFONT 2.1\n");

    /*
     * Add the vanity comments.
     */
    fprintf(out, "COMMENT\n");
    fprintf(out, "COMMENT Converted from OpenType font \"%s\" by \"%s %s\".\n",
            iname, prog, OTF2BDF_VERSION);
    fprintf(out, "COMMENT\n");

    fprintf(out, "FONT %s\n", xlfd);
    fprintf(out, "SIZE %d %d %d\n", point_size, hres, vres);

    /*
     * Generate the font bounding box.
     */
    fprintf(out, "FONTBOUNDINGBOX %hd %hd %hd %hd\n",
            bbx.maxrb - bbx.minlb, bbx.maxas + bbx.maxds,
            bbx.minlb, -bbx.maxds);

    /*
     * Print the properties.
     */
    fprintf(out, "STARTPROPERTIES %hd\n", 19);

    /*
     * Print the font properties from the XLFD name.
     */
    for (i = 0, xp = xlfd; i < 14; i++) {
        /*
         * Print the XLFD property name.
         */
        fprintf(out, "%s ", xlfd_props[i]);

        /*
         * Make sure the ATOM properties are wrapped in double quotes.
         */
        if (i < 6 || i == 10 || i > 11)
          putc('"', out);

        /*
         * Skip the leading '-' in the XLFD name.
         */
        xp++;

        /*
         * Skip until the next '-' or NULL.
         */
        for (; *xp && *xp != '-'; xp++)
          putc(*xp, out);

        /*
         * Make sure the ATOM properties are wrapped in double quotes.
         */
        if (i < 6 || i == 10 || i > 11)
          putc('"', out);

        putc('\n', out);
    }

    /*
     * Make sure to add the FONT_ASCENT and FONT_DESCENT properties
     * because X11 can not live without them.
     */
    fprintf(out, "FONT_ASCENT %hd\nFONT_DESCENT %hd\n",
            (horizontal->Ascender * imetrics.y_ppem) / upm,
            -((horizontal->Descender * imetrics.y_ppem) / upm));

    /*
     * Get the copyright string from the font.
     */
    (void) otf_get_english_string(face, BDFOTF_COPYRIGHT_STRING, 0, xlfd,
                                  sizeof(xlfd));
    fprintf(out, "COPYRIGHT \"%s\"\n", xlfd);

    /*
     * Last, print the two user-defined properties _OTF_FONTFILE and
     * _OTF_PSNAME.  _OTF_FONTFILE provides a reference to the original OT
     * font file which some systems can take advantage of, and _OTF_PSNAME
     * provides the Postscript name of the font if it exists.
     */
    (void) otf_get_english_string(face, BDFOTF_POSTSCRIPT_STRING, 0, xlfd,
                                  sizeof(xlfd));
    fprintf(out, "_OTF_FONTFILE \"%s\"\n_OTF_PSNAME \"%s\"\n", iname, xlfd);

    fprintf(out, "ENDPROPERTIES\n");

    /*
     * Print the actual number of glyphs to the output file.
     */
    eof = fprintf(out, "CHARS %ld\n", ng);

    /*
     * Copy the temporary file to the output file.
     */
    while (eof != EOF && (ng = fread(iobuf, 1, OTF2BDF_IOBUFSIZ, tmp))) {
        if (fwrite(iobuf, 1, ng, out) == 0)
          eof = EOF;
    }
        
    /*
     * Close the temporary file and delete it.
     */
    fclose(tmp);
    (void) unlink(tmpfile);

    /*
     * If an error occured when writing to the output file, issue a warning
     * and return.
     */
    if (eof == EOF) {
        fprintf(stderr, "%s: problem writing to output file '%s'.\n",
                prog, oname);
        return -1;
    }

    /*
     * End the font and do memory cleanup on the glyph and raster structures.
     */
    eof = fprintf(out, "ENDFONT\n");

    return eof;
}

static int
generate_bdf(FILE *out, char *iname, char *oname)
{
    FT_Long i;

    /*
     * Get the requested cmap.
     */
    for (i = 0; i < face->num_charmaps; i++) {
        if (face->charmaps[i]->platform_id == pid &&
            face->charmaps[i]->encoding_id == eid)
          break;
    }
    if (i == face->num_charmaps && pid == 3 && eid == 1) {
        /*
         * Make a special case when this fails with pid == 3 and eid == 1.
         * Change to eid == 0 and try again.  This captures the two possible
         * cases for MS fonts.  Some other method should be used to cycle
         * through all the alternatives later.
         */
        for (i = 0; i < face->num_charmaps; i++) {
        if (face->charmaps[i]->platform_id == pid &&
            face->charmaps[i]->encoding_id == 0)
              break;
        }
        if (i < face->num_charmaps) {
            pid = 3;
            eid = 1;
            FT_Set_Charmap(face, face->charmaps[i]);
        } else {
            /*
             * No CMAP was found.
             */
            nocmap = 1;
            pid = eid = -1;
        }
    } else {
        FT_Set_Charmap(face, face->charmaps[i]);
        nocmap = 0;
    }

    if (nocmap && verbose) {
        fprintf(stderr,
                    "%s: no character map for platform %d encoding %d.  ",
                    prog, pid, eid);
        fprintf(stderr, "Generating all glyphs.\n");
    }

    /*
     * Now go through and generate the glyph bitmaps themselves.
     */
    return generate_font(out, iname, oname);
}

#define isdig(cc) ((cc) >= '0' && (cc) <= '9')

/*
 * Routine to parse a subset specification supplied on the command line.
 * The syntax for this specification is the same as the syntax used for
 * the XLFD font names (XLFD documentation, page 9).
 *
 * Example:
 *
 *  "60 70 80_90" means the glyphs at codes 60, 70, and between 80 and
 *  90 inclusive.
 */
static void
parse_subset(char *s)
{
    long l, r;

    /*
     * Make sure to clear the flag and bitmap in case more than one subset is
     * specified on the command line.
     */
    maxcode = 0;
    do_subset = 0;
    (void) memset((char *) subset, 0, sizeof(unsigned long) * 2048);

    while (*s) {
        /*
         * Collect the next code value.
         */
        for (l = r = 0; *s && isdig(*s); s++)
          l = (l * 10) + (*s - '0');

        /*
         * If the next character is an '_', advance and collect the end of the
         * specified range.
         */
        if (*s == '_') {
            s++;
            for (; *s && isdig(*s); s++)
              r = (r * 10) + (*s - '0');
        } else
          r = l;

        /*
         * Add the range just collected to the subset bitmap and set the flag
         * that indicates a subset is wanted.
         */
        for (; l <= r; l++) {
            do_subset = 1;
            subset[l >> 5] |= (1 << (l & 31));
            if (l > maxcode)
              maxcode = l;
        }

        /*
         * Skip all non-digit characters.
         */
        while (*s && !isdig(*s))
          s++;
    }
}

static void
usage(int eval)
{
    printf("Usage: %s [options below] font.ttf\n", prog);
    printf("-h\t\tThis message.\n");
    printf("-v\t\tPrint warning messages during conversion.\n");
    printf("-l \"subset\"\tSpecify a subset of glyphs to generate.\n");
    printf("-m mapfile\tGlyph reencoding file.\n");
    printf("-n\t\tTurn off glyph hinting.\n");
    printf("-a\t\tForce auto hinting.\n");
    printf("-et\t\tDisplay the encoding tables available in the font.\n");
    printf("-c c\t\tSet the character spacing (default: from font).\n");
    printf("-f name\t\tSet the foundry name (default: freetype).\n");
    printf("-t name\t\tSet the typeface name (default: from font).\n");
    printf("-w name\t\tSet the weight name (default: Medium).\n");
    printf("-s name\t\tSet the slant name (default: R).\n");
    printf("-k name\t\tSet the width name (default: Normal).\n");
    printf("-d name\t\tSet the additional style name (default: empty).\n");
    printf("-u char\t\tSet the character to replace '-' in names ");
    printf("(default: space).\n");
    printf("-pid id\t\tSet the platform ID for encoding (default: %d).\n",
            DEFAULT_PLATFORM_ID);
    printf("-eid id\t\tSet the encoding ID for encoding (default: %d).\n",
            DEFAULT_ENCODING_ID);
    printf("-p n\t\tSet the point size (default: %dpt).\n",
           DEFAULT_POINT_SIZE);
    printf("-r n\t\tSet the horizontal and vertical resolution ");
    printf("(default: %ddpi).\n", DEFAULT_RESOLUTION);
    printf("-rh n\t\tSet the horizontal resolution ");
    printf("(default: %ddpi)\n", DEFAULT_RESOLUTION);
    printf("-rv n\t\tSet the vertical resolution ");
    printf("(default: %ddpi)\n", DEFAULT_RESOLUTION);
    printf("-o outfile\tSet the output filename (default: stdout).\n");
    exit(eval);
}

int
main(int argc, char *argv[])
{
    int res, pet;
    char *infile, *outfile, *iname, *oname;
    FILE *out, *mapin;

    if ((prog = strrchr(argv[0], '/')))
      prog++;
    else
      prog = argv[0];

    /*
     * Flag indicating whether the encoding tables are supposed to be printed
     * or not.
     */
    pet = 0;

    out = stdout;
    infile = outfile = 0;

    argc--;
    argv++;

    while (argc > 0) {
        if (argv[0][0] == '-') {
            switch (argv[0][1]) {
              case 'v': case 'V':
                verbose = 1;
                break;
              case 'l': case 'L':
                argc--;
                argv++;
                parse_subset(argv[0]);
                break;
              case 'n': case 'N':
                load_flags |= FT_LOAD_NO_HINTING;
                break;
              case 'a': case 'A':
                load_flags |= FT_LOAD_FORCE_AUTOHINT;
                break;
              case 'c': case 'C':
                argc--;
                argv++;
                spacing = argv[0][0];
                break;
              case 't': case 'T':
                argc--;
                argv++;
                face_name = argv[0];
                break;
              case 'w': case 'W':
                argc--;
                argv++;
                weight_name = argv[0];
                break;
              case 's': case 'S':
                argc--;
                argv++;
                slant_name = argv[0];
                break;
              case 'k': case 'K':
                argc--;
                argv++;
                width_name = argv[0];
                break;
              case 'd': case 'D':
                argc--;
                argv++;
                style_name = argv[0];
                break;
              case 'f': case 'F':
                argc--;
                argv++;
                foundry_name = argv[0];
                break;
              case 'u': case 'U':
                argc--;
                argv++;
                dashchar = argv[0][0];
                break;
              case 'p': case 'P':
                res = argv[0][2];
                argc--;
                argv++;
                if (res == 'i' || res == 'I') {
                    /*
                     * No need to print the encoding tables if the user
                     * is supplying a platform ID.
                     */
                    pet = 0;

                    /*
                     * Set the platform ID.
                     */
                    pid = atoi(argv[0]);
                } else
                  /*
                   * Set the point size.
                   */
                  point_size = atoi(argv[0]);
                break;
              case 'e': case 'E':
                if (argv[0][2] == 't' || argv[0][2] == 'T')
                  pet = 1;
                else {
                    /*
                     * No need to print the encoding tables if the user
                     * is supplying a platform ID.
                     */
                    pet = 0;

                    /*
                     * Set the encoding ID.
                     */
                    argc--;
                    argv++;
                    eid = atoi(argv[0]);
                }
                break;
              case 'r':
                /*
                 * Set the horizontal and vertical resolutions.
                 */
                if (argv[0][2] == 'h')
                  hres = atoi(argv[1]);
                else if (argv[0][2] == 'v')
                  vres = atoi(argv[1]);
                else
                  hres = vres = atoi(argv[1]);
                argc--;
                argv++;
                break;
              case 'm': case 'M':
                /*
                 * Try to load a remap table.
                 */
                argc--;
                argv++;

                /*
                 * Always reset the `do_remap' variable here in case more than
                 * one map file appears on the command line.
                 */
                do_remap = 0;
                if ((mapin = fopen(argv[0], "r")) == 0)
                  fprintf(stderr, "%s: unable to open the remap table '%s'.\n",
                          prog, argv[0]);
                else {
                    if (otf2bdf_load_map(mapin) < 0) {
                        fprintf(stderr,
                                "%s: problem loading remap table '%s'.\n",
                                prog, argv[0]);
                        do_remap = 0;
                    } else
                      do_remap = 1;
                    fclose(mapin);
                }
                break;
              case 'o': case 'O':
                /*
                 * Set the output file name.
                 */
                argc--;
                argv++;
                outfile = argv[0];
                break;
              default:
                usage(1);
            }
        } else
          /*
           * Set the input file name.
           */
          infile = argv[0];

        argc--;
        argv++;
    }

    /*
     * Validate the values passed on the command line.
     */
    if (infile == 0) {
        fprintf(stderr, "%s: no input file provided.\n", prog);
        usage(1);
    }
    /*
     * Set the input filename that will be passed to the generator
     * routine.
     */
    if ((iname = strrchr(infile, '/')))
      iname++;
    else
      iname = infile;

    /*
     * Check the platform and encoding IDs.
     */
    if (pid < 0 || pid > 255) {
        fprintf(stderr, "%s: invalid platform ID '%d'.\n", prog, pid);
        exit(1);
    }
    if (eid < 0 || eid > 65535) {
        fprintf(stderr, "%s: invalid encoding ID '%d'.\n", prog, eid);
        exit(1);
    }

    /*
     * Arbitrarily limit the point size to a minimum of 2pt and maximum of
     * 256pt.
     */
    if (point_size < 2 || point_size > 256) {
        fprintf(stderr, "%s: invalid point size '%dpt'.\n", prog, point_size);
        exit(1);
    }

    /*
     * Arbitrarily limit the resolutions to a minimum of 10dpi and a maximum
     * of 1200dpi.
     */
    if (hres < 10 || hres > 1200) {
        fprintf(stderr, "%s: invalid horizontal resolution '%ddpi'.\n",
                prog, hres);
        exit(1);
    }
    if (vres < 10 || vres > 1200) {
        fprintf(stderr, "%s: invalid vertical resolution '%ddpi'.\n",
                prog, vres);
        exit(1);
    }

    /*
     * Open the output file if specified.
     */
    if (outfile != 0) {
        /*
         * Attempt to open the output file.
         */
        if ((out = fopen(outfile, "w")) == 0) {
            fprintf(stderr, "%s: unable to open the output file '%s'.\n",
                    prog, outfile);
            exit(1);
        }
        /*
         * Set the output filename to be passed to the generator routine.
         */
        if ((oname = strrchr(outfile, '/')))
          oname++;
        else
          oname = outfile;
    } else
      /*
       * Set the default output file name to <stdout>.
       */
      oname = "<stdout>";

    /*
     * Intialize Freetype.
     */
    if ((res = FT_Init_FreeType(&library))) {
        /*
         * Close the output file.
         */
        if (out != stdout) {
            fclose(out);
            (void) unlink(outfile);
        }
        fprintf(stderr, "%s[%d]: unable to initialize renderer.\n",
                prog, res);
        exit(1);
    }

    /*
     * Open the input file.
     */
    if ((res = FT_New_Face(library, infile, 0, &face))) {
        if (out != stdout) {
            fclose(out);
            (void) unlink(outfile);
        }
        fprintf(stderr, "%s[%d]: unable to open input file '%s'.\n",
                prog, res, infile);
        exit(1);
    }

    if (pet)
      /*
       * Simply print the encoding tables and do nothing else.
       */
      print_encoding_table();
    else {
        /*
         * Set the instance resolution and point size and the relevant
         * metrics.
         */
        FT_Set_Char_Size(face, 0, point_size * 64, hres, vres);

        /*
         * Set the global units per em value for convenience.
         */
        upm = face->units_per_EM;

        /*
         * Generate the BDF font from the TrueType font.
         */
        res = generate_bdf(out, iname, oname);
    }

    /*
     * Free up the mapping table if one was loaded.
     */
    otf2bdf_free_map();

    /*
     * Close the input and output files.
     */
    (void) FT_Done_Face(face);
    if (out != stdout) {
        fclose(out);
        if (res < 0)
          /*
           * An error occured when generating the font, so delete the
           * output file.
           */
          (void) unlink(outfile);
    }

    /*
     * Shut down the renderer.
     */
    FT_Done_FreeType(library);

    exit(res);

    return 0;
}
