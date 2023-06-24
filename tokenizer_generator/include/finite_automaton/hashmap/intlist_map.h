#ifndef KEVCC_TOKENIZER_GENERATOR_INCLUDE_FINITE_AUTOMATON_HASHMAP_INTLIST_MAP_H
#define KEVCC_TOKENIZER_GENERATOR_INCLUDE_FINITE_AUTOMATON_HASHMAP_INTLIST_MAP_H

#include "tokenizer_generator/include/finite_automaton/list/node_list.h"
#include "tokenizer_generator/include/general/global_def.h"

typedef struct tagKevIntListMapNode {
  uint64_t key;
  KevNodeList* value;
  struct tagKevIntListMapNode* next;
} KevIntListMapNode;

typedef struct tagKevIntListMapBucket {
  KevIntListMapNode* map_node_list;
  struct tagKevIntListMapBucket* next;
} KevIntListMapBucket;

typedef struct tagKevIntListMap {
  KevIntListMapBucket* array;
  KevIntListMapBucket* bucket_head;
  uint64_t capacity;
  uint64_t size;
} KevIntListMap;


bool kev_intlistmap_init(KevIntListMap* map, uint64_t capacity);
void kev_intlistmap_destroy(KevIntListMap* map);

KevIntListMapNode* kev_intlistmap_insert(KevIntListMap* map, uint64_t key, KevNodeList* value);
KevIntListMapNode* kev_intlistmap_search(KevIntListMap* map, uint64_t key);
void kev_intlistmap_make_empty(KevIntListMap* map);

static inline KevIntListMapNode* kev_intlistmap_iterate_begin(KevIntListMap* map);
KevIntListMapNode* kev_intlistmap_iterate_next(KevIntListMap* map, KevIntListMapNode* current);

static inline KevIntListMapNode* kev_intlistmap_iterate_begin(KevIntListMap* map) {
    return map->bucket_head ? map->bucket_head->map_node_list : NULL;
}


#endif
