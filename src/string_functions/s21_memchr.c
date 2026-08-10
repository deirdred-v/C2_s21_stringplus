#include "../s21_string.h"

void *s21_memchr(const void *s, int c, s21_size_t n) {
  const unsigned char *byte_ptr = (const unsigned char *)s;
  unsigned char target = (unsigned char)c;
  char *result = S21_NULL;

  for (s21_size_t i = 0; i < n; i++) {
    if (byte_ptr[i] == target) {
      result = (void *)(byte_ptr + i);
      i = n;
    }
  }
  return result;
}