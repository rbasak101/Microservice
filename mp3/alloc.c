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

  struct metadata_t * next_ptr; // free lists
  struct metadata_t * prev_ptr;
} metadata_t;

int free_blocks = 0;
metadata_t *head = NULL;
metadata_t *tail = NULL;
void *startOfHeap = NULL;

void *split(size_t new_size);
void *split_lists(size_t new_size);
void *split_lists2(size_t size);
void combine_properly(), memory_coalesce(), print_heap(), print_list();
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

void *malloc(size_t size) {
  // implement malloc
  //printf("Inside: malloc(%lu):\n", size);
  if(size == 0) {
    return NULL;
  }
  // // If we have not recorded the start of the heap, record it:
  if (startOfHeap == NULL) {
    startOfHeap = sbrk(0);
  }
  // printf("-- Start of Heap (%p) --\n", startOfHeap);
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);

  //combine_properly(); //here or in free 
  //memory_coalesce();
  //void *ptr = split(size);
  void *ptr = split_lists2(size);
  if(ptr == NULL) {
    metadata_t *meta = sbrk( sizeof(metadata_t) );
    meta->size = size;
    meta->isUsed = 1;
    //printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    ptr = sbrk(size);
  }
  //print_heap();
  return ptr;
}

void *split(size_t size) {
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  //print_heap();
  // printf("%d\n", size);
  while ((void *)curMeta < endOfHeap) {
    // printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    // printf("sumOfSizes %d \n", curMeta->size + sizeof(metadata_t));
     if(size + sizeof(metadata_t) <= curMeta->size && curMeta->isUsed == 0) { // free and big space
     // printf("Splitting \n");
      metadata_t *new_block = (void *)curMeta + sizeof(metadata_t) + size;
      memcpy(new_block, curMeta, sizeof(metadata_t));
      new_block->size = curMeta->size - sizeof(metadata_t) - size;
      new_block->isUsed = 0;
      curMeta->isUsed = 1;
      curMeta->size = size;
      return (void *)new_block + sizeof(metadata_t);

    } else if(size <= curMeta->size && curMeta->isUsed == 0) {
      //printf("Overiding current block size instead \n");
      curMeta->isUsed = 1;
      curMeta->size = size;
      return (void *)curMeta + sizeof(metadata_t);
    }
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  } 
  return NULL;
}

void *split_lists(size_t size) {
  metadata_t *curMeta = head;
  //void *end = sbrk(0);
  print_heap();
  // printf("-- Start of Heap (%p) --\n", startOfHeap);
  while (curMeta != NULL ) {
    // printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    // printf("sumOfSizes %d \n", curMeta->size + sizeof(metadata_t));
     if(size + sizeof(metadata_t) <= curMeta->size && curMeta->isUsed == 0) { // free and big space
     // printf("Splitting \n");
      metadata_t *new_block = (void *)curMeta + sizeof(metadata_t) + size;
      memcpy(new_block, curMeta, sizeof(metadata_t));
      new_block->size = curMeta->size - sizeof(metadata_t) - size;
      new_block->isUsed = 0;
      curMeta->isUsed = 1;
      curMeta->size = size;
      return (void *)new_block + sizeof(metadata_t);

    } else if(size <= curMeta->size && curMeta->isUsed == 0) {
      //printf("Overiding current block size instead \n");
      curMeta->isUsed = 1;
      curMeta->size = size;
      metadata_t *temp = curMeta->next_ptr;
      curMeta = curMeta->prev_ptr;
      curMeta->next_ptr = temp;
      curMeta = curMeta->prev_ptr;
      curMeta->next_ptr = NULL;
      return (void *)curMeta + sizeof(metadata_t);
    }
    curMeta = curMeta->next_ptr;
  } 
  return NULL;
}

