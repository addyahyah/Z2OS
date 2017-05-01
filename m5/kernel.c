/*
* team numbr: 1D
* team member: Zhou Zhou, Jake Patterson, Yuzong Gao, Luwen Zhang
*/
#include <stdlib.h>
#include <stdio.h>
#include "./string.h"
#include "./constants.h"

void printString(char *);
void readString(char *);
void readSector(char *, int);
void writeSector(char *, int);
int mod(int, int);
int div(int, int);
void handleInterrupt21(int, int, int, int);
void handleTimerInterrupt(int, int);
void readFile(char*, char*);
void deleteFile(char*);
void writeFile(char*, char*, int);
void executeProgram(char*, int);
void terminate();
void listFile();

int main() {
  char shell[6];

  int start;
  int i;
  makeTimerInterrupt();

  makeInterrupt21();
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '\0';
  interrupt(0x21, 4, shell, 0x2000, 0);
  return 0;
}

void listFile() {
  char directory[SECTOR_SIZE];
  char msg[3];
  unsigned int count, currNum, index;
  unsigned int i, j, k;
  char nameStr[12];

  readSector(directory, 2);
  for (i = 0; i < SECTOR_SIZE; i += DIRECTORY_RECORD_SIZE) {
    count = 0;
    if (directory[i] == 0) {
      continue;
    } else {
      for(k = 0; k<DIRECTORY_FILENAME_SIZE; k++){
        if(directory[i+k] == 0){
          nameStr[k] = ' ';
        } else{
          nameStr[k] = directory[i+k];
        }
      }
      for (j = DIRECTORY_FILENAME_SIZE; j < DIRECTORY_RECORD_SIZE; j++) {
        if (directory[i+j] == 0) {
          break;
        } else {
          count++;
        }
      }

      nameStr[6] = ' ';
      nameStr[7] = ' ';
      nameStr[8] = ' ';
      index = 0;
      do {
        currNum = mod(count, 10);
        nameStr[8-index] = 0x30+currNum;
        index++;
        count = div(count, 10);
      } while(count);
      nameStr[9] = '\r';
      nameStr[10] = '\n';
      nameStr[11] = '\0';

      printString(nameStr);
      
    }
  }
}

void printString(char *str) {
  unsigned int i = 0;
  while (str[i] != '\0') {
    interrupt(0x10, 0xe*256+str[i], 0, 0, 0);
    i++;
  }
}

void readString(char *buffer) {
  char currentChar;
  char bufferToPrint[3];
  unsigned int i = 0;
  bufferToPrint[1] = '\0';
  do {
    currentChar = interrupt(0x16, 0, 0, 0, 0);
    switch (currentChar) {
      case 0xd:
        buffer[i] = 0xd;
        i++;
        buffer[i] = 0xa;
        i++;
        buffer[i] = 0x0;
        i++;
        bufferToPrint[0] = '\r';
        bufferToPrint[1] = '\n';
        bufferToPrint[2] = '\0';
        printString(bufferToPrint);
        return;
      case 0x8:
        bufferToPrint[0] = currentChar;
        if (i > 0) {
          printString(bufferToPrint);
          i--;
        }
        break;
      default:
        buffer[i] = currentChar;
        bufferToPrint[0] = currentChar;
        printString(bufferToPrint);
        i++;
    }
  } while (1);
}

void readSector(char *buffer, int sector) {
  int relativeSector = mod(sector, 18) + 1;
  int head = mod(div(sector, 18), 2);
  int track = div(sector, 36);
  interrupt(0x13, 2*256+1, buffer, track*256+relativeSector, head*256+0);
}

void writeSector(char *buffer, int sector) {
  int relativeSector = mod(sector, 18) + 1;
  int head = mod(div(sector, 18), 2);
  int track = div(sector, 36);
  interrupt(0x13, 3*256+1, buffer, track*256+relativeSector, head*256+0);
}

int mod(int a, int b) {
  while (a >= b) {
    a = a - b;
  }
  return a;
}

