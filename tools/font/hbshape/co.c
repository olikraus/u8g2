/*

  WARNING: THIS FILE IS MODIFIED FOR U8G2 (BOM DETECTION REMOVED)

  co.c

  C Object Library
  (c) 2024 Oliver Kraus

  https://github.com/olikraus/c-object

  CC BY-SA 3.0  https://creativecommons.org/licenses/by-sa/3.0/

  -DCO_USE_ZLIB
    will enable autodetection of .gz compressed input files (this will require
  linking against "-lz")

  28 apr 2024:  fixed coMapClone(): Removed strdup(key) because strdup() is
  already applied internally

*/
#include "co.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*===================================================================*/
/* Generic Public Functions */
/*===================================================================*/

/* print the object, only used for debugging, does not print any \n */
void coPrint(cco o) {
  if (o != NULL)
    o->fn->print(o);
}

/* Delete the object and all child objects, this will also handle o==NULL */
void coDelete(co o) {
  if (o != NULL) {
    o->fn->destroy(o);
    free(o);
  }
}

/* Do a deep clone of the object and return the new objects. Flags are set to
 * CO_NONE for the new object tree. */
co coClone(cco o) {
  if (o == NULL)
    return NULL;
  return o->fn->clone(o);
}

long coSize(cco o) {
  if (o == NULL)
    return 0L;
  return o->fn->size(o);
}

/*===================================================================*/
/* Local Helper Functions */
/*===================================================================*/

static co coNewWithData(coFn t, unsigned flags, void *data) {
  co o = (co)malloc(sizeof(struct coStruct));
  if (o == NULL)
    return NULL;
  o->fn = t;
  o->flags = flags;
  if (t->init(o, data) == 0)
    return free(o), NULL;
  return o;
}

static co coNew(coFn t, unsigned flags) {
  return coNewWithData(t, flags, NULL);
}

/*===================================================================*/
/* Dummy / Blank (probably obsolete) */
/*===================================================================*/

int cobInit(co o, void *data);
long cobSize(cco o);
void cobPrint(cco o);
void cobDestroy(co o);
co cobClone(cco o);

struct coFnStruct cobStruct = {cobInit, cobSize, cobPrint, cobDestroy,
                               cobClone};
coFn coBlankType = &cobStruct;

co coNewBlank() { return coNew(coBlankType, 0); }

int cobInit(co o, void *data) {
  o->fn = coBlankType;
  return 1;
}

long cobSize(cco o) { return 0; }

void cobPrint(cco o) {}

void cobDestroy(co o) {}

co cobClone(cco o) { return coNew(o->fn, o->flags); }

/*===================================================================*/
/* Vector / Arry Object */
/*===================================================================*/

int coVectorInit(co o, void *data); // data: not used
long coVectorAdd(co o, cco p); // returns the index of the added element or -1
long coVectorSize(cco o);
cco coVectorGet(cco o, long idx);
int coVectorForEach(cco o, coVectorForEachCB cb, void *data);
void coVectorPrint(cco o);
static void coVectorDestroy(co o);
co coVectorMap(cco o, coVectorMapCB cb, void *data);
co coVectorClone(cco o);

struct coFnStruct coVectorStruct = {coVectorInit, coVectorSize, coVectorPrint,
                                    coVectorDestroy, coVectorClone};
coFn coVectorType = &coVectorStruct;

co coNewVector(unsigned flags) { return coNew(coVectorType, flags); }

#define COV_EXTEND 32
int coVectorInit(co o, void *data) {
  void *ptr = malloc(COV_EXTEND * sizeof(struct coStruct));
  if (ptr == NULL)
    return 0;
  o->fn = coVectorType;
  o->v.list = (cco *)ptr;
  o->v.max = COV_EXTEND;
  o->v.cnt = 0;
  return 1;
}

/*
  p will be moved, so maybe a clone is required
  returns -1 in case of memory error
  otherwise it returns the position where the element was added
*/
long coVectorAdd(co o, cco p) {
  void *ptr;
  assert(coIsVector(o));
  while (o->v.max <= o->v.cnt) {
    ptr = realloc(o->v.list, (o->v.cnt + COV_EXTEND) * sizeof(co));
    if (ptr == NULL)
      return -1;
    o->v.list = (cco *)ptr;
    o->v.max += COV_EXTEND;
  }
  o->v.list[o->v.cnt] = p;
  o->v.cnt++;
  return o->v.cnt - 1;
}

long coVectorSize(cco o) {
  if (o == NULL)
    return 0;
  assert(coIsVector(o));
  return o->v.cnt;
}

cco coVectorGet(cco o, long idx) {
  if (o == NULL)
    return NULL;
  assert(coIsVector(o));
  if (idx < 0)
    return NULL;
  if (idx >= o->v.cnt)
    return NULL;
  return o->v.list[idx];
}

/* returns 1 (success) or 0 (no success) */
int coVectorForEach(cco o, coVectorForEachCB cb, void *data) {
  long i;
  long cnt = o->v.cnt; // not sure what todo if v.cnt is modified...
  if (o == NULL)
    return 0;
  assert(coIsVector(o));
  for (i = 0; i < cnt; i++)
    if (cb(o, i, o->v.list[i], data) == 0)
      return 0;
  return 1;
}

static int coVectorPrintCB(cco o, long i, cco e, void *data) {
  if (i > 0)
    printf(", ");
  e->fn->print(e);
  return 1;
}
void coVectorPrint(cco o) {
  assert(coIsVector(o));
  printf("[");
  coVectorForEach(o, coVectorPrintCB, NULL);
  printf("]");
}

static int coVectorDestroyCB(cco o, long i, cco e, void *data) {
  coDelete((co)e); // it is ok to discard the const keyword, really delete here
  return 1;
}

static void coVectorDestroy(co o) {
  assert(coIsVector(o));
  if (o->flags & CO_FREE_VALS)
    coVectorForEach(o, coVectorDestroyCB, NULL);
  else if ((o->flags & CO_FREE_FIRST) != 0 && o->v.cnt > 0)
    coDelete((co)o->v.list[0]);
  free(o->v.list);
  o->v.list = NULL;
  o->v.max = 0;
  o->v.cnt = 0;
}

void coVectorClear(co o) {
  assert(coIsVector(o));
  if (o->flags & CO_FREE_VALS)
    coVectorForEach(o, coVectorDestroyCB, NULL);
  else if ((o->flags & CO_FREE_FIRST) != 0 && o->v.cnt > 0)
    coDelete((co)o->v.list[0]);
  o->v.cnt = 0;
}

int coVectorEmpty(cco o) {
  assert(coIsVector(o));
  if (o->v.cnt == 0)
    return 1;
  return 0;
}

co coVectorMap(cco o, coVectorMapCB cb, void *data) {
  co v = coNewVector(CO_FREE_VALS);
  co e;
  long i;
  assert(coIsVector(o));
  for (i = 0; i < o->v.cnt; i++) {
    e = cb(o, i, o->v.list[i], data);
    if (e == NULL) // memory error?
    {
      coVectorDestroy(v);
      return NULL;
    }

    if (coVectorAdd(v, e) < 0) // memory error?
    {
      coVectorDestroy(v);
      return NULL;
    }
  }
  return v;
}

co coVectorCloneCB(cco o, long i, cco e, void *data) { return coClone(e); }

co coVectorClone(cco o) { return coVectorMap(o, coVectorCloneCB, NULL); }

