#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct block_t {
  size_t size;
  struct block_t * prev;
  struct block_t * next;
} block;

void * ff_malloc(size_t size);
void ff_free(void * ptr);

void insertBlock(block * toinsert);
void removeBlock(block * bptr);
void addBlock(block * prev, block * next, block * toadd);
block * merge(block * b1, block * b2);
void * alloc_free(size_t size, block * bptr);

void * bf_malloc(size_t size);
void bf_free(void * ptr);

unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();
void printFreeList();