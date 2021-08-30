#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Modify the string `s` to capitalize the first letter of the string if the first letter of `s`
// is lower case.  Otherwise, do nothing.
void capitalize(char *s) {
  char first = s[0];
  int decimal = (int)first;
  if(decimal >= 97 && decimal <= 122) {
    int capitalDecimal = decimal - 32;
    *s = capitalDecimal;
  }
}

void capitalize2(char *s, int i) {
  char first = s[i];
  int decimal = (int)first;
  if(decimal >= 97 && decimal <= 122) {
    int capitalDecimal = decimal - 32;
    s[i] = capitalDecimal;
  }
}

// Modify the string `s` to capitalize the ALL of the lower case letters in `s`.  If the letter
// is not lower case, do nothing.  (You can assume you will be given only ASCII characters.)
void capitalizeAll(char *s) {
  for(int i = 0; i <= strlen(s); i++) {
    capitalize2(s, i);
    //printf(s);
  }
}

// Return a new string (allocated in heap memory that you allocate) that transformed `s` in
// the same way as `capitalizeAll`.  (In this function `s` cannot be modified directly and
// must be done in the new memory you will allocate.)
char *capitalizeAll_alloc(const char *s) {
  char *t = malloc(strlen(s)+1);
  //strcpy(t, s);
  strcpy(t, s);
  capitalizeAll(t);
  //return NULL;
  return t;
}