/*===================================================================*/
/* special vector functions */
/*===================================================================*/

/*
        Replace element inside a vector.
        The existing element is free'd according to the flags.
        The same will be true with the new element 'e'.
        The new element 'e' can be the NULL pointer.
        'i' must be a valid entry into the vector
*/
void coVectorSet(co v, long i, cco e) {
  assert(coIsVector(v));
  assert(i < v->v.cnt);
  assert(i >= 0);

  if (v->flags & CO_FREE_VALS) {
    coDelete((co)(v->v.list[i])); // delete the element, NULL is handled within
                                  // coDelete
  } else if ((v->flags & CO_FREE_FIRST) != 0 &&
             i == 0) // consider the case, where only the first element needs to
                     // be deleted
  {
    coDelete((co)v->v.list[0]);
  }

  v->v.list[i] = e;
}

/* delete an element in the vector. Size is reduced  by 1 */
void coVectorErase(co v, long i) {
  if (i >= v->v.cnt)
    return; // do nothing, index is outside the vecor
  // note: v->cnt > 0 at this point
  if (v->flags & CO_FREE_VALS) {
    coDelete((co)(v->v.list[i])); // delete the element
  } else if ((v->flags & CO_FREE_FIRST) != 0 &&
             i == 0) // consider the case, where only the first element needs to
                     // be deleted
  {
    coDelete((co)v->v.list[0]);
    v->flags &= ~CO_FREE_FIRST; // reset the FRE_FIRST flag, because the other
                                // elements must not be deleted
  }
  i++;
  while (i < v->v.cnt)
    v->v.list[i - 1] = v->v.list[i];
  v->v.cnt--;
}

static int coVectorAppendVectorCB(cco o, long idx, cco element, void *data) {
  co c = coClone(element);
  if (c == NULL)
    return 0;
  if (coVectorAdd((co)data, c) < 0)
    return 0;
  return 1;
}

/*
  append elements from src to v
  src can be vector or map
  elements from src are cloned
*/
int coVectorAppendVector(co v, cco src) {
  if (v->fn == coVectorType) {
    long oldCnt = v->v.cnt;

    if (src->fn == coVectorType) {
      if (coVectorForEach(src, coVectorAppendVectorCB, v) == 0) {
        long i = v->v.cnt;
        while (i > oldCnt) {
          i--;
          coVectorErase(v, i);
        }
      }
      return 1;
    }
  }
  return 0; // first / seconad arg is NOT a vector
}

/*===================================================================*/
/* String */
/*===================================================================*/

int coStrInit(co o, void *data); // optional data: const char *
long coStrSize(cco o);
void coStrPrint(cco o);
void coStrDestroy(co o);
co coStrClone(cco o);

struct coFnStruct coStrStruct = {coStrInit, coStrSize, coStrPrint, coStrDestroy,
                                 coStrClone};
coFn coStrType = &coStrStruct;

co coNewStr(unsigned flags, const char *s) {
  co o = coNewWithData(coStrType, flags, (void *)s);
  if (o == NULL)
    return NULL;
  return o;
}

int coStrInit(co o, void *data) {
  static char empty_string[2] = "";
  char *s = (char *)data;
  o->fn = coStrType;
  if (s == NULL)
    s = empty_string;
  if (o->flags & CO_STRDUP) {
    o->s.str = strdup(s);
    o->flags |= CO_STRFREE;
  } else
    o->s.str = s;
  o->s.len = strlen(o->s.str);
  o->s.memlen = 0; // not used for string

  return 1;
}

int coStrAdd(co o, const char *s) {
  assert(coIsStr(o));
  assert(o->s.str != NULL);
  if (o->flags & CO_STRDUP) {
    size_t len = strlen(s);
    char *p = (char *)realloc(o->s.str, o->s.len + len + 1);
    if (p == NULL)
      return 0;
    o->s.str = p;
    strcpy(o->s.str + o->s.len, s);
    o->s.len += len;
    return 1;
  }
  return 0; // static string, can not add another string
}

long coStrSize(cco o) { return (long)o->s.len; }

const char *coStrToString(cco o) {
  assert(coIsStr(o));
  return o->s.str;
}

const char *coStrGet(cco o) {
  if (o == NULL)
    return "";
  assert(coIsStr(o));
  return o->s.str;
}

void coStrPrint(cco o) { printf("%s", o->s.str); }

void coStrDestroy(co o) {
  if (o->flags & CO_STRFREE)
    free(o->s.str);
  o->s.str = NULL;
}

co coStrClone(cco o) {
  return coNewStr(o->flags | CO_STRDUP | CO_STRFREE, o->s.str);
}

/*
  Close the string object and return a string pointer with the content.
  The return value points to allocated memory and must be freed
*/
char *coStrDeleteAndGetAllocatedStringContent(co o) {
  assert(coIsStr(o));
  char *s;
  if (o->flags & CO_STRFREE)
    s = o->s.str;
  else
    s = strdup(o->s.str);
  o->flags &=
      ~CO_STRFREE; // clear the strfree flag so that the memory is not released
  coDelete(o); // close the object itself
  return s; // and finally return the allocated string
}

/*===================================================================*/
/* Binary Memory Block */
/*===================================================================*/

int coMemInit(co o, void *data); // optional data: const char *
long coMemSize(cco o);
void coMemPrint(cco o);
void coMemDestroy(co o);
co coMemClone(cco o);

struct coFnStruct coMemStruct = {coMemInit, coMemSize, coMemPrint, coMemDestroy,
                                 coMemClone};
coFn coMemType = &coMemStruct;

co coNewMem(void) {
  co o = coNewWithData(coMemType, 0, NULL);
  if (o == NULL)
    return NULL;
  return o;
}

int coMemInit(co o, void *data) {
  o->fn = coMemType;
  o->s.str = NULL;
  o->s.len = 0;
  o->s.memlen = 0;
  return 1;
}

#define CO_MEM_MIN_EXTEND (16 * 1024)
#define CO_MEM_MAX_EXTEND (1024 * 1024)
int coMemAdd(co o, const void *mem, size_t len) {
  void *m;
  size_t extend;

  extend = o->s.memlen; // double the memory...
  if (extend < CO_MEM_MIN_EXTEND)
    extend = CO_MEM_MIN_EXTEND;
  if (extend >
      CO_MEM_MAX_EXTEND) // ... but only the max extend value is reached
    extend = CO_MEM_MAX_EXTEND;
  if (extend < len)
    extend = len;

  assert(coIsMem(o));
  if (mem == NULL || len == 0)
    return 1;

  if (o->s.str == NULL) {
    o->s.memlen = extend;
    m = malloc(o->s.memlen);
    o->s.len = 0;
  } else if (o->s.len + len > o->s.memlen) {
    o->s.memlen = o->s.len + extend;
    m = realloc(o->s.str, o->s.memlen);
  } else {
    m = o->s.str;
  }

  if (m == NULL)
    return 0;

  memcpy(m + o->s.len, mem, len);
  o->s.str = (char *)m;
  o->s.len += len;
  return 1;
}

const void *coMemGet(cco o) { return (const void *)o->s.str; }

long coMemSize(cco o) {
  assert(coIsMem(o));
  return (long)o->s.len;
}

void coMemPrint(cco o) { printf("<%zd bytes>", o->s.len); }

void coMemDestroy(co o) {
  if (o->s.str != NULL)
    free(o->s.str);
  o->s.str = NULL;
}

