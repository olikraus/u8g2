/*

  co.h

  C Object Library
  (c) 2024 Oliver Kraus
  https://github.com/olikraus/c-object

  CC BY-SA 3.0  https://creativecommons.org/licenses/by-sa/3.0/

  -DCO_USE_ZLIB
    will enable autodetection of .gz compressed input files (this will require
  linking against "-lz")

  co    read/and writeable c-object
  cco   read only c-object

  Warning like "discards ‘const’ qualifier" indicate ERRORs!
  However the above warnings are not always generated, this means
    - if the warning is generated, then create a clone
    - if the warning is not generated, then there still might be an isse (see
  examples below)


  objects assigned to containers are moved, this means, the source obj is
  destroyed (unless otherwise mentioned) objects taken out of a container are
  read only

  Example 1:
    co v = coNewVector();
    coAdd(v, coNewStr(CO_STRDUP, "abc"));  // valid, return value of coNewStr is
  deleted by v

  Example 2:
    co v = coNewVector();
    co s = coNewStr(CO_STRDUP, "abc");
    coAdd(v, s);               // valid, but s is invalid after this statement
    coAdd(v, s);                // invalid, because s is already illegal

  Example 3:
    co v = coNewVector();
    co s = coNewStr(CO_STRDUP, "abc");
    coAdd(v, s);               // valid, but s is invalid after this statement
    coDelete(s);                // invalid, because s will be deleted by v again

  Example 4:
    co v = coNewVector();
    co s;
    for( i = 0; i < 10; i++ )
    {
      s = coNewStr(CO_STRDUP, "abc");  // valid, s is illegal and will be
  overwritten coAdd(v, s);               // valid, but s is invalid after this
  statement
    }

  Example 5:
    coAdd(v, coGetByIdx(v, 0)); // invalid & will generate a warning.. use clone
  instead



  Hint:
  gcc -Wall -fsanitize=address -fsanitize=undefined -fsanitize=leak *.c
  gcc -Wall -fsanitize=address -I./co ./co/co.c ./test/co_test.c && ./a.out

  Copy operation from somewhere into a container
    1) Move: The remote element becoMapes obsolete and becoMapes part of the
  container The moved element will be deleted together with the container The
  moved element must not be referenced any more somewhere else 2) Reference: The
  element in the container is just a reference to the remote element. The
  element of the container is not deleted. Risk: If the remote element becoMapes
  deleted, then the element in the container is also invalid 3) Clone: Usually a
  manual operation, where the element is cloned and then stored in the
  container. Makes only sense with option 1

    Vector: isElementDelete --> 0 / 1
    Map: isKeyDelete --> 0 / 1, isValueDelete --> 0 / 1

    Vector
      isElementDelete == true
        add( new() ) --> ok
        add( clone( new() ) --> wrong, memory leak
        add( container_get() ) --> wrong, double delete
        add ( clone( container_get() ) ) -> ok
      isElementDelete == false
        add( new() ) --> wrong, memory leak
        add( clone( new() ) ) --> wrong, 2x memory leak
        add( container_get() ) --> ok
        add( clone( container_get() ) ) -> memory leak



*/

#ifndef CO_INCLUDE
#define CO_INCLUDE

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#ifdef CO_USE_ZLIB
#include "zlib.h"
#endif /* CO_USE_ZLIB */
/*
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#endif
*/
typedef struct coStruct *co;
typedef const struct coStruct *cco;
typedef struct coFnStruct *coFn;

typedef int (*coInitFn)(co o, void *data);
typedef long (*coSizeFn)(cco o);
typedef void (*coPrintFn)(const cco o);
typedef void (*coDestroyFn)(co o);
typedef co (*coCloneFn)(cco o);

struct co_avl_node_struct // structure to build the AVL tree for the "map"
                          // object
{
  const char *key;
  void *value;
  struct co_avl_node_struct *kid[2];
  int height;
};

#define CO_NONE 0
#define CO_FREE_VALS 1
#define CO_FREE_FIRST 2

/* used by str and map objects
  CO_STRDUP: execute a strdup on the string or key
    If CO_STRDUP is used, then  also CO_STRFREE will be enabled
  CO_STRFREE: execute free on the string or key
*/
#define CO_STRDUP 4
#define CO_STRFREE 8

