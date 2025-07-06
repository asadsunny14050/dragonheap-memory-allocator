#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define META_SIZE sizeof(memory_chunk_t)

typedef struct memory_chunk {
  size_t size;
  bool is_free;
  struct memory_chunk *previous_chunk;

} memory_chunk_t;

memory_chunk_t *dragon_heap = {0};

void *dragon_hoard(size_t requested_size) // malloc/calloc
{
  void *hoard = sbrk(requested_size + META_SIZE);
  if (hoard == (void *)-1) {
    printf("memory allocation failed\n");
    return (void *)-1;
  }
  void *new_program_break = sbrk(0);
  // printf("hoard: %p\n", hoard);
  // printf("new program break: %p\n", new_program_break);
  size_t size_returned = new_program_break - hoard;
  // printf("size_returned: %lu\n", size_returned);
  // printf("size_usable: %lu\n", new_program_break - (hoard + META_SIZE));

  memory_chunk_t *current_head = dragon_heap;
  // printf("current_head: %p\n", current_head);
  dragon_heap = hoard;
  dragon_heap->size = requested_size;
  dragon_heap->is_free = false;
  dragon_heap->previous_chunk = NULL;
  if (current_head != NULL) {
    dragon_heap->previous_chunk = current_head;
  }

  return hoard + META_SIZE;
}

void *dragon_claim(void *hoard);                // calloc
void *dragon_reforge(void *hoard, size_t size); // realloc
void *dragon_release(void *hoard) {

  memory_chunk_t *hoard_meta_data = hoard - META_SIZE;
  printf("hoard_meta_data: %lu\n", hoard_meta_data->size);
  hoard_meta_data->is_free = true;
  memset(hoard, 0, hoard_meta_data->size);
}

void print_dragon_heap(memory_chunk_t *head) {
  memory_chunk_t *tmp = head;
  while (tmp != NULL) {
    if (tmp == head) {

      printf("(%p, %lu, %d)", tmp, tmp->size, tmp->is_free);
    } else {

      printf(" -> (%p, %lu, %d)", tmp, tmp->size, tmp->is_free);
    }
    tmp = tmp->previous_chunk;
  }
  printf("\n");
}

int main() {

  void *mem = dragon_hoard(13);
  void *mem2 = dragon_hoard(15);
  void *mem3 = dragon_hoard(20);
  print_dragon_heap(dragon_heap);
  dragon_release(mem);
  print_dragon_heap(dragon_heap);

  return 0;
}
