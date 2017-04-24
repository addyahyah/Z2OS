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
void readFile(char* fileName, char* buffer);
void deleteFile(char*);
void executeProgram(char*, int);
void terminate();

int main()
{
  int start;
  int i;

  makeInterrupt21();
  interrupt(0x21, 4, "shell\0", 0x2000, 0);
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
    default:
      printString("Error: What did you call it with?\r\n\0");
  }
}

void readFile(char* fileName, char* buffer) {
  char directory[SECTOR_SIZE];
  unsigned int i, j, k;

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
  printString("File not found!\r\n");
  return;
}

void deleteFile(char* fileName) {
  char map[SECTOR_SIZE];
  char directory[SECTOR_SIZE];
  char debug[2];
  unsigned int i, j;
  debug[1] = '\0';

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
  printString("File not found!\r\n");
  return;
}

void executeProgram(char* name, int segment) {
  char buffer[MAXIMUM_FILE_SIZE];
  unsigned int i;
  if (mod(segment, 0x1000) != 0) {
    printString("Segment not a multiple of 0x1000\r\n");
    return;
  } else if (segment == 0 || segment == 0x1000 || segment > 0xA000) {
    printString("Illegal segment\r\n");
    return;
  }
  readFile(name, buffer);
  for (i = 0; i < MAXIMUM_FILE_SIZE; i++) {
    putInMemory(segment, i, buffer[i]);
  }
  launchProgram(segment);
}

void terminate() {
  interrupt(0x21, 4, "shell\0", 0x2000, 0);
}
