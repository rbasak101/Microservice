#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#include "http.h"


ssize_t httprequest_parse_headers(HTTPRequest *req, char *buffer, ssize_t buffer_len) {
  return -1;
}

ssize_t httprequest_read(HTTPRequest *req, int sockfd) {
  return -1;
}

const char *httprequest_get_action(HTTPRequest *req) {
  return NULL;
}

const char *httprequest_get_header(HTTPRequest *req, const char *key) {
  return NULL;
}

const char *httprequest_get_path(HTTPRequest *req) {
  return NULL;
}

void httprequest_destroy(HTTPRequest *req) {

}