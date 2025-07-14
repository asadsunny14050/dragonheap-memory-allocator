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

// malloc function
void *dragon_hoard(size_t requested_size);
// calloc function
void *dragon_claim(void *hoard);
// realloc function
void *dragon_reforge(void *hoard, size_t size); // realloc
// free function
void dragon_release(void *hoard);

// debug functions
void print_free_list();