struct coStruct {
  coFn fn;
  unsigned flags; // see above, e.g. CO_NONE, CO_FREE_VALS, etc...
  union {
    struct // vector
    {
      cco *list;
      size_t cnt;
      size_t max;
    } v;
    struct // map
    {
      struct co_avl_node_struct *root;
    } m;
    struct // string and memory block
    {
      char *str;
      size_t len; // current str/mem length
      size_t memlen; // allocated memory (not used for strings)
    } s;
    struct // double
    {
      double n;
    } d;
  };
};

struct coFnStruct {
  coInitFn init; // (*coInitFn)(co o);
  coSizeFn size;
  coPrintFn print;
  coDestroyFn destroy; // counterpart to init
  coCloneFn clone;
};

/* object types */

extern coFn coBlankType;
extern coFn coVectorType;
extern coFn coStrType;
extern coFn coMemType;
extern coFn coMapType;
extern coFn coDblType;

/* object construction */

co coNewBlank();
co coNewStr(unsigned flags,
            const char *s); // most often CO_STRDUP should be used
co coNewDbl(double n);
co coNewMem(void);
co coNewVector(unsigned flags);
co coNewVectorByMap(
    cco map); // constructs a vector from a map, elements of the vector is again
              // a vector with two elements, the key and the value
co coNewMap(unsigned flags);

/* object type test procedures */

#define coGetType(o) ((o == NULL) ? 0 : ((o)->fn))

#define coIsVector(o) (coGetType(o) == coVectorType)
#define coIsStr(o) (coGetType(o) == coStrType)
#define coIsMem(o) (coGetType(o) == coMemType)
#define coIsMap(o) (coGetType(o) == coMapType)
#define coIsDbl(o) (coGetType(o) == coDblType)

/* generic object functions */

void coPrint(const cco o); // debug output of the provided object "o"
void coDelete(
    co o); // deletes "o" and the childs objects of "o" depending on the flags
co coClone(cco o); // do a deep copy of the object "o"
long coSize(cco o);

/* JSON read/write */

co coReadJSONByString(const char *json);
co coReadJSONByFP(FILE *fp); // supports UTF-8 BOM and detects GZIP (if
                             // CO_USE_ZLIB is enabled)
void coWriteJSON(cco o, int isCompact, int isUTF8,
                 FILE *fp); // isUTF8 is 0, then output char codes >=128 via \u

/* string functions */
int coStrAdd(co o, const char *s); // concats the given string to the string
                                   // object, requires the CO_STRDUP flag
char *coStrDeleteAndGetAllocatedStringContent(
    co o); // convert a str obj to a string, return value must be free'd
const char *coStrToString(cco o); /* obsolete, use coStrGet() */
const char *coStrGet(cco o); /* return the internal string as a reference */

/* memory functions */
long coMemSize(cco o);
int coMemAdd(co o, const void *mem, size_t len);
const void *coMemGet(cco o);

/* double functions */
double coDblGet(cco o);

/* vector functions */
long coVectorAdd(
    co o, cco p); // add object at the end of the list, returns -1 for error
int coVectorAppendVector(co v, cco src); // append elements from src to vector v
cco coVectorGet(cco o,
                long idx); // return object at specific position from the vector
void coVectorSet(co v, long i, cco e); // replace an element within the vector
void coVectorErase(
    co v, long i); // delete and remove element at the specified position
void coVectorClear(co o); // first if flags are not CO_NONE delete all elements
                          // and second clear the array to size 0
int coVectorEmpty(cco o); // return 1 if the vector is empty, return 0 otherwise
long coVectorSize(cco o); // return the number of elements in the vector

typedef int (*coVectorForEachCB)(cco o, long idx, cco element, void *data);
int coVectorForEach(cco o, coVectorForEachCB cb, void *data);

typedef co (*coVectorMapCB)(cco o, long idx, cco element, void *data);
co coVectorMap(cco o, coVectorMapCB cb, void *data);

long coVectorPredecessorBinarySearch(
    cco v,
    const char
        *search_key); // assumes structre as returned by "coNewVectorByMap()"

/* map functions */
// int coMapAdd(co o, const char *key, cco value);    // insert object into the
// map, returns 0 for memory error
const char *
coMapAdd(co o, const char *key,
         cco value); // insert object into map, returns NULL for memory error,
                     // otherwise the internal pointer to key
// note: if key exists, then the internal pointer to the same key string is
// returned, otherwise the key argument is returned

cco coMapAddValueKey(
    co o, const char *key); // add a new entry to the map with key and also the
                            // value beeing the same key, CO_FREE_VALS must be
                            // enable for the map. The value object is returned.

