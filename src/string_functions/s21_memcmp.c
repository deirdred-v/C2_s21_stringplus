#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *byte_ptr1 = (const unsigned char *)str1;
  const unsigned char *byte_ptr2 = (const unsigned char *)str2;
  int result = 0;
  for (s21_size_t i = 0; i < n && result == 0; i++) {
    result = (int)(byte_ptr1[i] - byte_ptr2[i]);
  }
  return result;
}