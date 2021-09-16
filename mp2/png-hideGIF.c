#include <stdio.h>
#include <stdlib.h>
#include "lib/png.h"
#include <string.h>

int png_hideGIF(const char *png_filename, const char *gif_filename) {
// insert gif_filename into png_filename; right after IHDR chunk for ease
  PNG *png = PNG_open(png_filename, "w"); 
  FILE *gif_file;
  gif_file = fopen(gif_filename, "r");

  size_t byte_chunk = 0;
  size_t written = 0;
  int chunk_count = 1;
  long int size = 0;
  while(1) {
     PNG_Chunk chunk;
     byte_chunk = PNG_read(png, &chunk);

    if(strcmp(chunk.type, "IHDR")) {
   // printf("%d\n", sizeof(png->file));
    // long size = ftell(png->file);
    // printf("%d\n", size);
    //if(chunk_count == 3) {
       PNG_Chunk uiuc_chunk;
       // uiuc_chunk.len = sizeof(FILE); // correct size?
       // uiuc_chunk.len = sizeof(gif_file); // correct size?
       fseek(gif_file, 0L,  SEEK_END);
       size = ftell(gif_file);
       fseek(gif_file, 0L,  SEEK_SET);
       uiuc_chunk.len = size;
       uiuc_chunk.type[0] = 'u';
       uiuc_chunk.type[1] = 'i';
       uiuc_chunk.type[2] = 'u';
       uiuc_chunk.type[3] = 'c';
       uiuc_chunk.type[4] = '\0';
      
       uiuc_chunk.data = malloc(uiuc_chunk.len);
       fread(uiuc_chunk.data, 1, uiuc_chunk.len, gif_file);
       
       printf("Finished \n");
       printf("%d\n", sizeof(FILE));
       printf("%d\n", written);
       printf("%d\n", sizeof(gif_file));
       printf("%d\n", sizeof(png->file));
       written += PNG_write(png, &uiuc_chunk);
       printf("%d\n", written);
       //printf("%d\n", ftell(png->file));
       PNG_close(png);
       //free(uiuc_chunk.data);
       fclose(gif_file);
       break;
     }
    printf("Finished \n");
    chunk_count += 1;
  }
  return 0;  // Change the to a zero to indicate success, when your implementaiton is complete.
}

int main(int argc, char *argv[]) {
  // Ensure the correct number of arguments:
  if (argc != 3) {
    printf("Usage: %s <PNG File> <GIF File>\n", argv[0]);
    return ERROR_INVALID_PARAMS;
  }

  return png_hideGIF(argv[1], argv[2]);
}
