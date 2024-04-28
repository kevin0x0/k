#ifndef KEVCC_UTILS_INCLUDE_QUEUE_INT_QUEUE_H
#define KEVCC_UTILS_INCLUDE_QUEUE_INT_QUEUE_H

#include "include/utils/utils.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct tagKIQueue {
  size_t* array;
  size_t capacity;
  size_t head;
  size_t tail;
} KIQueue;

bool kev_intqueue_init(KIQueue* queue);
void kev_intqueue_destroy(KIQueue* queue);

bool kev_intqueue_expand(KIQueue* queue);
static inline bool kev_intqueue_insert(KIQueue* queue, size_t element);
static inline size_t kev_intqueue_pop(KIQueue* queue);

static inline bool kev_intqueue_empty(KIQueue* queue) {
  return queue->head == queue->tail;
}

static inline bool kev_intqueue_insert(KIQueue* queue, size_t element) {
  size_t new_tail = (queue->tail + 1) & (queue->capacity - 1);
  if (new_tail == queue->head) {
    if (k_unlikely(!kev_intqueue_expand(queue)))
      return false;
    new_tail = (queue->tail + 1) & (queue->capacity - 1);
  }
  queue->array[queue->tail] = element;
  queue->tail = new_tail;
  return true;
}

static inline size_t kev_intqueue_pop(KIQueue* queue) {
  size_t retval = queue->array[queue->head];
  queue->head = (queue->head + 1) & (queue->capacity - 1);
  return retval;
}

#endif
