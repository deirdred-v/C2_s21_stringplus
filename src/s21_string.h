#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_

#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define S21_NULL ((void *)0)
#define s21_size_t unsigned long

void *s21_memchr(const void *s, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);

typedef struct {
  int flag_minus;  // Флаг '-'
  int flag_plus;   // Флаг '+'
  int flag_space;  // Флаг ' '
  int flag_zero;   // Флаг '0'
  int flag_hash;   // Флаг '#'
  int width;       // Ширина
  int precision;   // Точность
  char length;     // Длина: 'h', 'l', 'L'
  char specifier;  // Спецификатор: 'c', 'd', 'f', ...
} FormatSpec;

int s21_sprintf(char *str, const char *format, ...);

int parse_format(const char *format, FormatSpec *spec, size_t *index,
                 va_list *args);

int handle_char(char *buffer, FormatSpec *spec, va_list *args);
int handle_int(char *buffer, FormatSpec *spec, va_list *args);
int handle_float(char *buffer, FormatSpec *spec, va_list *args);
int handle_string(char *buffer, FormatSpec *spec, va_list *args);
int handle_unsigned(char *buffer, FormatSpec *spec, va_list *args);
int handle_percent(char *buffer);
int handle_eE(char *buffer, FormatSpec *spec, long double val, char e_char);
int handle_gG(char *buffer, FormatSpec *spec, long double val);
int handle_xX(char *buffer, FormatSpec *spec, va_list *args);
int handle_o(char *buffer, FormatSpec *spec, va_list *args);
int handle_p(char *buffer, FormatSpec *spec, va_list *args);
int handle_char(char *buffer, FormatSpec *spec, va_list *args);
int s21_handle_width(char *buffer, int length, FormatSpec *spec);
long double s21_round_half_to_even(long double val, int precision);
int format_floating(char *result_body, long double val, FormatSpec *spec,
                    int exponent);
int align_and_pad(char *dest, const char *src, int length,
                  const FormatSpec *spec, int sign);
int format_zero(char *buffer, FormatSpec *spec, char sign_char);
int convert_to_base(char *buffer, unsigned long long value, int base,
                    int is_upper);
int format_number_with_prefix(char *buffer, const char *number,
                              FormatSpec *spec, const char *prefix);

#endif  // SRC_S21_STRING_H_
