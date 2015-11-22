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
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <stdlib.h>
#include <unistd.h>
#endif

/*
 * Structure for managing simple lists in place.
 */
typedef struct {
    unsigned char *bfield;
    unsigned long bsize;
    unsigned long bused;
    unsigned char **field;
    unsigned long size;
    unsigned long used;
} list_t;

/*
 * Callback type used with the high speed text file reader function.
 */
typedef int (*scanlines_callback_t)(unsigned char *line, unsigned long linelen,
                                    unsigned long lineno, void *client_data);

/*
 * Various utility routines.
 */

#define setsbit(m, cc) (m[(cc) >> 3] |= (1 << ((cc) & 7)))
#define sbitset(m, cc) (m[(cc) >> 3] & (1 << ((cc) & 7)))

/*
 * An empty string for empty fields.
 */
static unsigned char empty[1] = { 0 };

/*
 * Assume the line is NULL terminated and that the `list' parameter was
 * initialized the first time it was used.
 */
static void
splitline(unsigned char *separators, unsigned char *line,
          unsigned long linelen, list_t *list)
{
    int mult, final_empty;
    unsigned char *sp, *ep, *end;
    unsigned char seps[32];

    /*
     * Initialize the list.
     */
    list->used = list->bused = 0;

    /*
     * If the line is empty, then simply return.
     */
    if (linelen == 0 || line[0] == 0)
      return;

    /*
     * If the `separators' parameter is NULL or empty, split the list into
     * individual bytes.
     */
    if (separators == 0 || *separators == 0) {
        if (linelen > list->bsize) {
            if (list->bsize)
              list->bfield = (unsigned char *) malloc(linelen);
            else
              list->bfield = (unsigned char *) realloc(list->bfield, linelen);
            list->bsize = linelen;
        }
        list->bused = linelen;
        (void) memcpy(list->bfield, line, linelen);
        return;
    }

    /*
     * Prepare the separator bitmap.
     */
    (void) memset((char *) seps, 0, 32);

    /*
     * If the very last character of the separator string is a plus, then set
     * the `mult' flag to indicate that multiple separators should be
     * collapsed into one.
     */
    for (mult = 0, sp = separators; sp && *sp; sp++) {
        if (*sp == '+' && *(sp + 1) == 0)
          mult = 1;
        else
          setsbit(seps, *sp);
    }

    /*
     * Break the line up into fields.
     */
    for (final_empty = 0, sp = ep = line, end = sp + linelen;
         sp < end && *sp;) {
        /*
         * Collect everything that is not a separator.
         */
        for (; ep < end && *ep && !sbitset(seps, *ep); ep++) ;

        /*
         * Resize the list if necessary.
         */
        if (list->used == list->size) {
            if (list->size == 0)
              list->field = (unsigned char **)
                  malloc(sizeof(unsigned char *) << 3);
            else
              list->field = (unsigned char **)
                  realloc((char *) list->field,
                          sizeof(unsigned char *) * (list->size + 8));

            list->size += 8;
        }

        /*
         * Assign the field appropriately.
         */
        list->field[list->used++] = (ep > sp) ? sp : empty;

        sp = ep;
        if (mult) {
            /*
             * If multiple separators should be collapsed, do it now by
             * setting all the separator characters to 0.
             */
            for (; ep < end && *ep && sbitset(seps, *ep); ep++)
              *ep = 0;
        } else
          /*
           * Don't collapse multiple separators by making them 0, so just
           * make the one encountered 0.
           */
          *ep++ = 0;
        final_empty = (ep > sp && *ep == 0);
        sp = ep;
    }

    /*
     * Finally, NULL terminate the list.
     */
    if (list->used + final_empty + 1 >= list->size) {
        if (list->used == list->size) {
            if (list->size == 0)
              list->field = (unsigned char **)
                  malloc(sizeof(unsigned char *) << 3);
            else
              list->field = (unsigned char **)
                  realloc((unsigned char *) list->field,
                          sizeof(char *) * (list->size + 8));
            list->size += 8;
        }
    }
    if (final_empty)
      list->field[list->used++] = empty;

    if (list->used == list->size) {
        if (list->size == 0)
          list->field = (unsigned char **)
              malloc(sizeof(unsigned char *) << 3);
        else
          list->field = (unsigned char **)
              realloc((char *) list->field,
                      sizeof(unsigned char *) * (list->size + 8));
        list->size += 8;
    }
    list->field[list->used] = 0;
}

