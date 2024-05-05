#ifndef _K_INCLUDE_HASHMAP_STRX_MAP_H_
#define _K_INCLUDE_HASHMAP_STRX_MAP_H_


#include <stdbool.h>
#include <stddef.h>
typedef struct tagKStrXMapNode {
  char* key;
  void* value;
  size_t hashval;
  struct tagKStrXMapNode* next;
} KStrXMapNode;

typedef struct tagKStrXMapBucket {
  KStrXMapNode* map_node_list;
  struct tagKStrXMapBucket* next;
} KStrXMapBucket;

typedef struct tagKStrXMap {
  KStrXMapBucket* array;
  KStrXMapBucket* bucket_head;
  size_t capacity;
  size_t size;
} KStrXMap;

bool kstrxmap_init(KStrXMap* map, size_t capacity);
KStrXMap* kstrxmap_create(size_t capacity);
void kstrxmap_destroy(KStrXMap* map);
void kstrxmap_delete(KStrXMap* map);

bool kstrxmap_insert(KStrXMap* map, const char* key, void* value);
KStrXMapNode* kstrxmap_search(KStrXMap* map, const char* key);

static inline KStrXMapNode* kstrxmap_iterate_begin(KStrXMap* map);
KStrXMapNode* kstrxmap_iterate_next(KStrXMap* map, KStrXMapNode* current);

static inline KStrXMapNode* kstrxmap_iterate_begin(KStrXMap* map) {
    return map->bucket_head ? map->bucket_head->map_node_list : NULL;
}

#endif
