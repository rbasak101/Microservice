#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "http.h"

char *token = NULL;
int findNth(char* str, char ch, int N){
    int occur = 0;
    for (int i = 0; i < strlen(str); i++){
        if (str[i] == ch) {
            occur += 1;
        }
        if (occur == N)
            return i;
    }
    return -1;
}
void remove_spaces(char* s) {
    char *d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++); 
}
/**
 * httprequest_parse_headers
 * 
 * Populate a `req` with the contents of `buffer`, returning the number of bytes used from `buf`.
 */
ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) {

  char *rest = strdup(buffer);
  req->action = strtok_r(rest, " ", &token);
  req->path = strtok_r(NULL, " ", &token);
  
  char *chunk = NULL;
  chunk = strtok_r(NULL, "\r\n", &token);
  req->version = chunk;
  char *temp_key, *temp_val, *k, *v = NULL;

  while(chunk != NULL) { // parsing key:values
    chunk = strtok_r(NULL, "\r\n", &token);
    if(chunk == NULL) {
      break;
    }

    Node *item = malloc(sizeof(Node));  // populating k:v
    k = strtok_r(chunk, ": ", &temp_key);

    if(k == NULL) {
      break;
    }
    item->key = k;
   // printf("%s\n", item->key);
    v = strtok_r(NULL, " ", &temp_key);
    if(v == NULL) {
      break;
    }
    item->value = v;
   // printf("%s\n", item->value);
    if(req->head == NULL) { // creating linked list
      req->head = item;
    } else {
      item->next = req->head;
      req->head = item;
    }
  }
  return -1;
}

/**
 * httprequest_read
 * 
 * Populate a `req` from the socket `sockfd`, returning the number of bytes read to populate `req`.
 */
ssize_t httprequest_read(HTTPRequest *req, int sockfd) {
  return -1;
}


/**
 * httprequest_get_action
 * 
 * Returns the HTTP action verb for a given `req`.
 */
const char *httprequest_get_action(HTTPRequest *req) {
  return req->action;
}


/**
 * httprequest_get_action
 * 
 * Returns the value of the HTTP header `key` for a given `req`.
 */
const char *httprequest_get_header(HTTPRequest *req, const char *key) {
  //traverse the linked list
  Node *iterator;
  for(iterator = req->head; iterator != NULL; iterator = iterator->next) {
    if(strcmp(iterator->key, key) == 0){
      return iterator->value;
    }
  }
  return NULL;
}


/**
 * httprequest_get_action
 * 
 * Returns the requested path for a given `req`.
 */
const char *httprequest_get_path(HTTPRequest *req) {
  return req->path;
}


/**
 * httprequest_get_action
 * 
 * Destroys a `req`, freeing all associated memory.
 */
void httprequest_destroy(HTTPRequest *req) {
  free(req->action);
  free(req->path);
  free(req->version);
  free(req->payload);
}