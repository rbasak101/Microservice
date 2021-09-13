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
  while(1) {
     PNG_Chunk chunk;
     byte_chunk = PNG_read(png, &chunk);

     if(strcmp(chunk.type, "IHDR")) {
       PNG_Chunk uiuc_chunk;
       uiuc_chunk.len = sizeof(FILE); // correct size?
       uiuc_chunk.type[0] = 'u';
       uiuc_chunk.type[1] = 'i';
       uiuc_chunk.type[2] = 'u';
       uiuc_chunk.type[3] = 'c';
       uiuc_chunk.type[4] = '\0';

       uiuc_chunk.data = &gif_file;
       printf("Finished \n");
       printf("%d\n", sizeof(FILE));
       printf("%d\n", written);
       // written += fwrite(&uiuc_chunk, 1, sizeof(PNG_Chunk), png->file);
       written += PNG_write(png, &uiuc_chunk);
       printf("%d\n", written);
       break;
     }
    // if(strcmp(chunk.type, "uiuc") == 0 ) {
    //   fwrite(gif_file, 1, sizeof(FILE), png->file);
    //   break;  
    // }
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
