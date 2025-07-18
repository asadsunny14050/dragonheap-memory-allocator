#pragma once

#include <stdbool.h>
#include <stddef.h>
#define META_SIZE sizeof(memory_chunk_t)

typedef struct memory_chunk {
  size_t size;
  bool is_free;
  struct memory_chunk *previous_chunk;

} memory_chunk_t;

typedef struct free_list {
  struct free_list *next_free_chunk;
} free_list_t;

void print_dragon_heap();
void print_free_list();
void rm_from_freelist(free_list_t *previous_node, free_list_t *node_to_delete);
void rm_from_dragonheap(memory_chunk_t *next_node,
                        memory_chunk_t *node_to_delete);
