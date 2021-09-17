/**
 * Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



typedef struct _metadata_t {
  unsigned int size;     // The size of the memory block.
  unsigned char isUsed;  // 0 if the block is free; 1 if the block is used.
} metadata_t;

int free_blocks = 0;

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
    // implement calloc:
    if(size == 0 || num == 0) {
      return NULL;
    }
    void *block = malloc(num * size);
    if(block != NULL) {
      memset(block, 0, num * size);
    }
    return block;
    // return NULL;
}


/**
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */
void *startOfHeap = NULL;
void *split(size_t new_size);

void *malloc(size_t size) {
  // implement malloc
  printf("Inside: malloc(%lu):\n", size);
  if(size == 0) {
    return NULL;
  }
  // // If we have not recorded the start of the heap, record it:
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
  }
  // Print out data about each metadata chunk:
   metadata_t *curMeta = startOfHeap;
   void *endOfHeap = sbrk(0);
  // printf("-- Start of Heap (%p) --\n", startOfHeap);
  // while ((void *)curMeta < endOfHeap) {   
  //   printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
  //   curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  // }
  // printf("-- End of Heap (%p) --\n\n", endOfHeap);

  // metadata_t *meta = sbrk( sizeof(metadata_t) );
  // meta->size = size;
  // meta->isUsed = 1;

  // void *ptr = sbrk(size);
  // return ptr;
  // printf("-- Start of Heap (%p) --\n", startOfHeap);
  // while ((void *)curMeta < endOfHeap) {
  //   printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
  //   if(size <= curMeta->size + sizeof(metadata_t) && curMeta->isUsed == 0) { // free and big space
  //     printf("Splitting \n");
      
  //     metadata_t *new_block = (void *)curMeta + sizeof(metadata_t) + size;
  //     printf("memory adress of new_block %p\n", new_block);
  //     memcpy(new_block, curMeta, sizeof(metadata_t));
  //     new_block->size = curMeta->size - sizeof(metadata_t) - size;
  //     new_block->isUsed = 0;
  //     curMeta->isUsed = 1;
  //     curMeta->size = size;
  //     return new_block;
  //     // meta_data_t *new_block = sizeof(meta_data_t) + size //shift ptr up
  //   }
  //   curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  // }
  void *ptr = split(size);
  if(ptr == NULL) {
    printf("Not splitting \n");
    metadata_t *meta = sbrk( sizeof(metadata_t) );
    meta->size = size;
    meta->isUsed = 1;

    void *ptr = sbrk(size);
    return ptr;
  }
  //printf("-- End of Heap (%p) --\n\n", endOfHeap);
  // metadata_t *meta = sbrk( sizeof(metadata_t) );
  // meta->size = size;
  // meta->isUsed = 1;

  // void *ptr = sbrk(size);
  // return ptr;

   return NULL;
}

void *split(size_t size) {
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  printf("-- Start of Heap (%p) --\n", startOfHeap);
  //printf("-- End of Heap (%p) --\n", endOfHeap);
  while ((void *)curMeta < endOfHeap) {
    printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    printf("sumOfSizes %d \n", curMeta->size + sizeof(metadata_t));
    if(size + sizeof(metadata_t) <= curMeta->size && curMeta->isUsed == 0) { // free and big space
      printf("Splitting \n");
      
      metadata_t *new_block = (void *)curMeta + sizeof(metadata_t) + size;
      printf("memory adress of new_block %p\n", new_block);
      memcpy(new_block, curMeta, sizeof(metadata_t));
      new_block->size = curMeta->size - sizeof(metadata_t) - size;
      new_block->isUsed = 0;
      curMeta->isUsed = 1;
      curMeta->size = size;
      return new_block;
      // meta_data_t *new_block = sizeof(meta_data_t) + size //shift ptr up
    }
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }
  return NULL;
}





void *split2(size_t new_size) {
  if(new_size <= 0) {
    return NULL;
  }
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  //printf("%d\n", free_blocks);
  //if(free_blocks > 0) { // free must have been called
  while((void *)curMeta < endOfHeap) {
    printf("Inside split function \n");
    if(new_size <= curMeta->size && curMeta->isUsed == 0) { // free and big space
      printf("Splitting \n");
      metadata_t *new_block = curMeta + sizeof(metadata_t) + new_size;
      memcpy(new_block, curMeta, sizeof(metadata_t));
      new_block->size = curMeta->size - sizeof(metadata_t) - new_size;
      new_block->isUsed = 0;
      curMeta->isUsed = 1;
      return new_block;
      // meta_data_t *new_block = sizeof(meta_data_t) + size //shift ptr up
    }
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }
 // } 
  return NULL;
}

void print_heap(size_t size) {
  printf("Inside: malloc(%lu):\n", size);
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  printf("-- Start of Heap (%p) --\n", startOfHeap);
  while ((void *)curMeta < endOfHeap) {   // While we're before the end of the heap...
    printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }
  printf("-- End of Heap (%p) --\n\n", endOfHeap);

}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realloc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */
void free(void *ptr) {
  // implement free
  if(ptr == NULL) {
    return;
  }
  metadata_t *meta = ptr - sizeof(metadata_t);
  meta->isUsed = 0;
  free_blocks += 1;
}


/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
    // implement realloc:
    if((size == 0) | (size == 0 && ptr != NULL)) {
      free(ptr);
      return NULL;
    }
    if(ptr == NULL ) {
      return malloc(size);
    }
    
    void *block = ptr - sizeof(metadata_t);
    metadata_t *meta = (metadata_t *) block;

    if(size <= meta->size && ptr != NULL) {
      return ptr;
    }

    void *size_change = malloc(size);
    if(size_change != NULL) {
      memcpy(size_change, ptr, meta->size);
      free(ptr);
      return size_change;
    }
    return NULL;
}