int div(int a, int b) {
  int quotient = 0;
  while ((quotient + 1) * b <= a) {
    quotient++;
  }
  return quotient;
}

void handleInterrupt21(int ax, int bx, int cx, int dx) {
  char errorMsg[36];
  switch (ax) {
    case 0:
      printString(bx);
      break;
    case 1:
      readString(bx);
      break;
    case 2:
      readSector(bx, cx);
      break;
    case 3:
      readFile(bx, cx);
      break;
    case 4:
      executeProgram(bx, cx);
      break;
    case 5:
      terminate();
      break;
    case 6:
      writeSector(bx, cx);
      break;
    case 7:
      deleteFile(bx);
      break;
    case 8:
      writeFile(bx, cx, dx);
      break;
    case 9:
      listFile();
      break;
    default:
      errorMsg[0] = 'E';
      errorMsg[1] = 'r';
      errorMsg[2] = 'r';
      errorMsg[3] = 'o';
      errorMsg[4] = 'r';
      errorMsg[5] = ':';
      errorMsg[6] = ' ';
      errorMsg[7] = 'w';
      errorMsg[8] = 'h';
      errorMsg[9] = 'a';
      errorMsg[10] = 't';
      errorMsg[11] = ' ';
      errorMsg[12] = 'd';
      errorMsg[13] = 'i';
      errorMsg[14] = 'd';
      errorMsg[15] = ' ';
      errorMsg[16] = 'y';
      errorMsg[17] = 'o';
      errorMsg[18] = 'u';
      errorMsg[19] = ' ';
      errorMsg[20] = 'c';
      errorMsg[21] = 'a';
      errorMsg[22] = 'l';
      errorMsg[23] = 'l';
      errorMsg[24] = ' ';
      errorMsg[25] = 'i';
      errorMsg[26] = 't';
      errorMsg[27] = ' ';
      errorMsg[28] = 'w';
      errorMsg[29] = 'i';
      errorMsg[30] = 't';
      errorMsg[31] = 'h';
      errorMsg[32] = '?';
      errorMsg[33] = '\r';
      errorMsg[34] = '\n';
      errorMsg[35] = '\0';
      printString(errorMsg);
  }
}

void readFile(char* fileName, char* buffer) {
  char directory[SECTOR_SIZE];
  unsigned int i, j, k;
  char notFound[17];

  readSector(directory, 2);
  for (i = 0; i < SECTOR_SIZE; i += DIRECTORY_RECORD_SIZE) {
    if (directory[i] == 0) {
      continue;
    } else if (strncmp(fileName, directory + i, 6)) {
      for (j = DIRECTORY_FILENAME_SIZE, k = 0; j < DIRECTORY_RECORD_SIZE; j++, k += SECTOR_SIZE) {
        if (directory[i+j] == 0) {
          return;
        } else {
          readSector(buffer+k, directory[i+j]);
        }
      }
      return;
    }
  }
  
  notFound[0] = 'F';
  notFound[1] = 'i';
  notFound[2] = 'l';
  notFound[3] = 'e';
  notFound[4] = ' ';
  notFound[5] = 'n';
  notFound[6] = 'o';
  notFound[7] = 't';
  notFound[8] = ' ';
  notFound[9] = 'F';
  notFound[10] = 'o';
  notFound[11] = 'u';
  notFound[12] = 'n';
  notFound[13] = 'd';
  notFound[14] = '\r';
  notFound[15] = '\n';
  notFound[16] = '\0';

  printString(notFound);
  return;
}

