#ifndef KEVCC_UTILS_INCLUDE_QUEUE_ADDR_QUEUE_H
#define KEVCC_UTILS_INCLUDE_QUEUE_ADDR_QUEUE_H

#include "include/utils/utils.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct tagKQueue {
  void** array;
  size_t capacity;
  size_t head;
  size_t tail;
} KQueue;

bool kqueue_init(KQueue* queue);
void kqueue_destroy(KQueue* queue);

bool kqueue_expand(KQueue* queue);
static inline bool kqueue_insert(KQueue* queue, void* element);
static inline void* kqueue_pop(KQueue* queue);

static inline bool kqueue_empty(KQueue* queue) {
  return queue->head == queue->tail;
}

static inline bool kqueue_insert(KQueue* queue, void* element) {
  size_t new_tail = (queue->tail + 1) & (queue->capacity - 1);
  if (new_tail == queue->head) {
    if (k_unlikely(!kqueue_expand(queue)))
      return false;
    new_tail = (queue->tail + 1) & (queue->capacity - 1);
  }
  queue->array[queue->tail] = element;
  queue->tail = new_tail;
  return true;
}

static inline void* kqueue_pop(KQueue* queue) {
  void* retval = queue->array[queue->head];
  queue->head = (queue->head + 1) & (queue->capacity - 1);
  return retval;
}

#endif
