#include "http.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>

void *client_thread(void *vptr) {
  int fd = *((int *)vptr);
  HTTPRequest *req = malloc(sizeof(HTTPRequest));
  ssize_t read = httprequest_read(req, fd);
  printf("LINE 20 \n");
  printf("read: %u \n", read);
  // if(read <= 0) {
  //   printf("here\n");
  //   fclose(fd);
  //   return NULL;
  // }
  printf("path: %s\n", req->path);
  // if(strcmp(req->path, "/") == 0) {
  //   req->path = "/index.html";
  // }
  //printf("here\n");
  
  //char static_path = malloc(strlen(req->path) + 10);
  char content = "text/html";
  if(strstr(req->path, ".png") != NULL) {
    content = "image/png";
  }
  // if file does not exist, return 404 error else return 200 ok + others
  FILE *file;
  char fullPath[1024];
  sprintf(fullPath, "static%s", req->path);
  file = fopen(fullPath, "r");
  if(file == NULL) {
    printf("http/1.1 404 Not Found \n");
  } else {
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    void *ptr = malloc(size);
    fread(ptr, size, 1, file);
    fclose(file);
    printf("200 OK \n");
    //char response = "http/1.1: 200 OK Content-Type %s";
    char response [1024];
    sprintf(response, "http/1.1: 200 OK \r\n Content-Type %d \r\n Content-Length %d \r\n\r\n %s", content, size, req->head->key);
    // payload
    printf("%s\n", "LINE 52");
    printf("%s\n", response);
    printf(req->head->key);
    printf("Done");
    write(fd, response , strlen(response));
    //fclose(fd);

  }
  fclose(fd);
  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: %s <port>\n", argv[0]);
    return 1;
  }
  int port = atoi(argv[1]);
  printf("Binding to port %d. Visit http://localhost:%d/ to interact with your server!\n", port, port);

  // socket:
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // bind:
  struct sockaddr_in server_addr, client_address;
  memset(&server_addr, 0x00, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);  
  bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr));

  // listen:
  listen(sockfd, 10);

  // accept:
  socklen_t client_addr_len;
  while (1) {
    int *fd = malloc(sizeof(int));
    *fd = accept(sockfd, (struct sockaddr *)&client_address, &client_addr_len);
    printf("Client connected (fd=%d)\n", *fd);

    pthread_t tid;
    pthread_create(&tid, NULL, client_thread, fd);
    pthread_detach(tid);
  }

  return 0;
}