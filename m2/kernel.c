#include <stdlib.h>
#include <stdio.h>

void printString(char *);
void readString(char *);

int main()
{
  int start;
  int i;

  char line[80];
  /*start = 0x8000;*/
  /*
  * clear the video memory
  */
  /*for(i = 0; i < 25 * 80; i++) {
    putInMemory(0xB000, start, ' ');
    start+=2;
  }
  putInMemory(0xB000, 0x8000, 'H');
  putInMemory(0xB000, 0x8001, 0x7);
  putInMemory(0xB000, 0x8002, 'E');
  putInMemory(0xB000, 0x8003, 0x7);
  putInMemory(0xB000, 0x8004, 'L');
  putInMemory(0xB000, 0x8005, 0x7);
  putInMemory(0xB000, 0x8006, 'L');
  putInMemory(0xB000, 0x8007, 0x7);
  putInMemory(0xB000, 0x8008, 'O');
  putInMemory(0xB000, 0x8009, 0x7);

  putInMemory(0xB000, 0x800A, ' ');
  putInMemory(0xB000, 0x800C, 'W');
  putInMemory(0xB000, 0x800D, 0x7);
  putInMemory(0xB000, 0x800E, 'O');
  putInMemory(0xB000, 0x800F, 0x7);
  putInMemory(0xB000, 0x8010, 'R');
  putInMemory(0xB000, 0x8011, 0x7);
  putInMemory(0xB000, 0x8012, 'L');
  putInMemory(0xB000, 0x8013, 0x7);
  putInMemory(0xB000, 0x8014, 'D');
  putInMemory(0xB000, 0x8015, 0x7);*/
  printString("Hello world\0");
  printString("Enter a line: \0");
  readString(line);
  printString(line);
  while(1){}
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
