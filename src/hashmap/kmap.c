#include "include/hashmap/kmap.h"
#include "include/kutils/utils.h"

#include <stdlib.h>

inline static size_t kmap_hashing(void* key) {
  return (size_t)key >> 3;
}

static void kmap_rehash(KMap* to, KMap* from) {
  size_t from_capacity = from->capacity;
  size_t to_capacity = to->capacity;
  KMapNode** from_array = from->array;
  KMapNode** to_array = to->array;
  size_t mask = to_capacity - 1;
  for (size_t i = 0; i < from_capacity; ++i) {
    KMapNode* node = from_array[i];
    while (node) {
      KMapNode* tmp = node->next;
      size_t hashval = kmap_hashing(node->key);
      size_t index = hashval & mask;
      node->next = to_array[index];
      to_array[index] = node;
      node = tmp;
    }
  }
  to->size = from->size;
  free(from->array);
  from->array = NULL;
  from->capacity = 0;
  from->size = 0;
}

static bool kmap_expand(KMap* map) {
  KMap new_map;
  if (k_unlikely(!kmap_init(&new_map, map->capacity << 1)))
    return false;
  kmap_rehash(&new_map, map);
  *map = new_map;
  return true;
}

static void kmap_bucket_free(KMapNode* bucket) {
  while (bucket) {
    KMapNode* tmp = bucket->next;
    free(bucket);
    bucket = tmp;
  }
}

inline static size_t pow_of_2_above(size_t num) {
  size_t pow = 8;
  while (pow < num)
    pow <<= 1;

  return pow;
}

bool kmap_init(KMap* map, size_t capacity) {
  if (!map) return false;

  /* TODO: make sure capacity is power of 2 */
  capacity = pow_of_2_above(capacity);
  KMapNode** array = (KMapNode**)malloc(sizeof (KMapNode*) * capacity);
  if (k_unlikely(!array)) {
    map->array = NULL;
    map->capacity = 0;
    map->size = 0;
    return false;
  }

  for (size_t i = 0; i < capacity; ++i) {
    array[i] = NULL;
  }
  
  map->array = array;
  map->capacity = capacity;
  map->size = 0;
  return true;
}

void kmap_destroy(KMap* map) {
  if (k_unlikely(!map)) return;

  KMapNode** array = map->array;
  size_t capacity = map->capacity;
  for (size_t i = 0; i < capacity; ++i)
    kmap_bucket_free(array[i]);
  free(array);
  map->array = NULL;
  map->capacity = 0;
  map->size = 0;
}

bool kmap_insert(KMap* map, void* key, void* value) {
  if (k_unlikely(map->size >= map->capacity && !kmap_expand(map)))
    return false;

  KMapNode* new_node = (KMapNode*)malloc(sizeof (*new_node));
  if (k_unlikely(!new_node)) return false;

  size_t index = (map->capacity - 1) & kmap_hashing(key);
  new_node->key = key;
  new_node->value = value;
  new_node->next = map->array[index];
  map->array[index] = new_node;
  map->size++;
  return true;
}

KMapNode* kmap_search(KMap* map, void* key) {
  size_t index = (map->capacity - 1) & kmap_hashing(key);
  KMapNode* node = map->array[index];
  for (; node; node = node->next) {
    if (node->key == key) break;
  }

  return node;
}