void deleteFile(char* fileName) {
  char map[SECTOR_SIZE];
  char directory[SECTOR_SIZE];
  char notFound[17];
  unsigned int i, j;

  readSector(map, 1);
  readSector(directory, 2);
  for (i = 0; i < SECTOR_SIZE; i += DIRECTORY_RECORD_SIZE) {
    if (directory[i] == 0) {
      continue;
    } else if (strncmp(fileName, directory + i, 6)) {
      directory[i] = 0;

      for (j = DIRECTORY_FILENAME_SIZE; j < DIRECTORY_RECORD_SIZE; j++) {
        if (directory[i+j] == 0) {
          break;
        } else {
          map[directory[i+j]-1] = 0;
        }
      }
      writeSector(map, 1);
      writeSector(directory, 2);
      return;
    }
  }
  
  notFound[0] = 'F';
  notFound[1] = 'i';
  notFound[2] = 'l';
  notFound[3] = 'e';
  notFound[4] = ' ';
  notFound[5] = 'n';
  notFound[6] = 'o';
  notFound[7] = 't';
  notFound[8] = ' ';
  notFound[9] = 'F';
  notFound[10] = 'o';
  notFound[11] = 'u';
  notFound[12] = 'n';
  notFound[13] = 'd';
  notFound[14] = '\r';
  notFound[15] = '\n';
  notFound[16] = '\0';

  printString(notFound);
  return;
}

void writeFile(char* name, char* buffer, int numberOfSectors) {
  char map[SECTOR_SIZE];
  char directory[SECTOR_SIZE];
  unsigned int i, j, k, l;
  char fullName[DIRECTORY_FILENAME_SIZE];
  char errorMsg[34];
  char temp;

  bzero(fullName, DIRECTORY_FILENAME_SIZE);

  readSector(map, 1);
  readSector(directory, 2);
  for (i = 0; i < SECTOR_SIZE; i += DIRECTORY_RECORD_SIZE) {
    if (directory[i] == 0) {
      for (j = 0; j < SECTOR_SIZE - numberOfSectors; j++) {
        if (map[j] == 0) {
          for (k = 0; k < numberOfSectors; k++) {
            directory[i+DIRECTORY_FILENAME_SIZE+k] = j+k;
            map[j+k] = 0xFF;
            writeSector(buffer + k*SECTOR_SIZE, j+k);
          }
          for (k = DIRECTORY_FILENAME_SIZE+1+k; k < DIRECTORY_RECORD_SIZE; k++) {
            directory[k] = 0;
          }
          /* The bit following the last byte of filename needs backed up because strncpy writes \0 to the next index */
          temp = directory[i + DIRECTORY_FILENAME_SIZE];
          strncpy(fullName, name, strlen(name));
          strncpy(directory + i, fullName, DIRECTORY_FILENAME_SIZE);
          directory[i + DIRECTORY_FILENAME_SIZE] = temp;
          writeSector(map, 1);
          writeSector(directory, 2);
          return;
        }
      }
      errorMsg[0] = 'N';
      errorMsg[1] = 'o';
      errorMsg[2] = 't';
      errorMsg[3] = ' ';
      errorMsg[4] = 'e';
      errorMsg[5] = 'n';
      errorMsg[6] = 'o';
      errorMsg[7] = 'u';
      errorMsg[8] = 'g';
      errorMsg[9] = 'h';
      errorMsg[10] = ' ';
      errorMsg[11] = 'f';
      errorMsg[12] = 'r';
      errorMsg[13] = 'e';
      errorMsg[14] = 'e';
      errorMsg[15] = ' ';
      errorMsg[16] = 's';
      errorMsg[17] = 'e';
      errorMsg[18] = 'c';
      errorMsg[19] = 't';
      errorMsg[20] = 'o';
      errorMsg[21] = 'r';
      errorMsg[22] = 's';
      errorMsg[23] = '!';
      errorMsg[24] = '\r';
      errorMsg[25] = '\n';
      errorMsg[26] = '\0';
      printString(errorMsg);
    }
  }
  errorMsg[0] = 'N';
  errorMsg[1] = 'o';
  errorMsg[2] = ' ';
  errorMsg[3] = 'm';
  errorMsg[4] = 'o';
  errorMsg[5] = 'r';
  errorMsg[6] = 'e';
  errorMsg[7] = ' ';
  errorMsg[8] = 'f';
  errorMsg[9] = 'r';
  errorMsg[10] = 'e';
  errorMsg[11] = 'e';
  errorMsg[12] = ' ';
  errorMsg[13] = 'd';
  errorMsg[14] = 'i';
  errorMsg[15] = 'r';
  errorMsg[16] = 'e';
  errorMsg[17] = 'c';
  errorMsg[18] = 't';
  errorMsg[19] = 'o';
  errorMsg[20] = 'r';
  errorMsg[21] = 'y';
  errorMsg[22] = ' ';
  errorMsg[23] = 'e';
  errorMsg[24] = 'n';
  errorMsg[25] = 't';
  errorMsg[26] = 'r';
  errorMsg[27] = 'i';
  errorMsg[28] = 'e';
  errorMsg[29] = 's';
  errorMsg[30] = '!';
  errorMsg[31] = '\r';
  errorMsg[32] = '\n';
  errorMsg[33] = '\0';
  printString(errorMsg);
  return;
}

