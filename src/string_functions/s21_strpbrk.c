#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  int flag = 0;
  char *res = S21_NULL;
  for (; !flag && *str1 != '\0'; str1++) {
    for (s21_size_t i = 0; !flag && str2[i] != '\0'; i++) {
      if (*str1 == str2[i]) {
        res = (char *)str1;
        flag = 1;
      }
    }
  }
  return res;
}