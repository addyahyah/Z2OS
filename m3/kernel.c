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
int mod(int, int);
int div(int, int);
void handleInterrupt21(int, int, int, int);
int readFile(char* fileName, char* buffer);
void executeProgram(char*, int);
void terminate();

int main()
{
  char shell[6];

  int start;
  int i;

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

int readFile(char* fileName, char* buffer) {
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
          return 0;
        } else {
          readSector(buffer+k, directory[i+j]);
        }
      }
      return 0;
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
  return -1;
}

void executeProgram(char* name, int segment) {
  char buffer[MAXIMUM_FILE_SIZE];
  int result;
  unsigned int i;
  if (mod(segment, 0x1000) != 0) {
    char errorMsg[35];
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
    char errorMsg[16];
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
  result = readFile(name, buffer);
  
  if(result == 0){
    for (i = 0; i < MAXIMUM_FILE_SIZE; i++) {
      putInMemory(segment, i, buffer[i]);
    }
    launchProgram(segment);
  } 
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
