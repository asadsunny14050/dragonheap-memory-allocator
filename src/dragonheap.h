#pragma once
#include "memory_store.h"
#include <stdbool.h>
#include <stddef.h>
#define META_SIZE sizeof(memory_chunk_t)

// malloc function
void *dragon_hoard(size_t requested_size);
// realloc function
void *dragon_reforge(void *hoard, size_t revised_size);
// free function
void dragon_release(void *hoard);

// debug functions
void print_dragonheap();
void print_free_list();
