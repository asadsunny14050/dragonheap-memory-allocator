#include "./src/dragonheap.h"
#include <stdio.h>

int main() {
  printf("%lu\n", META_SIZE);

  // dragon heap should add this chunk
  printf("first action\n");
  void *mem = dragon_hoard(13);

  // dragon heap should mark the chunk as free
  // add it to the free_list
  printf("second action\n");
  dragon_release(mem);
  print_free_list();

  // dragon heap first check for free chunks equal or less in se
  // should find a chunk but not big enough
  // dragon heap should add this new chunk
  printf("third action\n");
  void *mem2 = dragon_hoard(15);

  // dragon heap should mark the chunk as free
  // add it to the free_list, free_list has now two entries
  printf("fourth action\n");
  dragon_release(mem2);
  print_free_list();

  // dragon heap first check for free chunks equal or less in size
  //  should find two chunks big enough to coalesce (15 + 13) = 28, more than enough
  // should print this fact but still allocate additional 20 chunk
  printf("fifth action\n");
  void *mem3 = dragon_hoard(20);
  print_free_list();

  printf("sixth action\n");
  dragon_release(mem3);
  print_free_list();

  void *mem4 = dragon_hoard(33);
  print_free_list();
  // print_dragon_heap(dragon_heap);
  // print_dragon_heap(dragon_heap);

  return 0;
}
