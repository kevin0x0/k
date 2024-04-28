#include "include/hashmap/strx_map.h"

#include <stdlib.h>
#include <string.h>


inline static size_t kstrxmap_hashing(const char* key) {
  if (!key) return 0;
  size_t hashval = 0;
  size_t count = 0;
  while (*key != '\0' && count++ < 8) {
    hashval ^= ((size_t)*key++ << (hashval & 0x3F));
  }
  return hashval;
}

static void kstrxmap_rehash(KStrXMap* to, KStrXMap* from) {
  size_t from_capacity = from->capacity;
  size_t to_capacity = to->capacity;
  KStrXMapBucket* from_array = from->array;
  KStrXMapBucket* to_array = to->array;
  KStrXMapBucket* bucket_head = NULL;
  size_t mask = to_capacity - 1;
  for (size_t i = 0; i < from_capacity; ++i) {
    KStrXMapNode* node = from_array[i].map_node_list;
    while (node) {
      KStrXMapNode* tmp = node->next;
      size_t hashval = node->hashval;
      size_t index = hashval & mask;
      node->next = to_array[index].map_node_list;
      to_array[index].map_node_list = node;
      if (node->next == NULL) {
        to_array[index].next = bucket_head;
        bucket_head = &to_array[index];
      }
      node = tmp;
    }
  }
  to->size = from->size;
  to->bucket_head = bucket_head;
  free(from->array);
  from->bucket_head = NULL;
  from->array = NULL;
  from->capacity = 0;
  from->size = 0;
}

static bool kstrxmap_expand(KStrXMap* map) {
  KStrXMap new_map;
  if (!kstrxmap_init(&new_map, map->capacity << 1))
    return false;
  kstrxmap_rehash(&new_map, map);
  *map = new_map;
  return true;
}

static void kstrxmap_bucket_free(KStrXMapBucket* bucket) {
  KStrXMapNode* node = bucket->map_node_list;
  while (node) {
    KStrXMapNode* tmp = node->next;
    free(node->key);
    free(node);
    node = tmp;
  }
  bucket->map_node_list = NULL;
  bucket->next = NULL;
}

inline static size_t pow_of_2_above(size_t num) {
  size_t pow = 8;
  while (pow < num)
    pow <<= 1;

  return pow;
}

bool kstrxmap_init(KStrXMap* map, size_t capacity) {
  if (!map) return false;

  map->bucket_head = NULL;
  capacity = pow_of_2_above(capacity);
  KStrXMapBucket* array = (KStrXMapBucket*)malloc(sizeof (KStrXMapBucket) * capacity);
  if (!array) {
    map->array = NULL;
    map->capacity = 0;
    map->size = 0;
    return false;
  }

  for (size_t i = 0; i < capacity; ++i) {
    array[i].map_node_list = NULL;
  }
  
  map->array = array;
  map->capacity = capacity;
  map->size = 0;
  return true;
}

KStrXMap* kstrxmap_create(size_t capacity) {
  KStrXMap* map = (KStrXMap*)malloc(sizeof (KStrXMap));
  if (!map || !kstrxmap_init(map, capacity)) {
    free(map);
    return NULL;
  }
  return map;
}

void kstrxmap_destroy(KStrXMap* map) {
  if (map) {
    KStrXMapBucket* bucket = map->bucket_head;
    while (bucket) {
      KStrXMapBucket* tmp = bucket->next;
      kstrxmap_bucket_free(bucket);
      bucket = tmp;
    }
    free(map->array);
    map->bucket_head = NULL;
    map->array = NULL;
    map->capacity = 0;
    map->size = 0;
  }
}

void kstrxmap_delete(KStrXMap* map) {
  kstrxmap_destroy(map);
  free(map);
}

bool kstrxmap_insert(KStrXMap* map, const char* key, void* value) {
  if (map->size >= map->capacity && !kstrxmap_expand(map))
    return false;

  KStrXMapNode* new_node = (KStrXMapNode*)malloc(sizeof (KStrXMapNode));
  if (!new_node) return false;

  size_t hashval = kstrxmap_hashing(key);
  size_t index = (map->capacity - 1) & hashval;
  size_t len = strlen(key);
  new_node->key = (char*)malloc((len + 1) * sizeof (char));
  if (!new_node->key) return NULL;
  strcpy(new_node->key, key);
  new_node->hashval = hashval;
  new_node->value = value;
  if (!new_node->key) {
    free(new_node->key);
    return false;
  }
  new_node->next = map->array[index].map_node_list;
  map->array[index].map_node_list = new_node;
  map->size++;
  if (new_node->next == NULL) {
    map->array[index].next = map->bucket_head;
    map->bucket_head = &map->array[index];
  }
  return true;
}

KStrXMapNode* kstrxmap_search(KStrXMap* map, const char* key) {
  size_t hashval = kstrxmap_hashing(key);
  size_t index = (map->capacity - 1) & hashval;
  KStrXMapNode* node = map->array[index].map_node_list;
  for (; node; node = node->next) {
    if (node->hashval == hashval &&
        strcmp(node->key, key) == 0)
      break;
  }
  return node;
}

KStrXMapNode* kstrxmap_iterate_next(KStrXMap* map, KStrXMapNode* current) {
  if (current->next) return current->next;
  size_t index = (map->capacity - 1) & current->hashval;
  KStrXMapBucket* current_bucket = &map->array[index];
  if (current_bucket->next)
    return current_bucket->next->map_node_list;
  return NULL;
}
