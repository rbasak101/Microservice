#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wallet.h"
pthread_cond_t cond;
/**
 * Initializes an empty wallet.
 */
void wallet_init(wallet_t *wallet) {
  // Implement
  //printf("hello \n");
  wallet->head = NULL;
  pthread_mutex_init(&(wallet->lock), NULL);
  //pthread_mutex_unlock(&(wallet->lock));
  pthread_cond_init(&cond, NULL);
}



/**
 * Returns the amount of a given `resource` in the given `wallet`.
 */
int wallet_get(wallet_t *wallet, const char *resource) {
  // Implement this
 // printf("LINE 25 \n");
  wallet_resource *iterate = wallet->head;
  for(iterate = wallet->head; iterate != NULL; iterate = iterate->next) {
    if(strcmp(iterate->resource_name, resource) == 0) {
      return iterate->amount;
    }
  }
  return 0;
}

/**
 * Modifies the amount of a given `resource` in a given `wallet by `delta`.
 * - If `delta` is negative, this function MUST NOT RETURN until the resource can be satisfied.
 *   (Ths function MUST WAIT until the wallet has enough resources to satisfy the request;
 *    there are several ways to accomplish this waiting and it does not have to be fancy.)
 */
int exists(wallet_t *wallet, const char *resource) {
  // Implement this
  //printf("LINE 25 \n");
  wallet_resource *iterate = wallet->head;
  for(iterate = wallet->head; iterate != NULL; iterate = iterate->next) {
    if(strcmp(iterate->resource_name, resource) == 0) {
      return 1; // True
    }
  }
  return 0; // False
}

void wallet_change_resource(wallet_t *wallet, const char *resource, const int delta) {
  // Implement this
  pthread_mutex_lock(&(wallet->lock));
  // printf("Delta: %d\n", delta);
  wallet_resource *iterator;
  // printf("LINE 59 \n");
  // printf("%p\n", wallet->head);
  if(exists(wallet, resource) == 0) {
    if(wallet->head == NULL) { // first resource
    //  printf("Inserting first item \n");
      wallet_resource *item = malloc(sizeof(wallet_resource));
     // printf("%s \n", resource);
      item->resource_name = resource;
     // printf("copied name \n");
      item->amount = 0; // or 0
      item->next = NULL;
      wallet->head = item;
     // printf("Inserted first item \n");
      } else {
        wallet_resource *item = malloc(sizeof(wallet_resource));
        item->resource_name = resource;
        item->amount = 0; // or 0
        item->next = wallet->head;
        wallet->head = item;
      //  printf("Address of new head: %p\n", wallet->head);
      }
  }

  for(iterator = wallet->head; iterator != NULL; iterator = iterator->next) {
  //  printf("iterating... \n");
    if(strcmp(iterator->resource_name, resource) == 0) {
      // printf("Found resource \n");
      // printf("current amount $$: %d\n", iterator->amount);
      if(delta < 0) {
       // printf("Debt \n");
        
        while(delta + iterator->amount < 0) {
         // printf("Amount: %d\n", iterator->amount);
          pthread_cond_wait(&cond, &(wallet->lock)); // check on the c.v + releases unlocked lock for another thread
        }
        iterator->amount = iterator->amount + delta;
      } else { // if delta > 0
          //printf("current amount: %d\n", iterator->amount);
          iterator->amount = iterator->amount + delta;
          pthread_cond_broadcast(&cond); // refers back to the 1st thread
         // printf("current amount: %d\n", iterator->amount);
      }
    }
  }
    pthread_mutex_unlock(&(wallet->lock));
   // printf("Unlocked \n");
}

/**
 * Destroys a wallet, freeing all associated memory.
 */
void wallet_destroy(wallet_t *wallet) {
  // Implement this
  wallet_resource *current = wallet->head;
  wallet_resource *next = NULL;
  while(current != NULL) { // deleting linked list
    next = current->next;
    free(current);
    current = next;
  }
}