int coMapExists(cco o, const char *key); // return 1 if "key" exists in the map
cco coMapGet(cco o, const char *key); // get object from map by key, returns
                                      // NULL if key doesn't exist in the map
const char *
coMapGetKey(cco o, const char *key); // returns the internal pointer to the key
                                     // string or NULL if the key doesn't exist
void coMapErase(co o, const char *key); // removes object from the map
void coMapClear(co o); // delete all elements and clear the array
int coMapEmpty(cco o); // return 1 if the map is empty, return 0 otherwise
long coMapSize(cco o); // O(n) !!

typedef int (*coMapForEachCB)(cco o, long idx, const char *key, cco value,
                              void *data);
int coMapForEach(
    cco o, coMapForEachCB cb,
    void *data); // returns 0 as soon as the callback function returns 0

/* https://stackoverflow.com/questions/30769383/finding-the-minimum-and-maximum-height-in-a-avl-tree-given-a-number-of-nodes
 */
/* --> floor(1.44*log2(n+2)-.328), with n=2^31 (long), this is 45 */
#define CO_AVL_STACK_MAX_DEPTH 45
struct coMapIteratorStruct {
  struct co_avl_node_struct *node[CO_AVL_STACK_MAX_DEPTH];
  struct co_avl_node_struct *current_node;
  int depth;
};
typedef struct coMapIteratorStruct coMapIterator;
#define coMapLoopKey(iter) ((iter)->current_node->key)
#define coMapLoopValue(iter) ((cco)((iter)->current_node->value))
int coMapLoopFirst(coMapIterator *iter, cco o);
int coMapLoopNext(coMapIterator *iter);

/*
coMapIterator iter;
if ( coMapLoopFirst(&iter, o) )
{
        do {
                coMapLoopKey(&iter);  // return the key of the current key/value
pair (const char *)
                coMapLoopValue(&iter);	// return the value of the current
key/value pair (cco) } while( coMapLoopNext(&iter) );
}
*/

/* file / string reader interface */

#define BOM_NONE 0
#define BOM_UTF8 1
#define BOM_UTF16BE 2
#define BOM_UTF16LE 3
#define BOM_UTF32BE 4
#define BOM_UTF32LE 5

typedef struct co_reader_struct *coReader;
typedef void (*coReaderNextFn)(coReader r);

struct co_reader_struct {
  int curr;
  int bom; // see constants above
  const char *reader_string;
  FILE *fp;
  coReaderNextFn next_cb;
  unsigned char stack_memory[16];  // this is normal last in first out stack for UTF-8 sequence
  int stack_pos;		// stack size, 0 means, stack is empty
#ifdef CO_USE_ZLIB
#define CHUNK (16 * 1024)
  unsigned have;
  unsigned pos;
  z_stream strm;
  unsigned char in[CHUNK];
  unsigned char out[CHUNK];
#endif /* CO_USE_ZLIB */
};

int coReaderInitByString(coReader reader, const char *s);
int coReaderInitByFP(coReader reader, FILE *fp);
void coReaderErr(coReader r, const char *msg);

#define coReaderNext(r) ((r)->next_cb(r))
#define coReaderCurr(r) ((r)->curr)

#define coReaderSkipWhiteSpace(r)                                              \
  for (;;) {                                                                   \
    if (coReaderCurr(r) < 0)                                                   \
      break;                                                                   \
    if (coReaderCurr(r) > ' ')                                                 \
      break;                                                                   \
    coReaderNext(r);                                                           \
  }

/* functions from co_extra.c */
co coReadA2LByString(const char *json);
co coReadA2LByFP(FILE *fp);
co coReadS19ByFP(FILE *fp); // returns map, key=8digit addres, value=mem block
co coReadHEXByFP(FILE *fp); // returns map, key=8digit addres, value=mem block
co coReadElfMemoryByFP(FILE *fp); // returns map, key=8digit addres, value=mem
                                  // block, .gz is NOT supported
co coReadCSVByFP(
    FILE *fp, int separator); // returns vector, separator should be ',' or ';'
co coReadCSVByFPWithPool(
    FILE *fp, int separator,
    co pool); // same as coReadCSVByFP, but will allocate all strings in the
              // pool, which must be a map with CO_FREE_VALS flag, pool can be
              // NULL
co coGetCSVRow(struct co_reader_struct *r, int separator);

#endif /* CO_INCLUDE */
