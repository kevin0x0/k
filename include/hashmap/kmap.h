#ifndef _K_INCLUDE_HASHMAP_KMAP_H_
#define _K_INCLUDE_HASHMAP_KMAP_H_

#include <stdbool.h>
#include <stddef.h>

typedef struct tagKMapNode {
  void* key;
  void* value;
  struct tagKMapNode* next;
} KMapNode;

typedef struct tagKMap {
  KMapNode** array;
  size_t capacity;
  size_t size;
} KMap;

bool kmap_init(KMap* map, size_t capacity);
void kmap_destroy(KMap* map);

bool kmap_insert(KMap* map, void* key, void* value);
KMapNode* kmap_search(KMap* map, void* key);

#endif