void executeProgram(char* name, int segment) {
  char buffer[MAXIMUM_FILE_SIZE];
  char errorMsg[35];
  unsigned int i;
  if (mod(segment, 0x1000) != 0) {
    errorMsg[0] = 'S';
    errorMsg[1] = 'e';
    errorMsg[2] = 'g';
    errorMsg[3] = 'm';
    errorMsg[4] = 'e';
    errorMsg[5] = 'n';
    errorMsg[6] = 't';
    errorMsg[7] = ' ';
    errorMsg[8] = 'n';
    errorMsg[9] = 'o';
    errorMsg[10] = 't';
    errorMsg[11] = ' ';
    errorMsg[12] = 'a';
    errorMsg[13] = ' ';
    errorMsg[14] = 'm';
    errorMsg[15] = 'u';
    errorMsg[16] = 'l';
    errorMsg[17] = 't';
    errorMsg[18] = 'i';
    errorMsg[19] = 'p';
    errorMsg[20] = 'l';
    errorMsg[21] = 'e';
    errorMsg[22] = ' ';
    errorMsg[23] = 'o';
    errorMsg[24] = 'f';
    errorMsg[25] = ' ';
    errorMsg[26] = '0';
    errorMsg[27] = 'x';
    errorMsg[28] = '1';
    errorMsg[29] = '0';
    errorMsg[30] = '0';
    errorMsg[31] = '0';
    errorMsg[32] = '\r';
    errorMsg[33] = '\n';
    errorMsg[34] = '\0';
    printString(errorMsg);
    return;
  } else if (segment == 0 || segment == 0x1000 || segment > 0xA000) {
    errorMsg[0] = 'I';
    errorMsg[1] = 'l';
    errorMsg[2] = 'l';
    errorMsg[3] = 'e';
    errorMsg[4] = 'g';
    errorMsg[5] = 'a';
    errorMsg[6] = 'l';
    errorMsg[7] = ' ';
    errorMsg[8] = 's';
    errorMsg[9] = 'e';
    errorMsg[10] = 'g';
    errorMsg[11] = 'm';
    errorMsg[12] = 'e';
    errorMsg[13] = 'n';
    errorMsg[14] = 't';
    errorMsg[15] = '\0';
    printString(errorMsg);
    return;
  }
  readFile(name, buffer);
  for (i = 0; i < MAXIMUM_FILE_SIZE; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  launchProgram(segment);
}

void terminate() {
  char shell[6];
  shell[0] = 's';
  shell[1] = 'h';
  shell[2] = 'e';
  shell[3] = 'l';
  shell[4] = 'l';
  shell[5] = '\0';
  interrupt(0x21, 4, shell, 0x2000, 0);
}

void handleTimerInterrupt(int segment, int sp) {
  char stringToPrint[4];
  stringToPrint[0]='T';
  stringToPrint[1]='i';
  stringToPrint[2]='c';
  stringToPrint[3]='\0';

  printString(stringToPrint);
  returnFromTimer(segment, sp);
}
