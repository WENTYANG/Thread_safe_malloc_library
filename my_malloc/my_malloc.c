#include "my_malloc.h"

block *head;
unsigned long data_size = 0;
unsigned long free_data_size = 0;

void printFreeList() {
  unsigned long len = 0;
  for (block * ptr = head; ptr != NULL; ptr = ptr->next){
    len ++;
  }
  printf("Free list length = %ld\n", len);
}

void *ff_malloc(size_t size)
{
  //Find free block using first fit strategy
  //printFreeList();
  //printf("FF with size = %ld\n", size);
  block *bptr;
  for (bptr = head; bptr != NULL; bptr = bptr->next)
  {
    if (bptr->size >= size)
    {
      //printf("Find block addr = %p with size = %ld\n", bptr, bptr->size);
      return alloc_free(size, bptr);
    }
  }

  //No available free block
  bptr = sbrk(sizeof(block) + size);
  //printf("Sbrk new block at %p, with size = %ld\n", bptr, size);
  bptr->size = size;
  bptr->prev = NULL;
  bptr->next = NULL;
  data_size += sizeof(block) + size;
  return bptr + 1;
}

void *alloc_free(size_t size, block *bptr)
{
  if (bptr->size > size + sizeof(block)) //Check if block needs to be splitted
  {
    //Split the current block
    //printf("Split block\n");
    block *newblk = (block *)((char *)bptr + sizeof(block) + size);
    newblk->size = bptr->size - size - sizeof(block);
    //Remove block from free list
    removeBlock(bptr);
    //Insert the new free block into the linked list, delete the original block
    addBlock(bptr->prev, bptr->next, newblk);
    //Update the block size of *bptr
    bptr->size = size;
  }
  else
  {
    //Remove block from free list
    removeBlock(bptr);
  }
  bptr->next = NULL;
  bptr->prev = NULL;
  return bptr + 1;
}

void ff_free(void *ptr)
{
  //printf("Free block addr = %p\n", ptr);
  block *bptr = (block *)ptr - 1;
  //Scan through free list, insert bptr to appropriate place
  insertBlock(bptr);
}

void addBlock(block *prev, block *next, block *toadd)
{
  //printf("Add block addr = %p\n", toadd);
  toadd->prev = prev;
  toadd->next = next;
  if (prev)
    prev->next = toadd;
  if (next)
    next->prev = toadd;
  if (head == NULL || head == next)
    head = toadd;
  free_data_size += sizeof(block) + toadd->size;
}

void removeBlock(block * bptr)
{
  //printf("Remove block addr = %p\n", bptr);
  if(head == bptr){
    head = bptr->next;
    if(head) head->prev = NULL;
  }
  if (bptr->prev)
    bptr->prev->next = bptr->next;
  if (bptr->next)
    bptr->next->prev = bptr->prev;
  free_data_size -=  sizeof(block) + bptr->size;
}

void insertBlock(block *toinsert)
{
  //printf("Insert block addr = %p\n", toinsert);
  block **cur = &head;
  block *prev = NULL;

  //find insert position;
  while (*cur != NULL && toinsert > *cur)
  {
    prev = *cur;
    cur = &((*cur)->next);
  }

  //insert
  toinsert->next = *cur;
  (*cur) = toinsert;
  toinsert->prev = prev;
  if (toinsert->next != NULL)
    toinsert->next->prev = toinsert;

  free_data_size += sizeof(block) + toinsert->size;

  //Merge prev, toinsert, next
  block *merged = merge(prev, toinsert);
  merge(merged, toinsert->next);
}

block *merge(block *b1, block *b2)
{
  //printf("Merge block addr = %p, and addr = %p", b1, b2);
  //b1 is the previous block
  if(b1 == NULL) return b2;
  if(b2 == NULL) return b1;
  block *b1_end = (void *)b1 + b1->size + sizeof(block);
  if (b2 == b1_end)
  {
    b1->size += b2->size + sizeof(block);
    b1->next = b2->next;
    if(b2->next) b1->next->prev = b1;
    return b1;
  }
  return b2;
}

void *bf_malloc(size_t size)
{
  //Find best block using first fit strategy
  block *bptr;
  block *min = NULL;
  for (bptr = head; bptr != NULL; bptr = bptr->next)
  {
    if (bptr->size == size)
    {
      //printf("Find block addr = %p with size = %ld\n", bptr, bptr->size);
      min = bptr;
      break;
    }
    else if((bptr->size > size) && (min == NULL || bptr->size < min->size)){
      min = bptr;
    }
  }
  if(min != NULL){
    return alloc_free(size, min);
  }
  //No available free block
  bptr = sbrk(sizeof(block) + size);
  //printf("Sbrk new block at %p, with size = %ld\n", bptr, size);
  bptr->size = size;
  bptr->prev = NULL;
  bptr->next = NULL;
  data_size += sizeof(block) + size;
  return bptr + 1;
}

void bf_free(void * ptr) {
  return ff_free(ptr);
}



unsigned long get_data_segment_size() { return data_size; }
unsigned long get_data_segment_free_space_size() { return free_data_size; }