void *split_lists2(size_t size) {
  metadata_t *curMeta = head;
  for(curMeta = head; curMeta != NULL; curMeta = curMeta->next_ptr) {

    if(size + sizeof(metadata_t) <= curMeta->size && curMeta->isUsed == 0) {
      metadata_t *newMeta = (void *)curMeta + size + sizeof(metadata_t);
      memcpy(newMeta, curMeta, sizeof(metadata_t));
      newMeta->size = curMeta->size - size - sizeof(metadata_t);
      newMeta->isUsed = 0; // allocated used block is at the bottom of split block
      curMeta->isUsed = 1;
      curMeta->size = size;
      //remove and update from list
      if(curMeta->prev_ptr != NULL) {
        metadata_t * y = curMeta->prev_ptr;
        y->next_ptr = newMeta;
        newMeta->prev_ptr = y;
      }
      if(curMeta->next_ptr != NULL) {
        newMeta->next_ptr = curMeta->next_ptr;
        metadata_t * t = curMeta->next_ptr;
        t->prev_ptr = newMeta;
      }
      if(curMeta == head) { //edge case
        head = newMeta;
      }
      return (void*)curMeta +sizeof(metadata_t);
    } else if (size <= curMeta->size && curMeta->isUsed == 0) {
      // printf("Overiding current block size instead \n");
      curMeta->isUsed = 1;
      curMeta->size = size;
      metadata_t *temp = curMeta->next_ptr;
      if(curMeta->prev_ptr != NULL) {
        metadata_t * t = curMeta->prev_ptr;
        t->next_ptr = curMeta->next_ptr; 
      }
      if(curMeta->next_ptr!=NULL) {
        temp->prev_ptr = curMeta->prev_ptr;
      }

      if(head == curMeta) { // edge case
        if(curMeta->next_ptr != NULL) {
          head = temp;
          head->prev_ptr =  NULL;
        } else {
          head = NULL;
        }
      }
      return (void*) curMeta + sizeof(metadata_t); // curMeta is referring to block no longer in list
    }
  } 
  return NULL;
}

void print_heap() {
  //printf("Inside: malloc(%lu):\n", size);
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  printf("-- Start of Heap (%p) --\n", startOfHeap);
  while ((void *)curMeta < endOfHeap) {   // While we're before the end of the heap...
    printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    curMeta = (void *)curMeta + curMeta->size + sizeof(metadata_t);
  }
  printf("-- End of Heap (%p) --\n\n", endOfHeap);
}

void print_list() {
  metadata_t *curMeta = head;
  void *endOfHeap = sbrk(0);
  printf("-- Start of list (%p) --\n", head);
  // printf("curMeta addy %p\n", (void *)curMeta);
  while (curMeta->next_ptr != NULL) {   // While we're before the end of the heap...
    printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    curMeta = curMeta->next_ptr;
  }
  printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
  printf("-- End of list (%p) --\n\n", curMeta);
  printf("Backwards traversal: \n");
  while (curMeta->prev_ptr != NULL) {   // While we're before the end of the heap...
    printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    curMeta = curMeta->prev_ptr;
  }
  printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
  printf("-- End of list (%p) --\n\n", curMeta);
  printf("Helloooo");
  //print_heap();
}


void insert_front(metadata_t *meta_ptr) {  // insert new encountered free block to the head of list
  metadata_t *curMeta = meta_ptr;
  //printf("Inside insert_front \n");
  if(curMeta->isUsed == 0){
    if(head == NULL) { // empty list
      head = curMeta;
      //printf("First element in list \n");
    } else {
      curMeta->next_ptr = head;
      head->prev_ptr = curMeta; 
      head = curMeta;
    }
  }
  // printf("metadata for next_ptr memory %p :\n ", curMeta->next_ptr);
  // printf("metadata for prev_ptr memory %p : \n", curMeta->prev_ptr);
  // print_list();
  // printf(" \n");
}

void insert_behind(metadata_t *meta_ptr) {  // insert new encountered free block to the head of list
  metadata_t *curMeta = meta_ptr;
  if(head == NULL) { // empty list
    head = curMeta;
    tail = curMeta;
   // printf("First element in list \n");
  } else {
    tail->next_ptr = curMeta;
    curMeta->prev_ptr = tail; 
    tail = curMeta;
    }
  
  // printf("metadata for next_ptr memory %p :\n ", curMeta->next_ptr);
  // printf("metadata for prev_ptr memory %p : \n", curMeta->prev_ptr);
  // print_list();
  // printf("metadata for head memory %p : \n", head);
  // printf("metadata for tail memory %p : \n", tail);
  // printf(" \n");
}

