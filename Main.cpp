#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char **argv)
{

  int basic_block_size = 128, memory_length = 128 * 1024 * 1024;
  static const char *optString = "b:s:";
  int opt = getopt(argc, argv, optString);
  while (opt != -1)
  {
    switch (opt)
    {
    case 'b':
      basic_block_size = atoi(optarg); /* true */
      break;

    case 's':
      memory_length = atoi(optarg);
      break;

    default:
      /* You won't actually get here. */
      break;
    }

    opt = getopt(argc, argv, optString);
  }

  // create memory manager
  BuddyAllocator *allocator = new BuddyAllocator(basic_block_size, memory_length);
  allocator->debug();

  // test memory manager
  cout << "allocating block a" << endl;
  char *a = allocator->alloc(212);
  allocator->debug();
  cout << "allocating block b" << endl;
  char *b = allocator->alloc(300);
  allocator->debug();

  cout << "freeing block a" << endl;
  allocator->free(a);
  allocator->debug();
  cout << "freeing block b" << endl;
  allocator->free(b);
  allocator->debug();

  // destroy memory manager
  delete allocator;
}
