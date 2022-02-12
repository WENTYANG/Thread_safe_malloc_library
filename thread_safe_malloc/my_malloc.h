#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include<pthread.h>

typedef struct block_t {
  size_t size;
  struct block_t * prev;
  struct block_t * next;
} block;

void * ff_malloc(size_t size);
void ff_free(void * ptr, block** head);

void insertBlock(block * toinsert, block** head);
void removeBlock(block * bptr, block** head);
void addBlock(block * prev, block * next, block * toadd, block** head);
block * merge(block * b1, block * b2);
void * alloc_free(size_t size, block * bptr, block** head);

void * bf_malloc(size_t size, int with_lock, block** head);
void bf_free(void * ptr, block** head);

unsigned long get_data_segment_size();
unsigned long get_data_segment_free_space_size();
void printFreeList();
void printLinkedList();

//Thread Safe malloc/free: locking version
void *ts_malloc_lock(size_t size);
void ts_free_lock(void *ptr);

//Thread Safe malloc/free: non-locking version
void *ts_malloc_nolock(size_t size);
void ts_free_nolock(void *ptr);