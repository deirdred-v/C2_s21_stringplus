#include "../s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *dstf = (unsigned char *)dest;
  const unsigned char *srcf = (const unsigned char *)src;
  for (s21_size_t i = 0; i < n; i++) dstf[i] = srcf[i];
  return (void *)dstf;
}