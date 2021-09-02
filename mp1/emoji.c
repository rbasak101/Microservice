#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Return your favorite emoji; do not allocate new memory.
// (This should **really** be your favorite emoji, we plan to use this later in the semester. :))
const char *emoji_favorite() {
  return "\xF0\x9F\x98\x8E"; // sunglasses smiley emoji
}

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
  char *space = " ";
  char *token = strtok(utf8str, space);
  while(token != NULL) {   //  utf8str[i] != '\0'
    emoji_invertChar(token);
    token = strtok(NULL, space);
    
  }
}


// Return a random emoji stored in new heap memory you have allocated.  Make sure what
// you return is a valid C-string that contains only one random emoji.
char *emoji_random_alloc() {
// (rand() % (upper - lower + 1)) + lower;
  /*   U+1F000 - U+1FAFF  
  \x80  (128) <= Index 2 <= \xAB [128 - 171]
  \x80 <= index 3 <= \xBF [128 - 191]
  */
  // char hex_characters[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  char *emoji = malloc(5 * sizeof(char)); // Generating the three hex values U+1F_ _ _
  //emoji[0] = 0xF0; 
  emoji[0] = '\xF0'; 
  emoji[1] = '\x9F'; 
  // char rand1 = hex_characters[(rand() % 3) + 8]; // 8, 9, A --> index 8,9,10
  // // char rand2 = hex_characters[rand() % 8 + 0]; // 0 to 7 --> index 0 to 7
  // char rand2 = hex_characters[(rand() % 12)]; // 0 to B --> index 0 to 11 // 
  // // emoji[2] = '\\x' + rand1 + rand2; // \xA7 // three different bytes  
  emoji[2] = (rand() % (171-128+-1)) + 128; //   emoji[2] = (rand() % 44) + 128;
  emoji[3] = (rand() % (191-128+1)) + 128;
  emoji[4] = '\0';
  return emoji;
}
