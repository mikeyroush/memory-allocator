#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char ** argv) {
    
    int basic_block_size = 128, memory_length = 128 * 1024 * 1024;
    static const char *optString = "b:s:";
    int opt = getopt( argc, argv, optString );
    while( opt != -1 ) {
        switch( opt ) {
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
        
        opt = getopt( argc, argv, optString );
    }

  // create memory manager
    BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);
//    allocator->debug();

  // test memory manager
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test.
  
//    char* test = allocator->alloc(2007);
//    allocator->free(test);
    
  // destroy memory manager
  delete allocator;

}
