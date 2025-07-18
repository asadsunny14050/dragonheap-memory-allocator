
#include "memory_store.h"
#include <stdio.h>

memory_chunk_t *dragon_heap = {0};
free_list_t *dragon_free_list = {0};

void print_dragon_heap() {
  printf("dragon_heap: ");
  memory_chunk_t *tmp = dragon_heap;
  while (tmp != NULL) {
    if (tmp == dragon_heap) {

      printf("(%lu, %lu, %lu, %d)", tmp, tmp->previous_chunk, tmp->size, tmp->is_free);
    } else {

      printf(" -> (%lu, %lu, %lu, %d)", tmp, tmp->previous_chunk, tmp->size, tmp->is_free);
    }
    tmp = tmp->previous_chunk;
  }
  printf("\n");
}

void print_free_list() {
  printf("free_list: ");
  free_list_t *iterator = dragon_free_list;
  while (iterator != NULL) {

    printf("(%lu, %lu) -> ", iterator, iterator->next_free_chunk);
    iterator = iterator->next_free_chunk;
  }
  printf("\n");
}

void rm_from_freelist(free_list_t *previous_node, free_list_t *node_to_delete) {
  if (previous_node != NULL) {
    previous_node->next_free_chunk = node_to_delete->next_free_chunk;

  } else {
    if (node_to_delete == dragon_free_list) {
      dragon_free_list = node_to_delete->next_free_chunk;

    } else {
      free_list_t *iterator = dragon_free_list;
      free_list_t *previous_free_chunk = NULL;
      while (iterator != NULL) {
        if (node_to_delete == iterator && previous_free_chunk) {
          previous_free_chunk->next_free_chunk = node_to_delete->next_free_chunk;
        }
        // printf("(%p, %p) -> ", iterator, iterator->next_free_chunk);
        previous_free_chunk = iterator;
        iterator = iterator->next_free_chunk;
      }
    }
  }
}
void rm_from_dragonheap(memory_chunk_t *next_node, memory_chunk_t *node_to_delete) {
  if (next_node != NULL) {
    next_node->previous_chunk = node_to_delete->previous_chunk;

  } else {
    if (node_to_delete == dragon_heap) {
      dragon_heap = node_to_delete->previous_chunk;
    } else {
      memory_chunk_t *iterator = dragon_heap;
      memory_chunk_t *next_chunk = NULL;
      while (iterator != NULL) {
        if (node_to_delete == iterator && next_chunk) {
          next_chunk->previous_chunk = node_to_delete->previous_chunk;
        }
        // printf("(%p, %p) -> ", iterator, iterator->next_free_chunk);
        next_chunk = iterator;
        iterator = iterator->previous_chunk;
      }
    }
  }
}
