#ifndef KEVCC_UTILS_INCLUDE_HASHMAP_SETINT_MAP_H
#define KEVCC_UTILS_INCLUDE_HASHMAP_SETINT_MAP_H

#include "include/set/bitset.h"

typedef struct tagKevSetIntMapNode {
  KBitSet* key;
  size_t value;
  size_t hashval;
  struct tagKevSetIntMapNode* next;
} KevSetIntMapNode;

typedef struct tagKevSetIntMap {
  KevSetIntMapNode** array;
  size_t capacity;
  size_t size;
} KevSetIntMap;

bool ksetintmap_init(KevSetIntMap* map, size_t capacity);
void ksetintmap_destroy(KevSetIntMap* map);

bool ksetintmap_insert(KevSetIntMap* map, KBitSet* key, size_t value);
KevSetIntMapNode* ksetintmap_search(KevSetIntMap* map, KBitSet* key);
void ksetintmap_make_empty(KevSetIntMap* map);

#endif