co coMemClone(cco o) {
  co new_o;
  assert(coIsMem(o));
  new_o = coNewMem();
  if (new_o == NULL)
    return NULL;
  if (coMemAdd(new_o, o->s.str, o->s.len) != 0)
    return new_o;
  coMemDestroy(new_o);
  return NULL;
}

/*===================================================================*/
/* Double */
/*===================================================================*/

int coDblInit(co o, void *data); // optional data: const double *
long coDblSize(cco o);
void coDblPrint(cco o);
void coDblDestroy(co o);
co coDblClone(cco o);

struct coFnStruct coDblStruct = {coDblInit, coDblSize, coDblPrint, coDblDestroy,
                                 coDblClone};
coFn coDblType = &coDblStruct;

co coNewDbl(double n) {
  co o = coNewWithData(coDblType, CO_NONE, (void *)&n);
  if (o == NULL)
    return NULL;
  return o;
}

int coDblInit(co o, void *data) {
  o->fn = coDblType;
  if (data == NULL)
    o->d.n = 0.0;
  else
    o->d.n = *(double *)data;
  return 1;
}

long coDblSize(cco o) { return 1; }

void coDblPrint(cco o) { printf("%.9g", o->d.n); }

void coDblDestroy(co o) { o->d.n = 0.0; }

co coDblClone(cco o) { return coNewDbl(o->d.n); }

double coDblGet(cco o) {
  if (o == NULL)
    return 0.0;
  assert(coIsDbl(o));
  return o->d.n;
}

/*===================================================================*/
/* Map */
/* based on https://rosettacode.org/wiki/AVL_tree/C */
/*===================================================================*/

static struct co_avl_node_struct avl_dummy = {
    NULL, NULL, {&avl_dummy, &avl_dummy}, 0};
static struct co_avl_node_struct *avl_nnil =
    &avl_dummy; // internally, avl_nnil is the new nul

typedef void (*avl_free_fn)(void *p);

static void avl_free_value(void *value) { coDelete((co)value); }

static void avl_keep_value(void *value) {}

static void avl_free_key(void *key) { free(key); }

static void avl_keep_key(void *key) {}

static struct co_avl_node_struct *avl_new_node(const char *key, void *value) {
  struct co_avl_node_struct *n = malloc(sizeof(struct co_avl_node_struct));
  if (n == NULL)
    return avl_nnil;
  n->key = key;
  n->value = value;
  n->height = 1;
  n->kid[0] = avl_nnil;
  n->kid[1] = avl_nnil;
  return n;
}

static void avl_delete_node(struct co_avl_node_struct *n, avl_free_fn free_key,
                            avl_free_fn free_value) {
  free_key((void *)(n->key));
  if (n->value != NULL)
    free_value(n->value);
  n->value = NULL;
  n->kid[0] = NULL;
  n->kid[1] = NULL;
  free(n);
}

static int avl_max(int a, int b) { return a > b ? a : b; }

static void avl_set_height(struct co_avl_node_struct *n) {
  n->height = 1 + avl_max(n->kid[0]->height, n->kid[1]->height);
}

static int avl_get_ballance_diff(struct co_avl_node_struct *n) {
  return n->kid[0]->height - n->kid[1]->height;
}

// rotate a subtree according to dir; if new root is nil, old root is freed
static struct co_avl_node_struct *avl_rotate(struct co_avl_node_struct **rootp,
                                             int dir, avl_free_fn free_key,
                                             avl_free_fn free_value) {
  struct co_avl_node_struct *old_r = *rootp;
  struct co_avl_node_struct *new_r = old_r->kid[dir];

  *rootp = new_r; // replace root with the selected child

  if (avl_nnil == *rootp) {
    avl_delete_node(old_r, free_key, free_value);
  } else {
    old_r->kid[dir] = new_r->kid[!dir];
    avl_set_height(old_r);
    new_r->kid[!dir] = old_r;
  }
  return new_r;
}

static void avl_adjust_balance(struct co_avl_node_struct **rootp,
                               avl_free_fn free_key, avl_free_fn free_value) {
  struct co_avl_node_struct *root = *rootp;
  int b = avl_get_ballance_diff(root) / 2;
  if (b != 0) {
    int dir = (1 - b) / 2;
    if (avl_get_ballance_diff(root->kid[dir]) == -b) {
      avl_rotate(&root->kid[dir], !dir, free_key, free_value);
    }
    root = avl_rotate(rootp, dir, free_key, free_value);
  }
  if (root != avl_nnil)
    avl_set_height(root);
}

// find the node that contains the given key; or returns NULL (which is a little
// bit inconsistent)
struct co_avl_node_struct *avl_query(struct co_avl_node_struct *root,
                                     const char *key) {
  int c;
  if (key == NULL)
    return NULL;

  if (root == avl_nnil)
    return NULL;

  c = strcmp(key, root->key);
  if (c == 0)
    return root;
  return avl_query(root->kid[c > 0], key);
}

/* returns NULL for any allocation error, otherwise the pointer to the internal
 * key  */
/* the return value is identical to the key argument if the key doesn't yet
 * exist otherwise it is a pointer to the existing key in the tree */
static const char *avl_insert(struct co_avl_node_struct **rootp,
                              const char *key, void *value,
                              avl_free_fn free_key, avl_free_fn free_value) {
  struct co_avl_node_struct *root = *rootp;
  int c;

  if (key == NULL)
    return NULL; // illegal key

  if (root == avl_nnil) {
    *rootp = avl_new_node(key, value);
    if (*rootp == avl_nnil) // memory error
      return NULL;
    return key;
  }

  c = strcmp(key, root->key);
  if (c == 0) {
    // key already exists: replace value
    free_key((void *)key);
    if (root->value != NULL)
      free_value(root->value);
    root->value = value;
    key = root->key; // return the internal key
  } else {
    key = avl_insert(&root->kid[c > 0], key, value, free_key, free_value);
    if (key == NULL)
      return NULL;
    avl_adjust_balance(rootp, free_key, free_value);
  }
  return key;
}

static void avl_delete(struct co_avl_node_struct **rootp, const char *key,
                       avl_free_fn free_key, avl_free_fn free_value) {
  struct co_avl_node_struct *root = *rootp;
  if (key == NULL)
    return; // illegal key
  if (root == avl_nnil)
    return; // not found

  // if this is the node we want, rotate until off the tree
  if (strcmp(key, root->key) == 0) {
    root = avl_rotate(rootp, avl_get_ballance_diff(root) < 0, free_key,
                      free_value);
    if (avl_nnil == root) {
      return;
    }
  }
  avl_delete(&root->kid[strcmp(key, root->key) > 0], key, free_key, free_value);
  avl_adjust_balance(rootp, free_key, free_value);
}

static int avl_for_each(cco o, struct co_avl_node_struct *n,
                        coMapForEachCB visitCB, long *idx, void *data) {
  if (n == avl_nnil)
    return 1;
  avl_for_each(o, n->kid[0], visitCB, idx, data);
  if (visitCB(o, *idx, n->key, (cco)(n->value), data) == 0)
    return 0;
  (*idx)++;
  avl_for_each(o, n->kid[1], visitCB, idx, data);
  return 1;
}

/*
  after calling avl_delete_all the "n" argument is illegal and points to
  avl_nnil
*/
static void avl_delete_all(struct co_avl_node_struct **n, avl_free_fn free_key,
                           avl_free_fn free_value) {
  if (*n == avl_nnil)
    return;
  avl_delete_all(&((*n)->kid[0]), free_key, free_value);
  avl_delete_all(&((*n)->kid[1]), free_key, free_value);
  avl_delete_node(*n, free_key, free_value);
  *n = avl_nnil;
}

