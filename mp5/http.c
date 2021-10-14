#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "http.h"

char *token = NULL;
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
ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) { //test case failing: len = 92
  printf("%d\n", buffer_len);
  size_t size = 0;
  char *rest = strdup(buffer);
  req->action = strtok_r(rest, " ", &token);


  size += strlen(req->action);
  size += 1;
  req->path = strtok_r(NULL, " ", &token);
  size += strlen(req->path);
  size += 1;
  char *chunk = NULL;
  chunk = strtok_r(NULL, "\r\n", &token);
  req->version = chunk;
  size += strlen(req->version);
  size += 2;  // action, path, version, 

  char *temp_key, *temp_val, *k, *v = NULL;  
  printf("%d\n", size); // 16
  int first_node = 1;
  while(chunk != NULL) { // parsing key:values
    chunk = strtok_r(NULL, "\r\n", &token);
    if(chunk == NULL) {
      break;
    }
    printf("%s\n", chunk);
    size += strlen(chunk);
    Node *item = malloc(sizeof(Node));  // populating k:v
    k = strtok_r(chunk, ": ", &temp_key);

    if(k == NULL) {
      break;
    }
    item->key = k;
    //memcpy(item->key, &buffer, 4);
    // printf("item->key: ");
    // printf("%s\n", item->key);
    v = strtok_r(NULL, " ", &temp_key);
    if(v == NULL) {
      item->value = "payload";
    } else {
      item->value = v;
    }
    // printf("item->value: ");
    // printf("%s\n", item->value);
    if(req->head == NULL || first_node == 1) { // creating linked list *** req->head first time has random address, not NULL
      req->head = item;
      item->next = NULL;
      first_node = 0;
    } else {
      printf("More than 1 \n");
      item->next = req->head;
      req->head = item;
    }
    printf("\n");
  }
  // char *the_rest = strdup(buffer);
  // char *pay = strtok_r(the_rest, "\r\n\r\n", &token);
  // printf("%s\n", pay);
  // char answer[23];
  // int i = 0;
  // while(pay != NULL) {
  //   if(strstr(pay, "Content-Length") != NULL) {
  //     printf("%s\n", pay);
  //     //pay = strtok_r(NULL, "", &token);
  //     for(int i = 23; i > 0; i--) {
  //       //printf("%c\n", buffer[buffer_len - i]);
  //       char c = buffer[buffer_len - i];
  //       strncat(answer, &c, 1);
  //     }
  //     //printf("%s\n", answer);
  //     brute_force_payload = answer;
  //     req->head->key = answer;
  //     // printf("%s\n", req->head->key);
  //     // printf("%s\n", req->head->value);
  //     break;
  //   }
  //   pay = strtok_r(NULL, "\r\n\r\n", &token);
  // }
  print_list(req);
  return buffer_len;
}
ssize_t httprequest_parse_headers2(HTTPRequest *req, char *buffer, ssize_t buffer_len) {
  size_t size = 0;
  char *rest = strdup(buffer);
  req->action = strtok_r(rest, " ", &token);

  size += strlen(req->action);

  req->path = strtok_r(NULL, " ", &token);
  size += strlen(req->path);

  char *chunk = NULL;
  chunk = strtok_r(NULL, "\r\n", &token);
  req->version = chunk;
  size += strlen(req->version);
  char *temp_key, *temp_val, *k, *v = NULL;

  while(chunk != NULL) { // parsing key:values
    chunk = strtok_r(NULL, "\r\n", &token);
    printf("%s\n", chunk);
    if(chunk == NULL) {
      break;
    }
    size += strlen(chunk);
    Node *item = malloc(sizeof(Node));  // populating k:v
    k = strtok_r(chunk, ": ", &temp_key);

    if(k == NULL) {
      break;
    }
    item->key = k;
    printf("item->key:");
    printf("%s\n", item->key);
    v = strtok_r(NULL, " ", &temp_key);
    if(v == NULL) {
      item->value = "payload";
    } else {
      item->value = v;
    }
    printf("item->value:");
    printf("%s\n", item->value);
    if(req->head == NULL) { // creating linked list
      req->head = item;
    } else {
      item->next = req->head;
      req->head = item;
    }
  }
  return size;
}

void print_list(HTTPRequest *req) {
  printf("Beginning \n");
  Node *iterator;
  for(iterator = req->head; iterator != NULL; iterator = iterator->next) {
    printf("key:");
    printf("%s\n", iterator->key);
    printf("value:");
    printf("%s\n", iterator->value);
    printf("address of node:");
    printf("%p\n", iterator);
    printf("next ptr:");
    printf("%p\n", iterator->next);
    printf("\n");
  }
  printf("End \n");
}

/**
 * httprequest_read
 * 
 * Populate a `req` from the socket `sockfd`, returning the number of bytes read to populate `req`.
 */
ssize_t httprequest_read(HTTPRequest *req, int sockfd) {
  char buffer[4096];
  ssize_t len = read(sockfd, buffer, 4096);
  buffer[len] = '\0';
  ssize_t read = 0;
  read += httprequest_parse_headers(req, buffer,len); // headers, action, path, version; need payload
  // payload is stored in linked list; key: 0123456789, value: "payload"
  if(strcmp(req->head->value, "payload") == 0) {
    //printf("LINE 195 \n");
    //printf("%s\n", req->head->key);
    req->payload = req->head->key;
  } else {
    req->payload = NULL;
  }
  return read;
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