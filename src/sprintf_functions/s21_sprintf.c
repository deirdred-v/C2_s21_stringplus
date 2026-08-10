#include <stdarg.h>

#include "../string_functions/s21_strlen.c"

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  size_t index = 0;
  size_t buffer_index = 0;

  while (format[index] != '\0') {
    if (format[index] == '%') {
      index++;
      FormatSpec spec;
      parse_format(format, &spec, &index, &args);

      char temp_buffer[2000];
      s21_memset(temp_buffer, 0, sizeof(temp_buffer));
      int temp_len = 0;

      switch (spec.specifier) {
        case 'c':
          temp_len = handle_char(temp_buffer, &spec, &args);
          break;
        case 'd':
          temp_len = handle_int(temp_buffer, &spec, &args);
          break;
        case 'f':
          temp_len = handle_float(temp_buffer, &spec, &args);
          break;
        case 's':
          temp_len = handle_string(temp_buffer, &spec, &args);
          break;
        case 'u':
          temp_len = handle_unsigned(temp_buffer, &spec, &args);
          break;
        case 'x':
        case 'X':
          temp_len = handle_xX(temp_buffer, &spec, &args);
          break;
        case 'g':
        case 'G': {
          long double val;
          if (spec.length == 'L') {
            val = va_arg(args, long double);
          } else {
            double tmp = va_arg(args, double);
            val = (long double)tmp;
          }
          temp_len = handle_gG(temp_buffer, &spec, val);
          break;
        }
        case 'e':
        case 'E': {
          long double val;
          if (spec.length == 'L') {
            val = va_arg(args, long double);
          } else {
            val = (long double)va_arg(args, double);
          }
          temp_len = handle_eE(temp_buffer, &spec, val, spec.specifier);
          break;
        }
        case 'o':
          temp_len = handle_o(temp_buffer, &spec, &args);
          break;
        case 'p':
          temp_len = handle_p(temp_buffer, &spec, &args);
          break;
        case '%':
          temp_len = handle_percent(temp_buffer);
          break;
      }

      int pad_len = 0;
      if (spec.width < temp_len) {
        pad_len = spec.width - temp_len;
      }

      for (int i = 0; i < temp_len; i++) {
        str[buffer_index++] = temp_buffer[i];
      }

      if (spec.flag_minus) {
        for (int i = 0; i < pad_len; i++) {
          str[buffer_index++] = ' ';
        }
      }
    } else {
      str[buffer_index++] = format[index++];
    }
  }

  str[buffer_index] = '\0';
  va_end(args);
  return buffer_index;
}
