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
  char prompt[8], type[6], execute[9], exit[6], delete[8],
       copy[6], dir[5], create[8], errorMsg[15];
  prompt[0] = 'S';
  prompt[1] = 'H';
  prompt[2] = 'E';
  prompt[3] = 'L';
  prompt[4] = 'L';
  prompt[5] = '>';
  prompt[6] = ' ';
  prompt[7] = '\0';
  type[0] = 't';
  type[1] = 'y';
  type[2] = 'p';
  type[3] = 'e';
  type[4] = ' ';
  type[5] = '\0';
  execute[0] = 'e';
  execute[1] = 'x';
  execute[2] = 'e';
  execute[3] = 'c';
  execute[4] = 'u';
  execute[5] = 't';
  execute[6] = 'e';
  execute[7] = ' ';
  execute[8] = '\0';
  exit[0] = 'e';
  exit[1] = 'x';
  exit[2] = 'i';
  exit[3] = 't';
  exit[4] = '\r';
  exit[5] = '\0';
  delete[0] = 'd';
  delete[1] = 'e';
  delete[2] = 'l';
  delete[3] = 'e';
  delete[4] = 't';
  delete[5] = 'e';
  delete[6] = ' ';
  delete[7] = '\0';
  copy[0] = 'c';
  copy[1] = 'o';
  copy[2] = 'p';
  copy[3] = 'y';
  copy[4] = ' ';
  copy[5] = '\0';
  dir[0] = 'd';
  dir[1] = 'i';
  dir[2] = 'r';
  dir[3] = ' ';
  dir[4] = '\0';
  delete[0] = 'd';
  delete[1] = 'e';
  delete[2] = 'l';
  delete[3] = 'e';
  delete[4] = 't';
  delete[5] = 'e';
  delete[6] = ' ';
  delete[7] = '\0';
  errorMsg[0] = 'B';
  errorMsg[1] = 'a';
  errorMsg[2] = 'd';
  errorMsg[3] = ' ';
  errorMsg[4] = 'C';
  errorMsg[5] = 'o';
  errorMsg[6] = 'm';
  errorMsg[7] = 'm';
  errorMsg[8] = 'a';
  errorMsg[9] = 'n';
  errorMsg[10] = 'd';
  errorMsg[11] = '!';
  errorMsg[12] = '\r';
  errorMsg[13] = '\n';
  errorMsg[14] = '\0';

  while(1) {
    /* Routine buffer cleanup */
    buffer[0] = '\0';
    filename1 = 0;
    fileContentBuffer[0] = '\0';

    /* Actual code */
    interrupt(0x21, 0, prompt, 0, 0);
    interrupt(0x21, 1, buffer, 0, 0);
    if (strncmp(buffer, type, 5)) {
      filename1 = buffer + 5;
      cleanFilename(filename1);
      interrupt(0x21, 3, filename1, fileContentBuffer, 0);
      interrupt(0x21, 0, fileContentBuffer, 0, 0);
    } else if (strncmp(buffer, execute, 8)) {
      filename1 = buffer + 8;
      cleanFilename(filename1);
      interrupt(0x21, 4, filename1, 0x2000, 0);
    } else if (strncmp(buffer, exit, 5)) {
      return 0;
    } else if (strncmp(buffer, delete, 7)) {
      filename1 = buffer + 7;
      cleanFilename(filename1);
      interrupt(0x21, 7, filename1, 0, 0);
    } else if (strncmp(buffer, copy, 5)) {
      filename1 = buffer + 5;
      filename2 = filename1 + indexOf(filename1, ' ') + 1;
      filename1[indexOf(filename1, ' ')] = '\0';
      cleanFilename(filename1);
      cleanFilename(filename2);
      interrupt(0x21, 3, filename1, fileContentBuffer, 0);
      interrupt(0x21, 8, filename2, fileContentBuffer, 1);
    } else if (strncmp(buffer, "dir\r", 4)) {
    } else if (strncmp(buffer, "create ", 7)) {
    } else {
      interrupt(0x21, 0, errorMsg, 0, 0);
    }
  }
  return 0;
}

void cleanFilename(char* filename) {
  unsigned int i;
  for (i = 0;;i++) {
    if (filename[i] == '\r' || filename[i] == ' ') {
      filename[i] = '\0';
      return;
    } else if (filename[i] == '\0') {
      return;
    }
  }
}