static int avl_get_size_cb(cco o, long idx, const char *key, cco value,
                           void *data) {
  return 1;
}

/* warning: this has O(n) runtime */
static long avl_get_size(struct co_avl_node_struct *n) {
  long cnt = 0;
  avl_for_each(NULL, n, avl_get_size_cb, &cnt, NULL);
  return cnt;
}

int coMapInit(co o, void *data);
long coMapSize(cco o);
void coMapPrint(cco o);
void coMapClear(co o);
int coMapEmpty(cco o);
co coMapClone(cco o);

struct coFnStruct coMapStruct = {
    coMapInit, coMapSize, coMapPrint,
    coMapClear, // destroy function is identical to clear function
    coMapClone};
coFn coMapType = &coMapStruct;

co coNewMap(unsigned flags) { return coNew(coMapType, flags); }

int coMapInit(co o, void *data) {
  assert(coIsMap(o));
  o->m.root = avl_nnil;

  if (o->flags & CO_STRDUP)
    o->flags |= CO_STRFREE;

  return 1;
}

const char *coMapAdd(co o, const char *key, cco value) {
  const char *k;
  assert(coIsMap(o));
  assert(key != NULL);
  if (o->flags & CO_STRDUP)
    k = strdup(key);
  else
    k = key;
  if (k == NULL)
    return 0;

  return avl_insert(&(o->m.root), k, (void *)value,
                    (o->flags & CO_STRFREE) ? avl_free_key : avl_keep_key,
                    (o->flags & CO_FREE_VALS) ? avl_free_value
                                              : avl_keep_value);
}

// Add a key and also store the key as a value
// returns the value (string object) if successful
// the provided map must have CO_FREE_VALS enabled
// Whether CO_STRDUP is enabled for the map, doesn't matter and will be taken
// care the return value belongs to the map and must not be free'd somewhere
// else
cco coMapAddValueKey(co o, const char *key) {
  const char *k;
  cco cv;
  co v;
  assert(coIsMap(o));
  assert(key != NULL);
  assert((o->flags & CO_FREE_VALS) != 0);

  cv = coMapGet(o, key);
  if (cv != NULL)
    return cv; // if the key exists, just return the string object with the key
               // (which is also the value)

  // otherwise create a new entry in the map
  if (o->flags & CO_STRDUP) {
    k = strdup(key);
    v = coNewStr(CO_NONE, k);
    if (avl_insert(&(o->m.root), k, (void *)v,
                   (o->flags & CO_STRFREE) ? avl_free_key : avl_keep_key,
                   (o->flags & CO_FREE_VALS) ? avl_free_value
                                             : avl_keep_value) == NULL)
      return coDelete(v), free((void *)k), NULL;
  } else {
    assert((o->flags & CO_STRFREE) == 0);
    v = coNewStr(CO_STRDUP, key);
    k = coStrGet(v);
    if (avl_insert(&(o->m.root), k, (void *)v,
                   (o->flags & CO_STRFREE) ? avl_free_key : avl_keep_key,
                   (o->flags & CO_FREE_VALS) ? avl_free_value
                                             : avl_keep_value) == NULL)
      return coDelete(v), NULL;
  }

  return v;
}

long coMapSize(cco o) {
  assert(coIsMap(o));
  return avl_get_size(o->m.root); // O(n) !
}

static int avl_co_map_print_cb(cco o, long idx, const char *key, cco value,
                               void *data) {
  if (idx > 0)
    printf(", ");
  printf("%s:", key);
  coPrint(value);
  return 1;
}

void coMapPrint(cco o) {
  long cnt = 0;
  assert(coIsMap(o));
  printf("{");
  avl_for_each(o, o->m.root, avl_co_map_print_cb, &cnt, NULL);
  printf("}");
}

void coMapClear(co o) {
  assert(coIsMap(o));
  avl_delete_all(&(o->m.root),
                 (o->flags & CO_STRFREE) ? avl_free_key : avl_keep_key,
                 (o->flags & CO_FREE_VALS) ? avl_free_value : avl_keep_value);
}

int coMapEmpty(cco o) {
  assert(coIsMap(o));
  if (o->m.root == avl_nnil)
    return 1;
  return 0;
}

static int avl_co_map_clone_cb(cco o, long idx, const char *key, cco value,
                               void *data) {
  // strdup() will be applied to key, because CO_STRDUP is active for the map
  return coMapAdd((co)data, key, coClone((co)(value))) == NULL ? 0 : 1;
}

co coMapClone(cco o) {
  long cnt = 0;
  co new_obj = coNewMap(o->flags | CO_FREE_VALS | CO_STRDUP | CO_STRFREE);
  assert(coIsMap(o));
  if (avl_for_each(o, o->m.root, avl_co_map_clone_cb, &cnt, new_obj) == 0) {
    coDelete(new_obj);
    return NULL;
  }
  return new_obj;
}

int coMapExists(cco o, const char *key) {
  struct co_avl_node_struct *n;
  assert(coIsMap(o));
  n = avl_query(o->m.root, key);
  if (n == NULL)
    return 0;
  return 1;
}

/* return the internal pointer to the key for a given string key, returns NULL
 * if the key doesn't exist */
const char *coMapGetKey(cco o, const char *key) {
  struct co_avl_node_struct *n;
  assert(coIsMap(o));
  n = avl_query(o->m.root, key);
  if (n == NULL)
    return NULL;
  return n->key;
}

/* return the value for a given key */
cco coMapGet(cco o, const char *key) {
  struct co_avl_node_struct *n;
  assert(coIsMap(o));
  n = avl_query(o->m.root, key);
  if (n == NULL)
    return NULL;
  return (cco)(n->value);
}

void coMapErase(co o, const char *key) {
  assert(coIsMap(o));
  avl_delete(&(o->m.root), key,
             (o->flags & CO_STRFREE) ? avl_free_key : avl_keep_key,
             (o->flags & CO_FREE_VALS) ? avl_free_value : avl_keep_value);
}

int coMapForEach(cco o, coMapForEachCB cb, void *data) {
  long cnt = 0;
  assert(coIsMap(o));
  return avl_for_each(o, o->m.root, cb, &cnt, data);
}

static int avl_loop_sub(coMapIterator *iter) {
  while (iter->depth > 0 || iter->current_node != avl_nnil) {
    if (iter->current_node != avl_nnil) {
      iter->node[iter->depth] = iter->current_node;
      iter->current_node = iter->current_node->kid[0];
      iter->depth++;
      assert(iter->depth < CO_AVL_STACK_MAX_DEPTH);
    } else {
      iter->depth--;
      iter->current_node = iter->node[iter->depth];
      return 1; // process current_node
    }
  }
  return 0;
}

int coMapLoopFirst(coMapIterator *iter, cco o) {
  // puts("coMapLoopFirst");
  assert(coIsMap(o));
  iter->depth = 0;
  iter->current_node = o->m.root;
  return avl_loop_sub(iter);
}

int coMapLoopNext(coMapIterator *iter) {
  iter->current_node = iter->current_node->kid[1];
  return avl_loop_sub(iter);
}

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

/*===================================================================*/
/* Publlic Utility Functions */
/*===================================================================*/

