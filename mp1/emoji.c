#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Return your favorite emoji; do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
const char *emoji_favorite() {
  //return "\x55\x2b\x31\x46\x36\x30\x45";
  return "\xF0\x9F\x98\x8E"; // sunglasses smiley emoji
  //return "";
}

// char* substr(const char *src, int m, int n)
// {
//     // get the length of the destination string
//     int len = n - m;
//     // allocate (len + 1) chars for destination (+1 for extra null character)
//     char *dest = (char*)malloc(sizeof(char) * (len + 1));
//     // extracts characters between m'th and n'th index from source string and copy them into the destination string
//     for (int i = m; i < n && (*(src + i) != '\0'); i++){
//         *dest = *(src + i);
//         dest++;
//     }
//     // null-terminate the destination string
//     *dest = '\0';
//     // return the destination string
//     return dest - len;
// }

int substring_count(char* string, char* substring) {
  int count = 0;
  for(int i = 0; i < strlen(string) - strlen(substring) + 1; i++) {
    if(strstr(string + i, substring) == string + i) {
      count++;
      i = i + strlen(substring) -1;
    }
  }
  return count;
}

// Count the number of emoji in the UTF-8 string `utf8str`, returning the count.  You should
// consider everything in the ranges starting from (and including) U+1F000 up to (and including) U+1FAFF.
int emoji_count(char *utf8str) { 
  // return count + 1;
  // int count = 0;
  // for (int i = 0; i + 3 < strlen(utf8str); i++) {
  //     //char *sub = strstr("\xF0", &utf8str);
  //     char *target = "\xF0";
  //     if(strcmp(substr(utf8str, i, i+3), target) != 0) {
  //       count ++;
  //       printf(substr(utf8str, i, i+3));
  //     }
  // }
  // return count + 1;
  char *substring = "\xF0";
  return substring_count(utf8str, substring);
}


// Modify the UTF-8 string `utf8str` to invert ONLY the FIRST UTF-8 character (which may be up to 4 bytes)
// in the string if it the first character is an emoji.  At a minimum:
// - Invert "😊" U+1F60A ("\xF0\x9F\x98\x8A") into a non-simpling face.
// - Choose at least five more emoji to invert.
void emoji_invertChar(char *utf8str) {
  // "\xF0\x9F\x98\x8A" (smiley face) to \xF0\x9F\x98\x9E 😞
  if(strcmp(utf8str, "\xF0\x9F\x98\x8A") == 0) {
   // *utf8str = "\xF0\x9F\x98\x9E";
    char *a = "\xF0\x9F\x98\x9E";
    strcpy(utf8str, a);
  }
  // 😂 \xF0\x9F\x98\x82 to 😭 \xF0\x9F\x98\xAD
  else if(strcmp(utf8str, "\xF0\x9F\x98\x82") == 0) {
    // *utf8str = "\xF0\x9F\x98\xAD";
    char *a = "\xF0\x9F\x98\xAD";
    strcpy(utf8str, a);
  }
  // 🙈	\xF0\x9F\x99\x88 to 🙉  \xF0\x9F\x99\x89
  else if(strcmp(utf8str, "\xF0\x9F\x99\x88") == 0) {
    // *utf8str = "\xF0\x9F\x99\x89";
    char *a = "\xF0\x9F\x99\x89";
    strcpy(utf8str, a);
    
  }
  // 😷 \xF0\x9F\x98\xB7 to 😵  \xF0\x9F\x98\xB5
  else if(strcmp(utf8str, "\xF0\x9F\x98\xB7") == 0) {
   // *utf8str = "\xF0\x9F\x98\xB5";
    char *a = "\xF0\x9F\x98\xB5";
    strcpy(utf8str, a);
    
  }
  // 😡 \xF0\x9F\x98\xA1 to  😨 \xF0\x9F\x98\xA8 
  else if(strcmp(utf8str, "\xF0\x9F\x98\xA1") == 0) {
    // *utf8str = "\xF0\x9F\x98\xA8";
    char *a = "\xF0\x9F\x98\xA8";
    strcpy(utf8str, a);
  }
  // 😌 \xF0\x9F\x98\x8C to 😔 \xF0\x9F\x98\x94
  else if(strcmp(utf8str, "\xF0\x9F\x98\x8C") == 0) {
    // *utf8str = "\xF0\x9F\x98\xA8";
    char *a = "\xF0\x9F\x98\x94";
    strcpy(utf8str, a);
  }
  // 😒 \xF0\x9F\x98\x92, 😌 \xF0\x9F\x98\x8C
 else {
  // *utf8str = "\xF0\x9F\x98\x8F";
    char *a = "\xF0\x9F\x98\x8C";
    strcpy(utf8str, a);
 } 
}


// Modify the UTF-8 string `utf8str` to invert ALL of the character by calling your
// emoji_invertChar` function on each character.
void emoji_invertAll(char *utf8str) {
// 💖 💻 🎉 😊
  // char *t;
  // char *space = " ";
  // for(int i = 0; i < strlen(utf8str); i++) {   //  utf8str[i] != '\0'
  //   t = strtok(utf8str, space);
  //   printf(t);
  //   emoji_invertChar(t);
  //   //printf(t);
  // }
  char *space = " ";
  char *token = strtok(utf8str, space);
  while(token != NULL) {   //  utf8str[i] != '\0'
    emoji_invertChar(token);
    //printf(token);
    token = strtok(NULL, space);
    
  }
}


// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc() {

  char *emoji = malloc(100);
  int rand1 = rand() % 10 + 48; // U+1F000 to U+1F9FF
  int rand2 = rand() % 23 + 48;
  int rand3 = rand() % 23 + 48;
  strcpy(emoji, "\xF0\x9F\x98\xB7");
  return emoji;

  // char *emoji = malloc(100);
  // emoji[0] = 49;
  // emoji[1] = 102;
  // emoji[2] = rand() % 10 + 48;
  // emoji[3] = rand() % 55 + 48;
  // // int rand1 = rand() % 10 + 48; // U+1F000 to U+1F9FF
  // // int rand2 = rand() % 23 + 48;
  // // int rand3 = rand() % 23 + 48;
  // //strcpy(emoji, "\xF0\x9F\x98\xB7");
  // return emoji;
  //return NULL;
}
