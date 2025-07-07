#include "./src/dragonheap.h"

int main() {

  void *mem = dragon_hoard(13);
  void *mem2 = dragon_hoard(15);
  void *mem3 = dragon_hoard(20);
  // print_dragon_heap(dragon_heap);
  dragon_release(mem);
  dragon_release(mem2);
  // print_dragon_heap(dragon_heap);

  return 0;
}