static int coVectorByMapCB(cco o, long idx, const char *key, cco value,
                           void *data) {
  co parent = (co)data;
  co key_value_vector =
      coNewVector(CO_FREE_FIRST); // the first element is a string object, which
                                  // must be freed
  assert(coIsMap(o));
  assert(key != NULL);
  if (key_value_vector == NULL)
    return 0;
  if (coVectorAdd(key_value_vector, coNewStr(CO_NONE, key)) <
      0) // do not free the key-string, it belongs to the map
    return coDelete(key_value_vector), 0;
  if (coVectorAdd(key_value_vector, value) <
      0) // do not free the value, it belongs to the map
    return coDelete(key_value_vector), 0;
  if (coVectorAdd(parent, key_value_vector) <
      0) // add the key value vector to the parent vector
    return coDelete(key_value_vector), 0;
  return 1;
}

/*
  Return a vector, where each element is a vector with two elements: Key and
  value from the map: [ [key, value], [key, value], ..., [key, value] ] key and
  value refer to the map, this means, the return value must be deleted before
  the map is deleted.
*/
co coNewVectorByMap(cco map) {
  co vector = coNewVector(
      CO_FREE_VALS); // the inner vectors (key, value pairs) should be free'd
  if (coMapForEach(map, coVectorByMapCB, vector) == 0)
    return coDelete(vector), NULL;
  return vector;
}

/*
  Search for index position within a sorted vector.
  Assumes a vector with key/value pairs: [ [key, value], [key, value], ...,
  [key, value] ] (=return value from coNewVectorByMap()) The vector must be
  sorted according to "key". Returns an index for which the key is lower or
  equal to the provided "search_key" argument. The returned index is the
  predecessor for the given "search_key": The position of the element, which is
  lower or equal to "search_key". The returned index can be -1 if there is no
  predecessor element. This means "search_key" is smaller than any other element
  in the vector. The returned index is always smaller than coVectorSize() The
  returned index is -1 if the vector is empty

    [] search 1 --> result -1
    [2] search 1 --> result -1
    [2] search 2 --> result 0
    [2] search 3 --> result 0
    [2, 4] search 1 --> result -1
    [2, 4] search 2 --> result 0
    [2, 4] search 3 --> result 0
    [2, 4] search 4 --> result 1
    [2, 4] search 5 --> result 1
    [2, 4, 6] search 1 --> result -1
    [2, 4, 6] search 2 --> result 0
    [2, 4, 6] search 3 --> result 0
    [2, 4, 6] search 4 --> result 1
    [2, 4, 6] search 5 --> result 1
    [2, 4, 6] search 6 --> result 2
    [2, 4, 6] search 7 --> result 2

*/
long coVectorPredecessorBinarySearch(cco v, const char *search_key) {
  long lower_pos = 0;
  long upper_pos = coVectorSize(v);
  long mid_pos;
  const char *mid_str;

  while (lower_pos < upper_pos) {
    mid_pos = (upper_pos + lower_pos) / 2;
    mid_str = coStrGet(coVectorGet(coVectorGet(v, mid_pos), 0));
    assert(mid_str != NULL);
    if (strcmp(mid_str, search_key) > 0) {
      upper_pos = mid_pos;
    } else {
      lower_pos = mid_pos + 1;
    }
  }
  return upper_pos - 1;
}

/*===================================================================*/
/* FILE/String Reader */
/*===================================================================*/

static int bom_read_and_skip(FILE *fp) {
  int c;
  c = fgetc(fp);
  if (c == 0xEF) {
    if (fgetc(fp) == 0xBB)
      if (fgetc(fp) == 0xBF)
        return BOM_UTF8;
    fseek(fp, 0, SEEK_SET);
    return BOM_NONE;
  }
  if (c == 0xFE) {
    if (fgetc(fp) == 0xFF)
      return BOM_UTF16BE;
  } else if (c == 0xFF) {
    if (fgetc(fp) == 0xFE) {
      if (fgetc(fp) != 0x00) {
        fseek(fp, 2, SEEK_SET);
        return BOM_UTF16LE;
      }
      if (fgetc(fp) == 0x00) // second 0x00?
        return BOM_UTF32LE;
    }
  } else if (c == 0x00) {
    if (fgetc(fp) == 0x00)
      if (fgetc(fp) == 0xFE)
        if (fgetc(fp) == 0xFF)
          return BOM_UTF32BE;
  }

  fseek(fp, 0, SEEK_SET);
  return BOM_NONE;
}

void coReaderErr(coReader r, const char *msg) {
  printf("JSON Parser error '%s', current char='%c'\n", msg, r->curr);
  /* removed, because we can't print fpos_t */
  /*
  if ( r->fp != NULL )
  {
        fpos_t pos;
        fgetpos(r->fp, &pos);
        printf("JSON Parser error at file pos %lld\n", (long long int)(pos));
  }
  */
}

static void coReaderStringNext(coReader r) {
  if (r->curr < 0)
    return;
  (r->reader_string)++;
  r->curr = *(r->reader_string);
  if (r->curr == '\0')
    r->curr = -1; // code below will check for <0
}

static void coReaderFileNext(coReader r) {
  if (r->curr < 0)
    return;
  r->curr = getc(
      r->fp); // may return EOF, which is -1, but code below will check for <0
}

/*
	return the size of the UTF8 sequence
	store the OTF8 sequence in reverse order (for the stack in coReader)
*/

