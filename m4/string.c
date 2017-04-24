/*
 * team number: 1D
 * team member: Zhou Zhou, Jake Patterson, Yuzong Gao, Luwen Zhang
*/
#include "./string.h"

int strlen(char *s) {
  unsigned int i = 0;
  while (s[i] != '\0') {
    i++;
  }
  return i;
}

int strcmp(char *s1, char *s2) {
  int count1 = strlen(s1);
  int count2 = strlen(s2);
  if (count1 != count2) {
    return 0;
  } else {
    return strncmp(s1, s2, count1);
  }
}

int strncmp(char *s1, char *s2, int n) {
  unsigned int i;
  for (i = 0; i < n; i++) {
    if (s1[i] != s2[i]) {
      return 0;
    } else if (s1[i] == '\0') {
      break;
    }
  }
  return 1;
}

char *strcat(char *dest, char *src) {
  int countSrc = strlen(src);
  return strncat(dest, src, countSrc);
}

char *strncat(char *dest, char *src, int n) {
  unsigned int i;
  int destStartIndex = strlen(dest);
  for (i = 0; i < n; i++) {
    dest[destStartIndex + i] = src[i];
    if (src[i] == '\0') {
      return dest;
    }
  }
  dest[destStartIndex + i] = '\0';
  return dest;
}
