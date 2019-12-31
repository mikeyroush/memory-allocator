/* 
    File: my_allocator.cpp
*/
#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
    //initialize variables and allocate memory
    minBlock = _basic_block_size;
    totalMem = _total_memory_length;
    mem = new char(_total_memory_length);
    
//    printf("memory space : [%llx, %llx]\n", mem,mem+_total_memory_length-1);
    
    //calculate how many indices
    int vecLength = log2(_total_memory_length / _basic_block_size) + 1;
    for (int i = 0; i < vecLength; i++) {
        freeList.push_back(new LinkedList());
    }
    BlockHeader* block = (BlockHeader*)mem;
    //block->edit(_total_memory_length, false, nullptr);
    block->size = _total_memory_length;
    block->used = false;
    block->next = nullptr;
    freeList[vecLength-1]->insert(block);
//    debug();
}

BuddyAllocator::~BuddyAllocator (){
    for (int i = 0; i < freeList.size(); i++) {
        BlockHeader* curr = freeList[i]->getFirst();
        while (curr != nullptr) {
            freeList[i]->remove(curr);
            curr = curr->next;
        }
        delete freeList[i];
    }
    delete mem;
}

char* BuddyAllocator::alloc(int _length) {
    //find new length of memory to allocate
    int newLength = pow(2,ceil(log2((double)_length + sizeof(BlockHeader))));
    if (newLength < minBlock){
        newLength = minBlock;
    } //cout << "wanted: " << _length << ", new: " << newLength << endl;
    
    //find which linked lists to look in and check them for free blocks
    int index = log2(newLength / minBlock);
    BlockHeader* addr = nullptr;
    for (int i = index; i < freeList.size(); i++) {
        if (freeList[i]->getFirst() != nullptr) {
            addr = freeList[i]->getFirst();
            break;
        }
    }
    
    //if it couldn't find a free block, then it returns nullptr
    if (addr == nullptr) {
        return nullptr;
    }
    
    //split block as needed
    if (addr->size > newLength) {
        int num = log2(addr->size / newLength);
        for (int i = 0; i < num; i++) {
            addr = split(addr);
        }
    } //else remove the block from the appropriate list and edit its header
    else{
        int index = freeList.size() - log2(totalMem / addr->size) -1;
        freeList[index]->remove(addr);
        addr->used = true;
        addr->next = nullptr;
    }
//    debug();
    return (char*)addr + sizeof(BlockHeader);
}

int BuddyAllocator::free(char* _a) {
    //if the biggest piece of mem is free, then there is no more mem to free
    if (freeList[freeList.size()-1]->getFirst() != nullptr)
        return 1;
    
    //find adjusted addr and add it to the freeList, then calc buddy
    BlockHeader* addr = (BlockHeader*)(_a - sizeof(BlockHeader));
    int index = freeList.size() - log2(totalMem / addr->size) -1;
    freeList[index]->insert(addr);
    addr->used = false;
//    debug();
    BlockHeader* buddy = getbuddy(addr);
    
    //check the size equality, and that they are free at the moment
    bool isbuddy = arebuddies (addr, buddy);
    if (isbuddy){
        BlockHeader* new_block = merge (addr, buddy);
        while (new_block) {
            //BlockHeader* new_addr = (BlockHeader*)(new_block - sizeof(BlockHeader));
            //use new addr and find new buddy
            BlockHeader* new_addr = new_block;
            BlockHeader* new_buddy = getbuddy(new_addr);
            
            //if they are free, call merge again, else exit while loop
            isbuddy = arebuddies (new_addr, new_buddy);
            if(!isbuddy) {
                new_block = nullptr;
            } else {
                new_block = merge (new_addr, new_buddy);
            }
        }
    }
//    debug();
  return 0;
}

BlockHeader* BuddyAllocator::split(BlockHeader *block){
    //if half the block size is >= the minBlock size then go ahead and do the split
    if (block->size/2 >= minBlock) {
        //find the linked list with the block and remove the block from the list
        int index = freeList.size() - log2(totalMem / block->size) -1;
        freeList[index]->remove(block);
        
        //place a new block header halfway through the orginal block and edit the block headers accordingly
        char* charMid = (char*)block;
        charMid = charMid + block->size/2;
        BlockHeader* mid = (BlockHeader*)charMid;
        
//        cout << "block->size : " << block->size << endl;
//        printf("block1_addr: %llx, block2_addr: %llx\n", block, charMid);
        
        //edit blockheaders
        block->size = block->size/2;
        block->used = true;
        block->next = nullptr;
        mid->size = block->size;
        mid->used = false;
        mid->next = nullptr;
        
        //insert the new block in the appropriate linked list and return the original
        index -= 1;
        freeList[index]->insert(mid);
//        debug();
        return block;
    }
    return nullptr;
}

BlockHeader* BuddyAllocator::merge(BlockHeader *block1, BlockHeader *block2){
    //remove both blocks from their linked list
    int index = freeList.size() - log2(totalMem / block1->size) -1;
    freeList[index]->remove(block1);
    freeList[index]->remove(block2);
    
    //find which block is on the left and edit it's header
    if (block1 < block2) {
        //set the right block to default values
        block2->size = 0;
        block2->used = false;
        block2->next = nullptr;
        //update left block
        block1->size = block1->size*2;
        block1->used = false;
        block1->next = nullptr;
        freeList[index+1]->insert(block1);
//        debug();
        return block1;
    } else{
        //block2->edit((block2->size)*2, false, nullptr);
        //set right block to default values
        block1->size = 0;
        block1->used = false;
        block1->next = nullptr;
        //update left block
        block2->size = block2->size*2;
        block2->used = false;
        block2->next = nullptr;
        freeList[index+1]->insert(block2);
//        debug();
        return block2;
    }
    return nullptr;
}

BlockHeader* BuddyAllocator::getbuddy(BlockHeader * addr){
    return (BlockHeader*)((((char*)addr - mem) ^ addr->size) + mem);
}

bool BuddyAllocator::arebuddies(BlockHeader* block1, BlockHeader* block2){
    return ( (block1->size == block2->size) & (block1->used == false) & (block2->used == false) );
}

void BuddyAllocator::debug (){
    //print out the number of free blocks of each size
    for (int i = 0; i < freeList.size(); i++) {
        cout << minBlock*pow(2, i) << ": ";
        BlockHeader* curr = freeList[i]->getFirst();
        int num = 0;
        while (curr != nullptr) {
            num++;
            cout << '\t' << curr->size;
            curr = curr->next;
        }
        cout << endl << num << endl;
    }
    cout << endl;
}

