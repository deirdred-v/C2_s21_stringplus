#include "../s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *last = S21_NULL;
  char *start = S21_NULL;

  if (str != S21_NULL) last = str;
  if (*last == '\0' || (*delim == '\0' && str == S21_NULL)) {
    start = S21_NULL;
  } else {
    char *curr_ch = last;
    while (*curr_ch != '\0' && s21_strchr(delim, *curr_ch)) {
      ++curr_ch;
    }

    if (*curr_ch == '\0') {
      start = S21_NULL;
    } else {
      start = curr_ch;
      while (*curr_ch && (s21_strchr(delim, *curr_ch) == S21_NULL)) {
        ++curr_ch;
      }

      if (*curr_ch == 0) {
        last = curr_ch;
      } else {
        *curr_ch = 0;
        last = curr_ch + 1;
      }
    }
  }

  return start;
}