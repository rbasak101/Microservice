#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "crc32.h"
#include "png.h"

const int ERROR_INVALID_PARAMS = 1;
const int ERROR_INVALID_FILE = 2;
const int ERROR_INVALID_CHUNK_DATA = 3;
const int ERROR_NO_UIUC_CHUNK = 4;


/**
 * Opens a PNG file for reading (mode == "r" or mode == "r+") or writing (mode == "w").
 * 
 * (Note: The function follows the same function prototype as `fopen`.)
 * 
 * When the file is opened for reading this function must verify the PNG signature.  When opened for
 * writing, the file should write the PNG signature.
 * 
 * This function must return NULL on any errors; otherwise, return a new PNG struct for use
 * with further fuctions in this library.
 * 
 * PNG File Signature: First 8 bytes of PNG always have decimal values: 137 80 78 71 13 10 26 10
 * or hex values: 89	50	4E	47	0D	0A	1A	0A
 */
PNG * PNG_open(const char *filename, const char *mode) { // filename = 0x54 0x45 0x53 0x54
  //char input = *mode;
  FILE *file;
  file = fopen(filename, mode);
  unsigned char *buffer = malloc(8);
  if(strcmp(mode, "r") ==0 || strcmp(mode, "r+") == 0) {   // input == "r+"  || input == 'r'
    // open file, verify PNG signature
    // FILE *file;
    // file = fopen(filename, mode);
    if(file != NULL) { // check if file exists
      fread(buffer, sizeof(char), 8, file);
      if(buffer[0] == 0x89 && buffer[1] == 0x50 && buffer[2] == 0x4e && buffer[3] == 0x47 && buffer[4] == 0x0d &&
         buffer[5] == 0x0a && buffer[6] == 0x1a && buffer[7] == 0x0a) {
            PNG *png = malloc(sizeof(PNG));
            //printf("Entered 2");
            png->file = file;
            return png;
      }
    } 
  } else if(strcmp(mode, "w") == 0 && file != NULL) { 
    printf("Entered");
    //const unsigned char *buffer2[8] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
    // char *buffer2[8] = {'\x89', '\x50', '\x4e', '\x47', '\x0d', '\x0a', '\x1a', '\x0a'};
    // printf(sizeof(buffer2));
    buffer[0] = 0x89;
    buffer[1] = 0x50;
    buffer[2] = 0x4e;
    buffer[3] = 0x47;
    buffer[4] = 0x0d;
    buffer[5] = 0x0a;
    buffer[6] = 0x1a;
    buffer[7] = 0x0a;
    fwrite(buffer, sizeof(char), sizeof(buffer), file);
    fclose(file); // NEED TO INCLUDE THIS WHEN WRITING!!!
    PNG *png = malloc(sizeof(PNG)); 
    png->file = file;
    return png;
  }
  return NULL;
}


/**
 * Reads the next PNG chunk from `png`.
 * 
 * If a chunk exists, a the data in the chunk is populated in `chunk` and the
 * number of bytes read (the length of the chunk in the file) is returned.
 * Otherwise, a zero value is returned.
 * 
 * Any memory allocated within `chunk` must be freed in `PNG_free_chunk`.
 * Users of the library must call `PNG_free_chunk` on all returned chunks.
 */
size_t PNG_read(PNG *png, PNG_Chunk *chunk) { // populate chunk data from png to chunk + return number of bytes of the data; 
// Read the four byte chunk for length | then for type | then allocate the length for that chunk 
// read the four byte crc | then return the number bytes of data length: 4 ifs
  size_t size = 0;
  // unsigned char *buffer = malloc(4);
  // FILE *file = png->file;
  // if(chunk != NULL) {
  //   size += fread(chunk, 1, sizeof(uint32_t), file);
  //   // size += fread(chunk->type, 1, sizeof(char), file);
  //   // size += fread(chunk->data, 1, sizeof(char), file);
  //   // size += fread(chunk->crc, 1, sizeof(uint32_t ), file);
  //   printf("Entered read");
  //   printf(size);
  // }

  return size;
}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written.   As part of writing the `chunk`, you must calculate the CRC
 * based on the other data in the `chunk`; do not assume the CRC given has been calculated for you.
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {
  return 0;
}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  free(chunk);
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  // fclose(png);
  // free(png);
}
