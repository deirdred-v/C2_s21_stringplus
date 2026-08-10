#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  int flag = 0;
  char *res = S21_NULL;

  if (*needle == '\0') res = (char *)haystack;

  for (; !flag && *haystack != '\0'; haystack++) {
    if (*haystack == needle[0]) {
      flag = 1;
      res = (char *)haystack;
      for (s21_size_t i = 0; flag && needle[i] != '\0'; i++) {
        if (*haystack == needle[i]) {
          haystack++;
        } else {
          flag = 0;
          res = S21_NULL;
          // haystack++;
        }
      }
    }
  }
  return res;
}
