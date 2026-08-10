#include "../s21_string.h"

char *s21_strrchr(const char *str, int c) {
  int flag = 0;
  char *res = S21_NULL;
  s21_size_t len_str = s21_strlen(str);
  str = str + len_str;

  for (s21_size_t i = len_str + 1; !flag && i > 0; i--) {
    if (*str == c) {
      res = (char *)str;
      flag = 1;
    }
    str--;
  }
  return res;
}
