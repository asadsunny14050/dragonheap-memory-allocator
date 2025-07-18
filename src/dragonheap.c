#include "dragonheap.h"
#include "memory_store.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
extern memory_chunk_t *dragon_heap;
extern free_list_t *dragon_free_list;

// #define META_SIZE sizeof(memory_chunk_t)

// free_list_t *dragon_free_list = {0};

// void print_dragon_heap(memory_chunk_t *head) {
//   printf("dragon_heap: ");
//   memory_chunk_t *tmp = head;
//   while (tmp != NULL) {
//     if (tmp == head) {

//       printf("(%lu, %lu, %lu, %d)", tmp, tmp->previous_chunk, tmp->size, tmp->is_free);
//     } else {

//       printf(" -> (%lu, %lu, %lu, %d)", tmp, tmp->previous_chunk, tmp->size, tmp->is_free);
//     }
//     tmp = tmp->previous_chunk;
//   }
//   printf("\n");
// }

// void print_free_list() {
//   printf("free_list: ");
//   free_list_t *iterator = dragon_free_list;
//   while (iterator != NULL) {

//     printf("(%lu, %lu) -> ", iterator, iterator->next_free_chunk);
//     iterator = iterator->next_free_chunk;
//   }
//   printf("\n");
// }

// void rm_from_freelist(free_list_t *previous_node, free_list_t *node_to_delete) {
//   if (previous_node != NULL) {
//     previous_node->next_free_chunk = node_to_delete->next_free_chunk;

//   } else {
//     if (node_to_delete == dragon_free_list) {
//       dragon_free_list = node_to_delete->next_free_chunk;
//     } else {
//       free_list_t *iterator = dragon_free_list;
//       free_list_t *previous_free_chunk = NULL;
//       while (iterator != NULL) {
//         if (node_to_delete == iterator && previous_free_chunk) {
//           previous_free_chunk->next_free_chunk = node_to_delete->next_free_chunk;
//         }
//         // printf("(%p, %p) -> ", iterator, iterator->next_free_chunk);
//         previous_free_chunk = iterator;
//         iterator = iterator->next_free_chunk;
//       }
//     }
//   }
// }

void *dragon_hoard(size_t requested_size) {

  size_t received_size = requested_size;
  printf("user requested %lu bytes\n", requested_size);
  memory_chunk_t *chunk = NULL;
  memory_chunk_t *current_head = dragon_heap;

  free_list_t *current_free_chunk = dragon_free_list;
  free_list_t *previous_free_chunk = NULL;
  printf("\nfree chunks available: ");

  while (current_free_chunk != NULL) {

    memory_chunk_t *free_chunk_meta = (void *)current_free_chunk - META_SIZE;

    printf("(%lu, %lu) - ", free_chunk_meta, free_chunk_meta->previous_chunk);
    printf("(is_free: %d, size: %lu) -> ", free_chunk_meta->is_free, free_chunk_meta->size);

    if (free_chunk_meta->size >= requested_size && free_chunk_meta->is_free) {
      chunk = free_chunk_meta;
      received_size = free_chunk_meta->size;
      rm_from_freelist(previous_free_chunk, current_free_chunk);
      break;

    } else {
      if (free_chunk_meta->previous_chunk && free_chunk_meta->previous_chunk->is_free) {
        printf("previous chunk is free and it's ripe for coalesce\n");
        if (free_chunk_meta->previous_chunk->size + free_chunk_meta->size + META_SIZE < requested_size)
          break;

        free_list_t *previous_free_chunk = (void *)free_chunk_meta->previous_chunk + META_SIZE;
        rm_from_freelist(NULL, current_free_chunk);
        rm_from_freelist(NULL, previous_free_chunk);

        chunk = free_chunk_meta->previous_chunk;
        received_size = free_chunk_meta->previous_chunk->size + free_chunk_meta->size + META_SIZE;
        memset(free_chunk_meta, 0, META_SIZE);
        memory_chunk_t *iterator = dragon_heap;
        if (current_head == free_chunk_meta) {
          printf("clap");
          dragon_heap = chunk;
        }
        while (iterator != NULL) {
          if (iterator->previous_chunk == free_chunk_meta) {
            printf("i'm running\n");
            iterator->previous_chunk = chunk;
            break;
          }
          iterator = iterator->previous_chunk;
        }
        break;
      }
    }
    previous_free_chunk = current_free_chunk;
    current_free_chunk = current_free_chunk->next_free_chunk;
  }
  printf("\n\n");
  if (chunk == NULL) {
    chunk = sbrk(requested_size + META_SIZE);
    dragon_heap = chunk;
    if (current_head != NULL) {
      chunk->previous_chunk = current_head;
    }
  }

  if (chunk == (void *)-1) {
    printf("memory allocation failed\n");
    return (void *)-1;
  }
  // void *new_program_break = sbrk(0);
  // printf("hoard: %p\n", chunk);
  // printf("new program break: %p\n", new_program_break);
  // size_t size_returned = new_program_break - chunk;
  // printf("size_returned: %lu\n", size_returned);
  // printf("size_usable: %lu\n", new_program_break - (chunk + META_SIZE));

  // printf("current_head: %p\n", current_head);
  chunk->size = received_size;
  chunk->is_free = false;

  void *hoard = (void *)chunk + META_SIZE;

  return hoard;
}

void *dragon_claim(void *hoard); // calloc
void *dragon_reforge(void *hoard, size_t revised_size) {
  memory_chunk_t *current_chunk = (void *)hoard - META_SIZE;
  if (revised_size < current_chunk->size) {
    return (void *)-1;
  }

  size_t additional_size = revised_size - current_chunk->size;

  void *program_break = sbrk(0);
  if (program_break == hoard + current_chunk->size) {
    // use sbrk
    void *additional_hoard = sbrk(additional_size);
    current_chunk->size = revised_size;
    return hoard;
  } else {
    printf("possible segfault here, look out!\n");
    memory_chunk_t *next_chunk = hoard + current_chunk->size;
    if (next_chunk && next_chunk->is_free) {
      if ((next_chunk->size + META_SIZE) < additional_size) {
        additional_size = additional_size - (next_chunk->size + META_SIZE);
        sbrk(additional_size);
        current_chunk->size += additional_size;
      }

      current_chunk->size += next_chunk->size + META_SIZE;
      rm_from_freelist(NULL, (void *)next_chunk + META_SIZE);
      rm_from_dragonheap(NULL, next_chunk);
      memset(next_chunk, 0, META_SIZE);
      return hoard;
    }
  }
  return NULL;
}
void dragon_release(void *hoard) {

  memory_chunk_t *hoard_meta_data = hoard - META_SIZE;
  // printf("hoard_meta_data: %lu\n", hoard_meta_data->size);
  hoard_meta_data->is_free = true;
  memset(hoard, 0, hoard_meta_data->size);
  print_dragon_heap();

  free_list_t *current_head = dragon_free_list;
  dragon_free_list = hoard;
  if (current_head != NULL) {
    free_list_t *free_list_node = hoard;
    free_list_node->next_free_chunk = current_head;
    print_free_list();
  }
}
