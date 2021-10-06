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
    char* d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++); // stackoverflow
}

/**
 * httprequest_parse_headers
 * 
 * Populate a `req` with the contents of `buffer`, returning the number of bytes used from `buf`.
 */
ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) {

  //int first_index = findNth(buffer, "/", 1);

  // remove_spaces(buffer);
  // char *token;
  // char *rest = buffer;
  // req->action = strtok(rest, "/");
  // req->path = "/";
  // printf("%s\n", req->path);
  // req->version = strtok(NULL, "\r");
  // printf("%s\n", req->version);

  // char *token;
  // char *rest = buffer;
  // req->action = strtok(rest, " ");
  // size_t length = strlen(req->action);
  // req->path = strtok(NULL, " ");
  // printf("%s\n", req->path);
  // req->version = strtok(NULL, "\r");
  // printf("%s\n", req->version);


  char *rest = buffer;
  req->action = strtok_r(rest, " ", &token);
  size_t length = strlen(req->action);
  req->path = strtok_r(NULL, " ", &token);
 // printf("%s\n", req->path);
  req->version = strtok_r(NULL, "\r", &token);
//  printf("%s\n", req->version);

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
  char *decoy;
  decoy = strtok_r(NULL, " ", &token);
  printf("LINE 97 \n");
  printf("%s\n", decoy);

  char *answer;
  answer = strtok_r(NULL, "\r", &token);
  printf("LINE 97 \n");
  printf("%s\n", answer);

  return answer;
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