static int coEncodeUTF8(uint32_t codepoint, uint8_t *utf8) {
    if (codepoint <= 0x7F) {
        utf8[0] = codepoint & 0x7F;
        return 1;
    } else if (codepoint <= 0x7FF) {
        utf8[1] = 0xC0 | ((codepoint >> 6) & 0x1F);
        utf8[0] = 0x80 | (codepoint & 0x3F);
        return 2;
    } else if (codepoint <= 0xFFFF) {
        utf8[2] = 0xE0 | ((codepoint >> 12) & 0x0F);
        utf8[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8[0] = 0x80 | (codepoint & 0x3F);
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        utf8[3] = 0xF0 | ((codepoint >> 18) & 0x07);
        utf8[2] = 0x80 | ((codepoint >> 12) & 0x3F);
        utf8[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8[0] = 0x80 | (codepoint & 0x3F);
        return 4;
    }
    return -1; // invalid codepoint
}

static uint32_t coGetCodepointFromUTF16(unsigned high, unsigned low) {
    if (high >= 0xD800 && high <= 0xDBFF && low >= 0xDC00 && low <= 0xDFFF) {
        return (uint32_t)0x10000 + (((uint32_t)(high - 0xD800) << 10) | (uint32_t)(low - 0xDC00));
    } 
    return high; // Not a surrogate pair, just return the ascii value
}


static void coReaderFileNext16LE(coReader r) {
  if (r->curr < 0)
    return;
  if ( r->stack_pos > 0 )
  {
	r->stack_pos--;
	r->curr = r->stack_memory[r->stack_pos];
  }
  else
  {
	/*
	r->curr = getc(r->fp); // lower byte comes first
	if (r->curr < 0)
		return;
	r->curr |= (getc(r->fp) << 8); // higher byte
	*/
	int low, high;	  // not sure whether low/high naming makes sense here
	low = getc(r->fp);
	if ( low < 0 )
	{
		r->curr = -1;
		return;
	}
	high = getc(r->fp);
	if ( high < 0 )
	{
		r->curr = -1;
		return;
	}
	r->stack_pos = coEncodeUTF8( coGetCodepointFromUTF16(low, high), r->stack_memory );  // low/high sequence tested
	if ( r->stack_pos < 1)
	{
		r->curr = -1;
		return;
	}
	r->stack_pos--;
	r->curr = r->stack_memory[r->stack_pos];	
  }
}

static void coReaderFileNext16BE(coReader r) {
  if (r->curr < 0)
    return;
  if ( r->stack_pos > 0 )
  {
	r->stack_pos--;
	r->curr = r->stack_memory[r->stack_pos];
  }
  else
  {
	/*
	r->curr = getc(r->fp); // higher byte comes first
	if (r->curr < 0)
		return;
	r->curr <<= 8;
	r->curr |= getc(r->fp); // lower byte
	*/
	int low, high;  // not sure whether low/high naming makes sense here
	low = getc(r->fp);
	if ( low < 0 )
	{
		r->curr = -1;
		return;
	}
	high = getc(r->fp);
	if ( high < 0 )
	{
		r->curr = -1;
		return;
	}
	r->stack_pos = coEncodeUTF8( coGetCodepointFromUTF16(high, low), r->stack_memory );	// bytes are swapped here
	if ( r->stack_pos < 1)
	{
		r->curr = -1;
		return;
	}
	r->stack_pos--;
	r->curr = r->stack_memory[r->stack_pos];	
  }
}

#ifdef CO_USE_ZLIB
static int coReaderGZInit(coReader r) {
  int ret;
  /* allocate inflate state */
  r->have = 0;
  r->pos = 0;
  r->strm.zalloc = Z_NULL;
  r->strm.zfree = Z_NULL;
  r->strm.opaque = Z_NULL;
  r->strm.avail_in = 0;
  r->strm.avail_out = 0;
  r->strm.next_in = r->in;
  r->strm.next_out = r->out;
  ret = inflateInit2(
      &(r->strm),
      32 +
          MAX_WBITS); // https://stackoverflow.com/questions/1838699/how-can-i-decompress-a-gzip-stream-with-zlib
  if (ret != Z_OK)
    return 0;
  return 1;
}

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE STRINGIZE(__LINE__)

static void coReaderGZFileNext(coReader r) {
  /* https://chromium.googlesource.com/native_client/nacl-gcc/+/master/zlib/examples/zpipe.c
   */
  int ret;

  if (r->pos >= r->have) {
    // printf(LINE " GZ: pos=%d have=%d strm.avail_in=%d strm.avail_out=%d\n",
    // r->pos, r->have, r->strm.avail_in, r->strm.avail_out);
    if (r->strm.avail_in ==
        0) // initially avail_in is 0, later we will only execute the if body if
           // further reads are required
    {
      r->strm.avail_in = fread(r->in, 1, CHUNK, r->fp);
      // printf(LINE " GZ: fread strm.avail_in=%d\n", r->strm.avail_in);
      r->strm.next_in = r->in;
      if (ferror(r->fp)) {
        inflateEnd(&(r->strm));
        coReaderErr(r, "File Read Error");
        r->curr = -1;
        return;
      }
      if (r->strm.avail_in == 0) {
        r->curr = -1;
        inflateEnd(&(r->strm));
        return;
      }
    }

    r->strm.avail_out = CHUNK;
    r->strm.next_out = r->out;

    ret = inflate(&(r->strm), Z_NO_FLUSH);
    // printf(LINE " GZ: ret=%d strm.avail_out=%d\n", ret, r->strm.avail_out);
    switch (ret) {
    case Z_NEED_DICT:
      coReaderErr(r, "ZLIB Decompression NEED_DICT Error");
      inflateEnd(&(r->strm));
      r->curr = -1;
      return;
    case Z_DATA_ERROR:
      coReaderErr(r, "ZLIB Decompression DATA Error");
      inflateEnd(&(r->strm));
      r->curr = -1;
      return;
    case Z_MEM_ERROR:
      coReaderErr(r, "ZLIB Decompression MEM Error");
      inflateEnd(&(r->strm));
      r->curr = -1;
      return;
    case Z_BUF_ERROR:
      coReaderErr(
          r, "ZLIB Decompression BUF Error (missing binary mode for fopen?)");
      inflateEnd(&(r->strm));
      r->curr = -1;
      return;
    case Z_STREAM_END:
      if (CHUNK - r->strm.avail_out > 0)
        break;
      // printf(LINE " GZ: STREAM_END\n");
      inflateEnd(&(r->strm));
      r->curr = -1;
      return;
    }
    r->have = CHUNK - r->strm.avail_out;
    r->pos = 0;
    // printf(LINE " GZ: have=%d\n", r->have);
  }

  r->curr = r->out[r->pos];
  r->pos++;
}
#endif /* CO_USE_ZLIB */

int coReaderInitByString(coReader reader, const char *s) {
  if (reader == NULL || s == NULL)
    return 0;
  reader->bom = BOM_NONE;
  reader->reader_string = s;
  reader->fp = NULL;
  reader->next_cb = coReaderStringNext;
  reader->curr = reader->reader_string[0];
  if (reader->curr == '\0')
    reader->curr = -1;
  coReaderSkipWhiteSpace(reader);
  return 1;
}

int coReaderInitByFP(coReader reader, FILE *fp) {
  if (reader == NULL || fp == NULL)
    return 0;
  reader->curr = 32;
  reader->stack_pos = 0;  
  reader->next_cb = coReaderFileNext; // assign some default
  reader->reader_string = NULL;
  reader->fp = fp;
  reader->bom = BOM_UTF8;
  
  /*
  reader->bom = bom_read_and_skip(fp);
  if (reader->bom == BOM_NONE)
    reader->next_cb = coReaderFileNext; // all good, contine with GZ test
  else if (reader->bom == BOM_UTF8)
    reader->next_cb = coReaderFileNext;
  else if (reader->bom == BOM_UTF16LE)
    reader->next_cb = coReaderFileNext16LE;
  else if (reader->bom == BOM_UTF16BE)
    reader->next_cb = coReaderFileNext16BE;
  else
    return 0; // unsupported BOM (like BOM_UTF32BE for example)
  */

#ifdef CO_USE_ZLIB
  if (reader->bom == BOM_NONE) // check for GZIP
  {
    reader->curr = getc(fp);
    if (reader->curr == 0x1f) // GZIP IDs: 0x1f 0x8b, see
                              // https://www.rfc-editor.org/rfc/rfc1952#page-5
    {
      fseek(fp, 0, SEEK_SET);
      coReaderGZInit(reader);
      reader->next_cb = coReaderGZFileNext; // this is a UTF8 BOM reader!
      coReaderNext(reader); // read the first byte

      // detect and skip UTF8 BOM if present
      // Warning: at the moment only UTF8 BOM is supported for gzip
      if (coReaderCurr(reader) == 0xEF) // UTF8 BOM?
      {
        coReaderNext(reader);
        if (coReaderCurr(reader) == 0xBB) {
          coReaderNext(reader);
          if (coReaderCurr(reader) == 0xBF) {
            coReaderNext(reader);
            reader->bom = BOM_UTF8;
          } else {
            reader->pos = 0;
            coReaderNext(reader); // read the first byte
          }
        } else {
          reader->pos = 0;
          coReaderNext(reader); // read the first byte
        }
      }
    }
  }
	// ToDo: is the initial call to coReaderNext() missing in case that this is not GZIP? shouldn't here be al #else also with an initial read???
#endif /* CO_USE_ZLIB */



  coReaderSkipWhiteSpace(reader);
  return 1;
}

/*===================================================================*/
/* JSON Parser */
/*===================================================================*/

co coJSONGetValue(coReader reader); // forward declaration

#define COJ_STR_BUF 1024

const char *coJSONGetIdentifier(coReader reader) {
  static char buf[COJ_STR_BUF + 16]; // extra data for UTF-8 sequence and \0
  size_t idx = 0;
  int c = 0;

  for (;;) {
    c = coReaderCurr(reader);
    if (isalpha(c) == 0)
      break;
    if (idx < COJ_STR_BUF - 1)
      buf[idx++] = c;
    coReaderNext(reader);
  }
  buf[idx] = '\0';
  coReaderSkipWhiteSpace(reader);
  return buf;
}

char *coJSONGetStr(coReader reader) {
  static char buf[COJ_STR_BUF + 16]; // extra data for UTF-8 sequence and \0
  char *s = NULL; // upcoming return value (allocated string)
  size_t len = 0; // len == strlen(s)
  size_t idx = 0;
  int c = 0;
  if (coReaderCurr(reader) != '\"')
    return coReaderErr(reader, "Internal error, double quote missing"), NULL;
  coReaderNext(reader); // skip initial double quote
  for (;;) {
    c = coReaderCurr(reader);
    if (c < 0) // unexpected end of stream
      return coReaderErr(reader, "Unexpected end of string"), NULL;
    if (c == '\"')
      break; // regular end
    if (c == '\\') {
      coReaderNext(reader); // skip back slash
      c = coReaderCurr(reader);
      if (c == 'u') {
        unsigned long u = 0;
        int i;
        for (i = 0; i < 4; i++) // read the \uXXXX hex number
        {
          coReaderNext(reader); // skip 'u' or any of the hex chars
          c = coReaderCurr(reader);
          if (c < 0) {
            return coReaderErr(reader, "Unexpected end within \\uXXXX"),
                   NULL; // todo: we might need to free(s)
          }
          if (c >= '0' && c <= '9') {
            u <<= 4;
            u += c - '0';
            continue;
          }
          if (c >= 'a' && c <= 'f') {
            u <<= 4;
            u += c - 'a' + 10;
            continue;
          }
          if (c >= 'A' && c <= 'F') {
            u <<= 4;
            u += c - 'A' + 10;
            continue;
          }
          // if we reach this point, then the \u arg is not a hex number
          return coReaderErr(reader, "Not a hex number with \\uXXXX"),
                 NULL; // todo: we might need to free(s)
        }
        coReaderNext(reader); // skip last hex char
        // convert the code point to UTF-8
        if (u < 0x80UL) {
          buf[idx++] = u;
        } else if (u < 0x800UL) {
          buf[idx++] = (0xC0 | (u >> 6));
          buf[idx++] = (0x80 | (u & 0x3F));
        } else if (u < 0x10000UL) {
          buf[idx++] = (0xE0 | (u >> 12));
          buf[idx++] = (0x80 | ((u >> 6) & 0x3F));
          buf[idx++] = (0x80 | (u & 0x3F));
        } else if (u < 0x200000UL) // hmmm... we will never arive here
        {
          buf[idx++] = (0xE0 | (u >> 18));
          buf[idx++] = (0x80 | ((u >> 12) & 0x3F));
          buf[idx++] = (0x80 | ((u >> 6) & 0x3F));
          buf[idx++] = (0x80 | (u & 0x3F));
        } else {
          return coReaderErr(reader, "Internal error"),
                 NULL; // I don't think, that this code is reachable, todo: we
                       // might need to free(s)
        }
      } // slash u
      else if (c == 'n') {
        coReaderNext(reader);
        buf[idx++] = '\n';
      } else if (c == 't') {
        coReaderNext(reader);
        buf[idx++] = '\t';
      } else if (c == 'b') {
        coReaderNext(reader);
        buf[idx++] = '\b';
      } else if (c == 'f') {
        coReaderNext(reader);
        buf[idx++] = '\f';
      } else if (c == 'r') {
        coReaderNext(reader);
        buf[idx++] = '\r';
      } else {
        coReaderNext(reader);
        buf[idx++] = c;
      } // treat escaped char as it is (this will handle both slashes ...
    } // escape
    else {
      coReaderNext(reader);
      buf[idx++] = c; // handle normal char
    }
    // check whether we need to flush the buffer to the string object
    if (idx > COJ_STR_BUF) {
      buf[idx] = '\0';
      if (s == NULL) {
        s = strdup(buf);
        len = idx;
        if (s == NULL)
          return coReaderErr(reader, "Memory error inside string parser"), NULL;
      } else {
        char *t = (char *)realloc(s, len + idx + 1);
        if (t == NULL)
          return coReaderErr(reader, "Memory error inside string parser"),
                 free(s), NULL; // memory error
        s = t;
        strcpy(s + len, buf);
        len += idx;
      }

      idx = 0; // buf is stored in the string object: reset the buffer counter
               // to 0
    } // handle buffer flash
  }
  coReaderNext(reader); // skip final double quote
  coReaderSkipWhiteSpace(reader);
  buf[idx] = '\0';

  if (s == NULL) {
    s = strdup(buf);
    len = idx;
    if (s == NULL)
      return coReaderErr(reader, "Memory error inside string parser"), NULL;
  } else {
    char *t = (char *)realloc(s, len + idx + 1);
    if (t == NULL)
      return coReaderErr(reader, "Memory error inside string parser"), free(s),
             NULL; // memory error
    s = t;
    strcpy(s + len, buf);
    len += idx;
  }

  return s;
}

#define COJ_DBL_BUF 64
co coJSONGetDbl(coReader reader) {
  char buf[COJ_DBL_BUF + 2];
  int i = 0;
  int c;
  for (;;) {
    c = coReaderCurr(reader);
    if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == 'e' ||
        c == 'E' || c == '.') {
      if (i < COJ_DBL_BUF)
        buf[i++] = c;
      coReaderNext(reader);
    } else {
      break;
    }
  }
  coReaderSkipWhiteSpace(reader);
  buf[i] = '\0';
  return coNewDbl(strtod(buf, NULL));
}

co coJSONGetArray(coReader reader) {
  int c;
  co array_obj;
  co element;

  // printf("array start\n");
  if (coReaderCurr(reader) != '[')
    return coReaderErr(reader, "Internal error"), NULL;

  array_obj = coNewVector(CO_FREE_VALS);
  coReaderNext(reader);
  coReaderSkipWhiteSpace(reader);
  for (;;) {
    c = coReaderCurr(reader);
    if (c == ']')
      break;
    if (c < 0)
      return coReaderErr(reader, "Missing ']'"), coDelete(array_obj), NULL;

    if (coVectorEmpty(array_obj) == 0) // expect a ',' after the first element
      if (c == ',') {
        coReaderNext(reader);
        coReaderSkipWhiteSpace(reader);
      }

    element = coJSONGetValue(reader);

    // if ( element == NULL )
    //    return coReaderErr(reader, "'null' element for 'array'"),
    //    coDelete(array_obj), NULL;
    if (coVectorAdd(array_obj, element) < 0)
      return coReaderErr(reader, "Memory error inside 'array'"),
             coDelete(array_obj), NULL;
  }
  coReaderNext(reader); // skip ']'
  coReaderSkipWhiteSpace(reader);
  assert(array_obj != NULL);
  // printf("array end, array_obj=%p\n", array_obj);
  return array_obj;
}

co coJSONGetMap(coReader reader) {
  int c;
  co map_obj;
  co element;
  char *key;
  if (coReaderCurr(reader) != '{')
    return coReaderErr(reader, "Internal error"), NULL;

  map_obj = coNewMap(
      CO_FREE_VALS |
      CO_STRFREE); // do not duplicate keys, because they are already allocated
  if (map_obj == NULL)
    return coReaderErr(reader, "Memory error with map create"), NULL;

  coReaderNext(reader); // skip '{'
  coReaderSkipWhiteSpace(reader);
  for (;;) {
    c = coReaderCurr(reader);
    if (c == '}')
      break;
    if (c < 0)
      return coReaderErr(reader, "Missing '}'"), NULL;

    if (coMapEmpty(map_obj) == 0) // expect a ',' after the first key/value pair
      if (c == ',') {
        coReaderNext(reader);
        coReaderSkipWhiteSpace(reader);
      }

    key =
        coJSONGetStr(reader); // key will contain a pointer to allocated memory
    if (key == NULL)
      return coDelete(map_obj), NULL;

    coReaderSkipWhiteSpace(reader);
    if (coReaderCurr(reader) != ':')
      return coReaderErr(reader, "Missng ':'"), free(key), coDelete(map_obj),
             NULL;
    coReaderNext(reader);
    coReaderSkipWhiteSpace(reader);

    element = coJSONGetValue(reader); // may return NULL for the "null" element

    // if ( element == NULL )
    //   return coReaderErr(reader, "Memory error with map element"), free(key),
    //   coDelete(map_obj), NULL;
    /*
printf("key=%s element=", key);
    coPrint(element);
    printf("\n");
    */
    if (coMapAdd(map_obj, key, element) == 0)
      return coReaderErr(reader, "Memory error with map update"), free(key),
             coDelete(map_obj),
             NULL; // ToDo: do we need to close 'element' here?
  }
  coReaderNext(reader); // skip '}'
  coReaderSkipWhiteSpace(reader);
  assert(map_obj != NULL);
  return map_obj;
}

co coJSONGetValue(coReader reader) {
  const char *identifier;
  int c = coReaderCurr(reader);
  if (c == '[')
    return coJSONGetArray(reader);
  if (c == '{')
    return coJSONGetMap(reader);
  if (c == '\"')
    return coNewStr(
        CO_STRFREE,
        coJSONGetStr(reader)); // return value of coJSONGetStr() is a pointer to
                               // allocated memory, so don't use CO_STRDUP
  if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == 'e' || c == 'E' ||
      c == '.')
    return coJSONGetDbl(reader);

  identifier = coJSONGetIdentifier(reader);
  if (strcmp(identifier, "null") == 0)
    return NULL;
  // todo: handle true, false
  return NULL;
}

