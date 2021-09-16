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
  if(strcmp(mode, "r") == 0|| strcmp(mode, "r+") == 0) {   // input == "r+"  || input == 'r'
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
    //printf("Entered");
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
    //
    PNG *png = malloc(sizeof(PNG)); 
    png->file = file;
    //fclose(file);
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
size_t PNG_read(PNG *png, PNG_Chunk *chunk) { // populate chunk data from png to chunk + return number of bytes of the data;  // PNG to chunk
// Read the four byte chunk for length | then for type | then allocate the length for that chunk read the four byte crc | then return the number bytes of data length: 
  //chunk = malloc(sizeof(PNG_Chunk));               //trial to resolve free_chunk
  size_t length = 0;
  uint32_t *buffer = malloc(4);
  if(png != NULL) {
    FILE *f = png->file;
    if(f != NULL) {
      if(length == 0) {
        //printf("Reading 1st set \n");
        length += fread(buffer, 1, sizeof(uint32_t), f); // read 1st 4 bytes length
        chunk->len = ntohl(*buffer);
        // printf("%d\n", length);
        // printf("%d\n", chunk->len);
        // printf("\n");
      }
      // unsigned int length = (buffer[0]<<24) + (buffer[1]<<16) + (buffer[2] <<8) + (buffer[3] << 4);
      if(length == 4) {
        //printf("Reading 2nd set \n");
        length += fread(chunk->type, 1, sizeof(uint32_t), f);
        //printf("%d\n", length);
        // chunk->type[0] = buffer[0]; 
        // chunk->type[1] = buffer[1];
        // chunk->type[2] = buffer[2];
        // chunk->type[3] = buffer[3];
        chunk->type[4] = '\x0';
        //printf("\n");
        // printf("%c\n", chunk->type[0]);
        // printf("%c\n", chunk->type[1]);
        // printf("%c\n", chunk->type[2]);
        // printf("%c\n", chunk->type[3]);
        // printf("%c\n", chunk->type[4]);
      }
      if(length == 8) {
        //printf("Reading 3rd set \n");
        chunk->data = malloc(chunk->len); // need to allocate blocks of memory to store bytes: prevents the seg fault !!! --> free(chunk->data)
        length += fread(chunk->data, sizeof(unsigned char), chunk->len, f);
        // printf("%d\n", length);
        // printf("\n");
      }
      if(length == chunk->len + 8) {
       // printf("Reading 4th set \n");
        length += fread(&(chunk->crc), 1, sizeof(uint32_t), f);
        //printf("\n");
      }

    }
    return length;
  }

}


/**
 * Writes a PNG chunk to `png`.
 * 
 * Returns the number of bytes written.   As part of writing the `chunk`, you must calculate the CRC
 * based on the other data in the `chunk`; do not assume the CRC given has been calculated for you.
 */
size_t PNG_write(PNG *png, PNG_Chunk *chunk) {  // Chunk to PNG
  // write contents from chunk to png
  // we know length, type, data;
  // Read the 1st 4 bytes of chunk(length) and then write that chunk | read 2nd set then write ...
  size_t written = 0;

  // length bytes:
  //printf("%d\n", chunk->len);
  FILE *f = png->file;
  uint32_t order = htonl(chunk->len);
  //printf("%d\n", written);

  //printf("%d\n", ftell(f));
  fwrite(&order, 1, sizeof(uint32_t), f);   // must follow with fclose after fwrite
  written = sizeof(uint32_t);
  //printf("%d\n", written);
  //fclose(f);

  // type bytes:
  fwrite(chunk->type, sizeof(char), sizeof(uint32_t), f);
  written += sizeof(uint32_t);
  //printf("%d\n", written);

  // // data types:
  fwrite(chunk->data, sizeof(char), chunk->len, f);
  written += chunk->len; // chunk->len vs temp
  //printf("%d\n", written);

  // // CRC calculation + type:
  unsigned char *buffer = malloc((sizeof(uint32_t) + chunk->len));
  memcpy(buffer, chunk->type, sizeof(uint32_t)); // store 4 bytes of chunk->type into buffer
  memcpy(buffer + sizeof(uint32_t), chunk->data, chunk->len);
  uint32_t crc = 0;
  crc32(buffer, chunk->len + sizeof(uint32_t), &crc);
  
  uint32_t new_crc = htonl(crc);
  fwrite(&new_crc, 1, sizeof(uint32_t), f);
  written += sizeof(uint32_t);

  return written;
}


/**
 * Frees all memory allocated by this library related to `chunk`.
 */
void PNG_free_chunk(PNG_Chunk *chunk) {
  // printf("%p\n", &chunk);
  if(chunk != NULL) {
   free(chunk->data);
  }
}


/**
 * Closes the PNG file and frees all memory related to `png`.
 */
void PNG_close(PNG *png) {
  if(png != NULL) {
   fclose(png->file);
   free(png);
  }
}
