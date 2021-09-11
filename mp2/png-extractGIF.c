#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>
#include <errno.h>

int png_extractGIF(const char *png_filename, const char *gif_filename) {
  // argv[1] is png_filename; extract uiuc chunk from png_filename;   Then store it in gif_filename
   PNG *png = PNG_open(png_filename, "r");
   //PNG *png_gif = PNG_open(gif_filename, "w");
  // traverse chunks till you reach uiuc or IEND
  // bool running = true;
  int running = 1;
  size_t byte_chunk = 0;
  while(running) {
    PNG_Chunk chunk;
    byte_chunk = PNG_read(png, &chunk);
    if(byte_chunk == 0) { // invalid
      PNG_close(png);
      return ERROR_INVALID_CHUNK_DATA;
    }

    if(strcmp(chunk.type, "uiuc") == 0 ) {
      // unsigned char *buffer = malloc(12 + chunk.len);
      printf("%d\n", 25);
      printf("%d\n", byte_chunk); // read 633438 bytes | chunk.len = 633426 (of data)
      printf("%d\n", chunk.len);
      FILE *gif;
      gif = fopen(gif_filename, "w+");
      size_t written = 0; // uint32_t written = 0;
      printf("%p\n", &chunk);
      printf("pointer address: %p\n", &(chunk.data));
      printf("data pointer address: %p\n", chunk.data);
      if(gif != NULL) {
        //printf("%p\n", &gif);
        printf("size %u\n", sizeof(PNG_Chunk));
        written += fwrite(chunk.data, 1, chunk.len, gif);
        fclose(gif);
        printf("%d\n", written);
        printf("%d\n", 36);
      }
      break;  
    }

    if(strcmp(chunk.type, "IEND") == 0 ) {
      running = 0;
      break;
    }
  }
  return 0;  // Change the to a zero to indicate success, when your implementaiton is complete.
}


int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF Name>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_extractGIF(argv[1], argv[2]);
}
