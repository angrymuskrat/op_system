#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void func(char str[]) {
  int i;
  for (i = 0; i < strlen(str); i++) {
    if (islower(str[i]))
      str[i] = toupper(str[i]);
    else
      str[i] = tolower(str[i]);
  }
}