void combine_properly() {
  metadata_t *curMeta = startOfHeap;
  void *endOfHeap = sbrk(0);
  unsigned int sizeTotal = 0;
  int num_meta = 0;
  while ((void *)curMeta < endOfHeap) {   
    //printf("metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta + sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    if(curMeta->isUsed == 0) {
      metadata_t *secondMatch = (void *)curMeta + sizeof(metadata_t) + curMeta->size;
      while((void *)secondMatch < endOfHeap) {
        if(secondMatch->isUsed == 0) {
           sizeTotal += secondMatch->size;
           num_meta ++;
        }
        secondMatch = (void *)secondMatch + sizeof(metadata_t) + secondMatch->size;
      }
      curMeta->size = sizeTotal + num_meta * sizeof(metadata_t);
    }
    curMeta = (void *)curMeta + sizeof(metadata_t) + curMeta->size; //check to see if points to the next right meta
  }
}

void memory_coalesce() {
  metadata_t * curMeta;
  metadata_t *endOfHeap = sbrk(0);
  metadata_t * iterator;
  for(curMeta = head; curMeta != NULL; curMeta = curMeta->next_ptr) {
    metadata_t *adjacent = (void *)curMeta + curMeta->size + sizeof(metadata_t);
    if(adjacent < endOfHeap) {
      for(iterator = head; iterator != NULL; iterator = iterator->next_ptr) {
        if((void *) iterator == adjacent) {
          curMeta->size = curMeta->size + iterator->size + sizeof(metadata_t);
          metadata_t * t = iterator->next_ptr;
          if(iterator->next_ptr != NULL) {
            t->prev_ptr = iterator->prev_ptr;
          }
          if(iterator->prev_ptr != NULL) {
            metadata_t * a = iterator->prev_ptr;
            a->next_ptr = iterator->next_ptr; 
          }
          if(iterator == head) {
            head = t;
            head->prev_ptr =  NULL;
          }
          break;
        } 
      }
    }
  }
}


void free_list() {
  metadata_t *curMeta = startOfHeap;
  // printf("Inside free_list \n");
  void *endOfHeap = sbrk(0);
  // printf("curMeta addy %p\n", (void *)curMeta);
  // printf("end %p\n", endOfHeap);
  while ((void *)curMeta < endOfHeap) {    
    //printf("Metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta+ sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    // printf("LINE 204 \n");
    if(curMeta->isUsed == 0) {
      //printf("Current Block is free, finding next one \n");
      metadata_t *secondMatch = (void *)curMeta + sizeof(metadata_t) + curMeta->size;
      while((void *)secondMatch < endOfHeap) {
        if(secondMatch->isUsed == 0) {
          //printf("Found another free block, setting next_ptr");
          curMeta->next_ptr = secondMatch;
          curMeta = secondMatch;
          break;
        } else {
          secondMatch = (void *)secondMatch + sizeof(metadata_t) + secondMatch->size;
        }
      }
    } else {
      curMeta = (void *)curMeta + sizeof(metadata_t) + curMeta->size;
    }
  }
  //curMeta->next_ptr = tail;
}

void free_list2(void *meta_ptr) {
  metadata_t *curMeta = meta_ptr;
  // printf("Inside free_list \n");
  void *endOfHeap = sbrk(0);
  // printf("curMeta addy %p\n", (void *)curMeta);
  // printf("end %p\n", endOfHeap);
  while ((void *)curMeta < endOfHeap) {    
    //printf("Metadata for memory %p: (%p, size=%d, isUsed=%d)\n", (void *)curMeta+ sizeof(metadata_t), curMeta, curMeta->size, curMeta->isUsed);
    // printf("LINE 204 \n");
    if(curMeta->isUsed == 0) {
      //printf("Current Block is free, finding next one \n");
      metadata_t *secondMatch = (void *)curMeta + sizeof(metadata_t) + curMeta->size;
      while((void *)secondMatch < endOfHeap) {
        if(secondMatch->isUsed == 0) {
          //printf("Found another free block, setting next_ptr");
          curMeta->next_ptr = secondMatch;
          curMeta = secondMatch;
          break;
        } else {
          secondMatch = (void *)secondMatch + sizeof(metadata_t) + secondMatch->size;
        }
      }
    } else {
      curMeta = (void *)curMeta + sizeof(metadata_t) + curMeta->size;
    }
  }
  curMeta->next_ptr = tail;
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

  //combine_properly(); // better in free or malloc?
  //free_list3(ptr); // adds to linked list
  insert_front(meta);
  memory_coalesce();
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
    //printf("realloc called \n");
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