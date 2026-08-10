#include "../s21_string.h"

int parse_format(const char *format, FormatSpec *spec, size_t *index,
                 va_list *args) {
  s21_memset(spec, 0, sizeof(FormatSpec));
  spec->precision = -1;

  const char *flags = "-+ 0#";
  while (s21_memchr(flags, format[*index], 5)) {
    if (format[*index] == '-') spec->flag_minus = 1;
    if (format[*index] == '+') spec->flag_plus = 1;
    if (format[*index] == ' ') spec->flag_space = 1;
    if (format[*index] == '0') spec->flag_zero = 1;
    if (format[*index] == '#') spec->flag_hash = 1;
    (*index)++;
  }

  if (format[*index] == '*') {
    spec->width = va_arg(*args, int);
    if (spec->width < 0) {
      spec->flag_minus = 1;
      spec->width = -spec->width;
    }
    (*index)++;
  } else if (format[*index] >= '0' && format[*index] <= '9') {
    spec->width = 0;
    while (format[*index] >= '0' && format[*index] <= '9') {
      spec->width = spec->width * 10 + (format[*index] - '0');
      (*index)++;
    }
  }

  if (format[*index] == '.') {
    (*index)++;
    if (format[*index] == '*') {
      spec->precision = va_arg(*args, int);
      (*index)++;
    } else {
      spec->precision = 0;
      while (format[*index] >= '0' && format[*index] <= '9') {
        spec->precision = spec->precision * 10 + (format[*index] - '0');
        (*index)++;
      }
    }
  }

  if (format[*index] == 'h' || format[*index] == 'l' || format[*index] == 'L') {
    spec->length = format[*index];
    (*index)++;
  }

  const char *specifiers = "cdsufgeExXopG%";
  if (s21_memchr(specifiers, format[*index], 14)) {
    spec->specifier = format[*index];
    (*index)++;
    return 0;
  }

  return -1;
}
