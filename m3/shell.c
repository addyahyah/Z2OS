/*
 * team number: 1D
 * team member: Zhou Zhou, Jake Patterson, Yuzong Gao, Luwen Zhang
*/
#include "./constants.h"
#include "./string.h"

void cleanFilename(char*);

int main()
{
  char buffer[READSTRING_MAX_BUFFER_SIZE];
  char *filename;
  char fileContentBuffer[MAXIMUM_FILE_SIZE];
  while(1) {
    /* Routine buffer cleanup */
    buffer[0] = '\0';
    filename = 0;
    fileContentBuffer[0] = '\0';

    /* Actual code */
    interrupt(0x21, 0, "SHELL> ", 0, 0);
    interrupt(0x21, 1, buffer, 0, 0);
    if (strncmp(buffer, "type ", 5)) {
      filename = buffer + 5;
      cleanFilename(filename);
      interrupt(0x21, 3, filename, fileContentBuffer, 0);
      interrupt(0x21, 0, fileContentBuffer, 0, 0);
    } else if (strncmp(buffer, "execute ", 8)) {
      filename = buffer + 8;
      cleanFilename(filename);
      interrupt(0x21, 4, filename, 0x2000, 0);
    } else if (strncmp(buffer, "exit ", 5)) {
      return 0;
    } else {
      interrupt(0x21, 0, "Bad Command!\r\n", 0, 0);
    }
  }
  return 0;
}

void cleanFilename(char* filename) {
  unsigned int i;
  for (i = 0;;i++) {
    if (filename[i] == '\r') {
      filename[i] = '\0';
      return;
    } else if (filename[i] == '\0') {
      return;
    }
  }
}