static int
scanlines(int fd, scanlines_callback_t callback, void *client_data,
          unsigned long *lineno)
{
    unsigned long lno;
    int n, res, done, refill, bytes, hold;
    char *ls, *le, *pp, *pe, *hp;
    char buf[65536];

    if (callback == 0)
      return -1;

    lno = 1;
    (void) memset(buf, 0, 65536);
    res = done = 0;
    pp = ls = le = buf;
    bytes = 65536;
    while (!done && (n = read(fd, pp, bytes)) > 0) {
        /*
         * Determine the new end of the buffer pages.
         */
        pe = pp + n;

        for (refill = 0; done == 0 && refill == 0; ) {
            while (le < pe && *le != '\n' && *le != '\r')
              le++;

            if (le == pe) {
                /*
                 * Hit the end of the last page in the buffer.
                 * Need to find out how many pages to shift
                 * and how many pages need to be read in.
                 * Adjust the line start and end pointers down
                 * to point to the right places in the pages.
                 */
                pp = buf + (((ls - buf) >> 13) << 13);
                n = pp - buf;
                ls -= n;
                le -= n;
                n = pe - pp;
                (void) memcpy(buf, pp, n);
                pp = buf + n;
                bytes = 65536 - n;
                refill = 1;
            } else {
                /*
                 * Temporarily NULL terminate the line.
                 */
                hp = le;
                hold = *le;
                *le = 0;

                if (callback && *ls != '#' && *ls != 0x1a && le > ls &&
                    (res = (*callback)((unsigned char *) ls, le - ls, lno,
                                       client_data)) != 0)
                  done = 1;
                else {
                    ls = ++le;
                    /*
                     * Handle the case of DOS CRLF sequences.
                     */
                    if (le < pe && hold == '\n' && *le =='\r')
                      ls = ++le;
                }

                /*
                 * Increment the line number.
                 */
                lno++;

                /*
                 * Restore the character at the end of the line.
                 */
                *hp = hold;
            }
        }
    }

    /*
     * Return with the last line number processed.
     */
    *lineno = lno;

    return res;
}

