#include "./src/dragonheap.h"
#include <stdio.h>
#include <string.h>

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

  // dragon heap first check for free chunks equal or less in size
  // should find a chunk but not big enough
  // dragon heap should add this new chunk by expanding the heap
  printf("third action\n");
  void *mem2 = dragon_hoard(15);

  // dragon heap should mark the chunk as free
  // add it to the free_list, free_list has now two entries
  printf("fourth action\n");
  dragon_release(mem2);
  print_free_list();

  // dragon heap first check for free chunks equal or less in size
  //  should find two chunks big enough to coalesce (15 + 13) = 28, more than enough
  printf("fifth action\n");
  void *mem3 = dragon_hoard(20);
  print_free_list();

  printf("sixth action\n");
  dragon_release(mem3);
  print_free_list();

  // testing realloc function so first we allcaote mem4 of 33 bytes
  printf("seventh action\n");
  void *mem4 = dragon_hoard(33);
  print_free_list();

  // then we want to resize the memory region from 33 to 60 bytes, anything below or equal to 33 should return an error
  printf("eighth action\n");
  dragon_reforge(mem4, 60);
  print_dragon_heap();

  // in the next two actions, we allocate and free the next adjacent chunk of memory to mem4 to test we can see upon further reallocation of mem4, does the realloc functions coalesces the next adjacent chunk, the chunk has to have enough size, if not enough size it sees if that insufficient chunk is at the program break, if so, it expands the heap
  printf("ninth action\n");
  dragon_release(dragon_hoard(10));
  print_free_list();
  print_dragon_heap();

  printf("tenth action\n");
  dragon_reforge(mem4, 100);
  print_free_list();
  print_dragon_heap();
  // the next set of lines tests to see if the "wanting to realloc chunk" isn't at the program break so thus can't expand the heap, and its next adjacent chunks aren't free, then it finds a new chunk from the free list or by expanding the heap by using the dragon_hoard function inside its implemention, if such a case it's encountered the new chunk copies the data from the old chunk, and the old chunk is freed and its data is emptied, and the pointer to the new chunk's hoard (the actual region to used by the user) is returned
  printf("eleventh action\n");
  void *mem5 = dragon_hoard(15);
  void *mem6 = dragon_hoard(10);
  void *mem7 = dragon_hoard(5);
  dragon_release(mem4);
  printf("mem6: %p\n", mem6);
  // checking to see if you can use the hoard or not
  strcpy(mem6, "hello");
  printf("mem6: %s\n", (char *)mem6);
  mem6 = dragon_reforge(mem6, 100);
  printf("mem6: %p\n", mem6);
  // checking to see if the old data is transferred to this new region or not and can i add new data to it
  strcpy(mem6 + 5, ", son!");
  printf("mem6: %s\n", (char *)mem6);

  print_dragon_heap();
  print_free_list();

  return 0;
}