co coReadJSONByString(const char *json) {
  struct co_reader_struct reader;
  if (coReaderInitByString(&reader, json) == 0)
    return NULL;
  return coJSONGetValue(&reader);
}

co coReadJSONByFP(FILE *fp) {
  struct co_reader_struct reader;
  if (coReaderInitByFP(&reader, fp) == 0)
    return NULL;
  return coJSONGetValue(&reader);
}

/*===================================================================*/
/* JSON File Write */
/*===================================================================*/

static void coWriteJSONTraverse(cco o, int depth, int isUTF8,
                                FILE *fp); // forward declaration

static void writeIndent(int depth, FILE *fp) {
  while (depth > 0) {
    fprintf(fp, "  ");
    depth--;
  }
}

static void writeString(const char *s, int isUTF8, FILE *fp) {
  while (*s != '\0') {
    if (*s == '\"' || *s == '/' || *s == '\\') {
      fputc('\\', fp);
      fputc(*s, fp);
    } else if (*s == '\n') {
      fputc('\\', fp);
      fputc('n', fp);
    } else if (*s == '\r') {
      fputc('\\', fp);
      fputc('r', fp);
    } else if (*s == '\t') {
      fputc('\\', fp);
      fputc('t', fp);
    } else if (*s == '\f') {
      fputc('\\', fp);
      fputc('f', fp);
    } else if (*s == '\b') {
      fputc('\\', fp);
      fputc('b', fp);
    } else if (*(unsigned char *)s < 32) {
      fprintf(fp, "\\u%04x", *(unsigned char *)s);
    } else if (isUTF8 == 0 && *(unsigned char *)s >= 128) {
      fprintf(fp, "\\u%04x", *(unsigned char *)s);
    } else {
      fputc(*s, fp);
    }
    s++;
  }
}

