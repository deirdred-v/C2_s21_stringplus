#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *dstf = dest;
  while (*dstf != '\0') dstf++;
  while (*src != '\0' && n > 0) {
    *dstf++ = *src++;
    n--;
  }
  *dstf = '\0';
  return dest;
}