static unsigned char a2i[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned char odigits[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static unsigned char ddigits[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static unsigned char hdigits[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03,
    0x7e, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define isdigok(m, d) (m[(d) >> 3] & (1 << ((d) & 7)))

static unsigned short
my_atous(unsigned char *s, unsigned char **end, int base)
{
    unsigned short v;
    unsigned char *dmap;

    if (s == 0 || *s == 0)
      return 0;

    /*
     * Make sure the radix is something recognizable.  Default to 10.
     */
    switch (base) {
      case 8: dmap = odigits; break;
      case 16: dmap = hdigits; break;
      default: base = 10; dmap = ddigits; break;
    }

    /*
     * Check for the special hex prefix.
     */
    if (*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
        base = 16;
        dmap = hdigits;
        s += 2;
    }

    for (v = 0; isdigok(dmap, *s); s++)
      v = (v * base) + a2i[(int) *s];

    if (end != 0)
      *end = s;

    return v;
}

/********************************************************************
 *
 * Routines to load, unload, and use mapping tables to remap BDF fonts
 * during generation by otf2bdf.
 *
 ********************************************************************/

/*
 * Strings used to store the registry and encoding values specified
 * in the mapping table.
 */
static char *registry;
static char *encoding;

/*
 * Trie node structure.
 */
typedef struct {
    unsigned short key;	/* Key value.					*/
    unsigned short val;	/* Data for the key.				*/
    unsigned long sibs;	/* Offset of siblings from trie beginning.	*/
    unsigned long kids; /* Offset of children from trie beginning.	*/
} node_t;

/*
 * The trie used for remapping codes.
 */
static node_t *nodes;
static unsigned long nodes_size = 0;
static unsigned long nodes_used = 0;

/*
 * Gets the next available node in the trie.
 */
static unsigned long
getnode(unsigned short key)
{
    unsigned long loc;
    node_t *np;

    if (nodes_used == nodes_size) {
        if (nodes_size == 0)
          nodes = (node_t *) malloc(sizeof(node_t) << 7);
        else
          nodes = (node_t *) realloc((char *) nodes, sizeof(node_t) *
                                     (nodes_size + 128));
        np = nodes + nodes_size;
        nodes_size += 128;
        (void) memset((char *) np, 0, sizeof(node_t) << 7);
    }

    loc = nodes_used++;
    np = nodes + loc;
    np->kids = np->sibs = 0;
    np->key = key;
    return loc;
}

/*
 * Inserts a node in the trie.
 */
static void
trie_insert(unsigned short key, unsigned short val)
{
    unsigned long i, n, t, l;
    unsigned short codes[2];

    /*
     * Convert the incoming key into two codes to make the trie lookup more
     * efficient.
     */
    codes[0] = (key >> 8) & 0xff;
    codes[1] = key & 0xff;

    for (i = t = 0; i < 2; i++) {
        if (nodes[t].kids == 0) {
            n = getnode(codes[i]);
            nodes[t].kids = n;
            t = n;
        } else if (nodes[nodes[t].kids].key == codes[i])
          t = nodes[t].kids;
        else if (nodes[nodes[t].kids].key > codes[i]) {
            n = getnode(codes[i]);
            nodes[n].sibs = nodes[t].kids;
            nodes[t].kids = n;
            t = n;
        } else {
            t = nodes[t].kids;
            for (l = t; nodes[t].sibs && nodes[t].key < codes[i]; ) {
                l = t;
                t = nodes[t].sibs;
            }
            if (nodes[t].key < codes[i]) {
                n = getnode(codes[i]);
                nodes[t].sibs = n;
                t = n;
            } else if (nodes[t].key > codes[i]) {
                n = getnode(codes[i]);
                nodes[n].sibs = t;
                nodes[l].sibs = n;
                t = n;
            }
        }
    }

    /*
     * Set the value in the leaf node.
     */
    nodes[t].val = val;
}

/*
 * List used by the routine that parses the map lines.
 */
static list_t list;

/*
 * Routine to parse each line of the mapping file.
 */
static int
add_mapping(unsigned char *line, unsigned long linelen, unsigned long lineno,
            void *client_data)
{
    unsigned short key, val;

    /*
     * Split the line into parts separted by one or more spaces or tabs.
     */
    splitline((unsigned char *) " \t+", line, linelen, &list);

    /*
     * Check to see if the line starts with one of the keywords.
     */
    if (memcmp((char *) list.field[0], "REGISTRY", 8) == 0) {
        /*
         * Collect the XLFD CHARSET_REGISTRY value.
         */
        if (registry != 0)
          free((char *) registry);
        if ((val = strlen((char *) list.field[1])) == 0)
          registry = 0;
        else {
            registry = (char *) malloc(val + 1);
            (void) memcpy(registry, (char *) list.field[1], val + 1);
        }
        return 0;
    }

    if (memcmp((char *) list.field[0], "ENCODING", 8) == 0) {
        /*
         * Collect the XLFD CHARSET_ENCODING value.
         */
        if (encoding != 0)
          free((char *) encoding);
        if ((val = strlen((char *) list.field[1])) == 0)
          encoding = 0;
        else {
            encoding = (char *) malloc(val + 1);
            (void) memcpy(encoding, (char *) list.field[1], val + 1);
        }
        return 0;
    }

    /*
     * Get the second field value as the key (the Unicode value).  Always
     * assume the values are in hex.
     */
    key = my_atous(list.field[1], 0, 16);
    val = my_atous(list.field[0], 0, 16);

    trie_insert(key, val);

    return 0;
}

/********************************************************************
 *
 * API for mapping table support.
 *
 ********************************************************************/

int
otf2bdf_load_map(FILE *in)
{
    unsigned long lineno;

    /*
     * Allocate some nodes initially.
     */
    if (nodes_size == 0) {
        nodes = (node_t *) malloc(sizeof(node_t) << 7);
        nodes_size = 128;
    }

    /*
     * Reset the trie in case more than one gets loaded for some reason.
     */
    if (nodes_size > 0)
      (void) memset((char *) nodes, 0, sizeof(node_t) * nodes_size);
    nodes_used = 1;

    return scanlines(fileno(in), add_mapping, 0, &lineno);
}

/*
 * Routine that deallocates the mapping trie.
 */
void
otf2bdf_free_map(void)
{
    if (registry != 0)
      free((char *) registry);
    if (encoding != 0)
      free((char *) encoding);
    registry = encoding = 0;

    if (list.size > 0)
      free((char *) list.field);
    list.size = list.used = 0;

    if (nodes_size > 0)
      free((char *) nodes);
    nodes_size = nodes_used = 0;
}

/*
 * The routine that actually remaps the code by looking it up in the trie.
 */
int
otf2bdf_remap(unsigned short *code)
{
    unsigned long i, n, t;
    unsigned short c, codes[2];

    /*
     * If no mapping table was loaded, then simply return the code.
     */
    if (nodes_used == 0)
      return 1;

    c = *code;
    codes[0] = (c >> 8) & 0xff;
    codes[1] = c & 0xff;

    for (i = n = 0; i < 2; i++) {
        t = nodes[n].kids;
        if (t == 0)
          return 0;
        for (; nodes[t].sibs && nodes[t].key != codes[i]; t = nodes[t].sibs);
        if (nodes[t].key != codes[i])
          return 0;
        n = t;
    }

    *code = nodes[n].val;
    return 1;
}

void
otf2bdf_remap_charset(char **registry_name, char **encoding_name)
{
    if (registry_name != 0)
      *registry_name = registry;
    if (encoding_name != 0)
      *encoding_name = encoding;
}