struct json_traverse_struct {
  int depth;
  int isUTF8;
  FILE *fp;
  long size;
};

static int coMapForEachJSONTraverseCB(cco o, long idx, const char *key,
                                      cco value, void *data) {
  struct json_traverse_struct *jts = (struct json_traverse_struct *)data;
  writeIndent(jts->depth, jts->fp);
  fputc('\"', jts->fp);
  writeString(key, jts->isUTF8, jts->fp);
  fputc('\"', jts->fp);
  fputc(':', jts->fp);
  coWriteJSONTraverse(value, jts->depth + 1, jts->isUTF8, jts->fp);
  if (idx + 1 != jts->size)
    fputc(',', jts->fp);
  if (jts->depth >= 0)
    fputc('\n', jts->fp);
  return 1;
}

static void coWriteJSONTraverse(cco o, int depth, int isUTF8, FILE *fp) {
  if (o == NULL) {
    fprintf(fp, "null");
  } else if (coIsStr(o)) {
    fputc('\"', fp);
    writeString(coStrGet(o), isUTF8, fp);
    fputc('\"', fp);
  } else if (coIsDbl(o)) {
    fprintf(fp, "%.9g", coDblGet(o));
  } else if (coIsVector(o)) {
    long i;
    long cnt = coVectorSize(o);
    fputc('[', fp);
    if (depth >= 0)
      fputc('\n', fp);
    for (i = 0; i < cnt; i++) {
      writeIndent(depth + 1, fp);
      coWriteJSONTraverse(coVectorGet(o, i), depth + 1, isUTF8, fp);
      if (i + 1 != cnt)
        fputc(',', fp);
      if (depth >= 0)
        fputc('\n', fp);
    }
    writeIndent(depth, fp);
    fputc(']', fp);
  } else if (coIsMap(o)) {
    struct json_traverse_struct jts;
    jts.size = coMapSize(o);
    jts.fp = fp;
    jts.depth = depth + 1;
    jts.isUTF8 = isUTF8;
    fputc('{', fp);
    if (depth >= 0)
      fputc('\n', fp);
    coMapForEach(o, coMapForEachJSONTraverseCB, &jts);
    writeIndent(depth, fp);
    fputc('}', fp);
  } else if (coIsMem(o)) // this will NOT generate proper JSON
  {
    unsigned char *ptr = (unsigned char *)coMemGet(o);
    long cnt = coMemSize(o);
    long i;
    fputc('\"', fp);
    for (i = 0; i < cnt; i++) {
      if (i % 64 == 0)
        fputc('\n', fp);
      fprintf(fp, "%02x", ptr[i]);
    }
    fputc('\n', fp);
    fputc('\"', fp);
  }
}

void coWriteJSON(cco o, int isCompact, int isUTF8, FILE *fp) {
  if (isCompact) {
    coWriteJSONTraverse(o, -10000, isUTF8, fp);
  } else {
    coWriteJSONTraverse(o, 0, isUTF8, fp);
  }
}
