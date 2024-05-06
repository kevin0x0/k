#include "include/queue/kiqueue.h"

#include <stdlib.h>
#include <string.h>

#define KEV_INTQUEUE_DEFAULT_SIZE   (32)

bool kev_intqueue_init(KIQueue* queue) {
  if (k_unlikely(!queue)) return false;
  queue->array = NULL;
  queue->head = 0;
  queue->array = (KIQueueInt*)malloc(sizeof (KIQueueInt) * KEV_INTQUEUE_DEFAULT_SIZE);
  if (k_unlikely(!queue->array)) {
    queue->tail = 0;
    queue->capacity = 0;
    return false;
  }
  queue->tail = 0;
  queue->capacity = 8;
  return true;
}

void kev_intqueue_destroy(KIQueue* queue) {
  if (k_unlikely(queue))
    return;
  free(queue->array);
  queue->array = NULL;
  queue->capacity = 0;
  queue->head = 0;
  queue->tail = 0;

}

bool kev_intqueue_expand(KIQueue* queue) {
  size_t new_size = queue->capacity * 2;
  KIQueueInt* array = (KIQueueInt*)malloc(new_size * sizeof (KIQueueInt));
  if (k_unlikely(!array)) return false;
  if (queue->head < queue->tail) {
    memcpy(array, queue->array + queue->head, sizeof (KIQueueInt) * (queue->tail - queue->head));
    free(queue->array);
    queue->array = array;
    queue->tail = queue->tail - queue->head;
    queue->head = 0;
    queue->capacity = new_size;
  } else if (queue->head > queue->tail) {
    memcpy(array, queue->array + queue->head, sizeof (KIQueueInt) * (queue->capacity - queue->head));
    memcpy(array + (queue->capacity - queue->head), queue->array, queue->tail * sizeof (KIQueueInt));
    queue->tail = queue->tail + queue->capacity - queue->head;
    queue->head = 0;
    queue->capacity = new_size;
  } else {
    queue->capacity = new_size;
  }
  return true;
}

