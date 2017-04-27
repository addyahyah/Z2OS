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
  char *filename1, *filename2, *filename3, *filename4;
  char fileContentBuffer[MAXIMUM_FILE_SIZE];
  char debug[2];
  debug[1] = '\0';
  while(1) {
    /* Routine buffer cleanup */
    buffer[0] = '\0';
    filename1 = 0;
    fileContentBuffer[0] = '\0';

    /* Actual code */
    interrupt(0x21, 0, "SHELL> ", 0, 0);
    interrupt(0x21, 1, buffer, 0, 0);
    if (strncmp(buffer, "type ", 5)) {
      filename1 = buffer + 5;
      cleanFilename(filename1);
      interrupt(0x21, 3, filename1, fileContentBuffer, 0);
      interrupt(0x21, 0, fileContentBuffer, 0, 0);
    } else if (strncmp(buffer, "execute ", 8)) {
      filename1 = buffer + 8;
      cleanFilename(filename1);
      interrupt(0x21, 4, filename1, 0x2000, 0);
    } else if (strncmp(buffer, "exit\r", 5)) {
      return 0;
    } else if (strncmp(buffer, "delete ", 7)) {
      filename1 = buffer + 7;
      cleanFilename(filename1);
      interrupt(0x21, 7, filename1, 0, 0);
    } else if (strncmp(buffer, "copy ", 5)) {
      filename1 = buffer + 5;
      debug[0] = indexOf(filename1, " ") + 0x30;
      interrupt(0x21, 0, debug, 0, 0);
      filename1[indexOf(filename1, " ")] = '\0';
      interrupt(0x21, 0, filename1, 0, 0);
      filename2 = indexOf(buffer + 5, " ") + 1;
      interrupt(0x21, 0, filename2, 0, 0);
      interrupt(0x21, 3, filename1, fileContentBuffer, 0);
      interrupt(0x21, 8, filename2, fileContentBuffer, 0);
    } else if (strncmp(buffer, "dir\r", 4)) {
    } else if (strncmp(buffer, "create ", 7)) {
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
