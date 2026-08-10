#include "../s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *dstf = (unsigned char *)str;
  unsigned char cf = (unsigned char)c;
  for (s21_size_t i = 0; i < n; i++) dstf[i] = cf;
  return dstf;
}
