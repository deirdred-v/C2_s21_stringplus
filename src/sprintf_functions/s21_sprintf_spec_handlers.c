#include "../s21_string.h"

#define MAX_BUFFER_SIZE 100

/* -------------------------------------------------------------------
   handle_char: обрабатывает спецификатор 'c'
   ------------------------------------------------------------------- */
int handle_char(char *buffer, FormatSpec *spec, va_list *args) {
  char value = (char)va_arg(*args, int);

  buffer[0] = value;
  buffer[1] = '\0';

  int length = s21_handle_width(buffer, 1, spec);

  return length;
}

/* -------------------------------------------------------------------
   handle_int: обрабатывает спецификатор 'd'
   ------------------------------------------------------------------- */
int handle_int(char *buffer, FormatSpec *spec, va_list *args) {
  int value = va_arg(*args, int);
  int is_negative = (value < 0);
  if (is_negative) {
    value = -value;
  }

  char temp[32];
  int temp_index = 0;
  do {
    temp[temp_index++] = (char)((value % 10) + '0');
    value /= 10;
  } while (value > 0);

  int num_length = temp_index;
  if (spec->precision > num_length) {
    num_length = spec->precision;
  }
  int total_length = num_length;

  if (is_negative || spec->flag_plus || spec->flag_space) {
    total_length++;
  }

  char pad_char = ' ';
  if (spec->flag_zero && !spec->flag_minus && spec->precision < 0) {
    pad_char = '0';
  }

  int pad_count =
      (spec->width > total_length) ? (spec->width - total_length) : 0;
  int pos = 0;

  if (!spec->flag_minus && pad_char == ' ') {
    for (int i = 0; i < pad_count; i++) {
      buffer[pos++] = ' ';
    }
  }

  if (is_negative) {
    buffer[pos++] = '-';
  } else if (spec->flag_plus) {
    buffer[pos++] = '+';
  } else if (spec->flag_space) {
    buffer[pos++] = ' ';
  }

  if (pad_char == '0') {
    for (int i = 0; i < pad_count; i++) {
      buffer[pos++] = '0';
    }
  }

  int zeros_needed = num_length - temp_index;
  for (int i = 0; i < zeros_needed; i++) {
    buffer[pos++] = '0';
  }

  for (int i = temp_index - 1; i >= 0; i--) {
    buffer[pos++] = temp[i];
  }

  if (spec->flag_minus) {
    for (int i = 0; i < pad_count; i++) {
      buffer[pos++] = ' ';
    }
  }

  buffer[pos] = '\0';
  return pos;
}

/* -------------------------------------------------------------------
   handle_float: обрабатывает спецификатор 'f'
   ------------------------------------------------------------------- */
int handle_float(char *buffer, FormatSpec *spec, va_list *args) {
  if (spec->precision < 0) {
    spec->precision = 6;
  }

  long double value;
  if (spec->length == 'L') {
    value = va_arg(*args, long double);
  } else {
    value = va_arg(*args, double);
  }

  char temp_buf[1024];
  int pos = 0;

  int negative = (value < 0.0);
  if (negative) {
    value = -value;
  }

  long long int_part = (long long)value;
  long double fractional = value - (long double)int_part;

  char int_part_buf[64];
  int int_len = 0;
  if (int_part == 0) {
    int_part_buf[int_len++] = '0';
  } else {
    while (int_part > 0) {
      int_part_buf[int_len++] = (char)('0' + (int_part % 10));
      int_part /= 10;
    }
  }

  for (int i = 0; i < int_len / 2; i++) {
    char temp = int_part_buf[i];
    int_part_buf[i] = int_part_buf[int_len - 1 - i];
    int_part_buf[int_len - 1 - i] = temp;
  }

  char frac_buf[128];
  if (spec->precision > 0) {
    fractional *= powl(10.0, spec->precision);
    long long frac_part = (long long)(fractional + 0.5);
    for (int i = spec->precision - 1; i >= 0; i--) {
      frac_buf[i] = (char)((frac_part % 10) + '0');
      frac_part /= 10;
    }
  }

  if (negative) {
    temp_buf[pos++] = '-';
  } else if (spec->flag_plus) {
    temp_buf[pos++] = '+';
  } else if (spec->flag_space) {
    temp_buf[pos++] = ' ';
  }

  for (int i = 0; i < int_len; i++) {
    temp_buf[pos++] = int_part_buf[i];
  }

  if (spec->precision > 0) {
    temp_buf[pos++] = '.';
    for (int i = 0; i < spec->precision; i++) {
      temp_buf[pos++] = frac_buf[i];
    }
  }

  temp_buf[pos] = '\0';

  int temp_len = pos;
  pos = 0;
  if (spec->width > temp_len) {
    int pad_count = spec->width - temp_len;
    if (spec->flag_zero) {
      if (spec->flag_plus || spec->flag_space) {
        buffer[pos++] = temp_buf[0];
        for (int i = 0; i < pad_count; i++) {
          buffer[pos++] = '0';
        }
        for (int i = 1; i < temp_len; i++) {
          buffer[pos++] = temp_buf[i];
        }
      } else {
        for (int i = 0; i < pad_count; i++) {
          buffer[pos++] = '0';
        }
        for (int i = 0; i < temp_len; i++) {
          buffer[pos++] = temp_buf[i];
        }
      }
    } else if (!spec->flag_minus) {
      for (int i = 0; i < pad_count; i++) {
        buffer[pos++] = ' ';
      }
      for (int i = 0; i < temp_len; i++) {
        buffer[pos++] = temp_buf[i];
      }
    } else {
      for (int i = 0; i < temp_len; i++) {
        buffer[pos++] = temp_buf[i];
      }
      for (int i = 0; i < pad_count; i++) {
        buffer[pos++] = ' ';
      }
    }
  } else {
    for (int i = 0; i < temp_len; i++) {
      buffer[pos++] = temp_buf[i];
    }
  }

  buffer[pos] = '\0';
  return pos;
}

/* -------------------------------------------------------------------
   handle_string: обрабатывает спецификатор 's'
   ------------------------------------------------------------------- */
int handle_string(char *buffer, FormatSpec *spec, va_list *args) {
  const char *str = va_arg(*args, const char *);
  if (!str) {
    str = "(null)";
  }

  int str_len = s21_strlen(str);
  if (spec->precision >= 0 && spec->precision < str_len) {
    str_len = spec->precision;
  }

  int padding = (spec->width > str_len) ? spec->width - str_len : 0;
  char pad_char = (spec->flag_minus) ? '0' : ' ';

  int length = 0;

  if (!spec->flag_minus) {
    for (int i = 0; i < padding; i++) {
      buffer[length++] = pad_char;
    }
  }

  for (int i = 0; i < str_len; i++) {
    buffer[length++] = str[i];
  }

  if (spec->flag_minus) {
    for (int i = 0; i < padding; i++) {
      buffer[length++] = ' ';
    }
  }

  buffer[length] = '\0';
  return length;
}

/* -------------------------------------------------------------------
   handle_unsigned: обрабатывает спецификатор 'u'
   ------------------------------------------------------------------- */
int handle_unsigned(char *buffer, FormatSpec *spec, va_list *args) {
  unsigned long long value;

  if (spec->length == 'l') {
    value = va_arg(*args, unsigned long);
  } else if (spec->length == 'L') {
    value = va_arg(*args, unsigned long long);
  } else {
    value = va_arg(*args, unsigned int);
  }

  char temp[32];
  int temp_len = 0;

  do {
    temp[temp_len++] = (char)('0' + (value % 10));
    value /= 10;
  } while (value > 0);

  for (int i = 0; i < temp_len / 2; i++) {
    char t = temp[i];
    temp[i] = temp[temp_len - 1 - i];
    temp[temp_len - 1 - i] = t;
  }
  temp[temp_len] = '\0';

  int num_zeros = 0;
  if (spec->precision > temp_len) {
    num_zeros = spec->precision - temp_len;
  }

  char pad_char =
      (spec->flag_zero && spec->precision < 0 && !spec->flag_minus) ? '0' : ' ';
  int total_len = temp_len + num_zeros;
  int pad_count = (spec->width > total_len) ? (spec->width - total_len) : 0;

  int pos = 0;

  if (!spec->flag_minus && pad_char == ' ') {
    for (int i = 0; i < pad_count; i++) {
      buffer[pos++] = ' ';
    }
  }

  for (int i = 0; i < num_zeros; i++) {
    buffer[pos++] = '0';
  }
  if (pad_char == '0') {
    for (int i = 0; i < pad_count; i++) {
      buffer[pos++] = '0';
    }
  }

  for (int i = 0; i < temp_len; i++) {
    buffer[pos++] = temp[i];
  }

  if (spec->flag_minus) {
    for (int i = 0; i < pad_count; i++) {
      buffer[pos++] = ' ';
    }
  }

  buffer[pos] = '\0';
  return pos;
}

/* -------------------------------------------------------------------
   handle_percent: обрабатывает '%'
   ------------------------------------------------------------------- */
int handle_percent(char *buffer) {
  int count = 0;

  buffer[count++] = '%';
  return count;
}

/* -------------------------------------------------------------------
   handle_eE: обрабатывает спецификаторы 'e'/'E'
   (с half-to-even и "двойной проверкой")
   ------------------------------------------------------------------- */

int handle_eE(char *buffer, FormatSpec *spec, long double val, char e_char) {
  int negative = (val < 0.0L);
  if (negative) val = -val;

  spec->precision = (spec->precision < 0) ? 6 : spec->precision;
  int exponent = 0;
  if (val != 0.0L) {
    exponent = (int)floorl(log10l(val));
    val /= powl(10.0L, exponent);
  }

  val = s21_round_half_to_even(val, spec->precision);

  char mantissa[128];
  long long int_part = (long long)val;
  long double frac_part = val - (long double)int_part;
  int pos = snprintf(mantissa, sizeof(mantissa), "%lld", int_part);

  if (spec->flag_hash || spec->precision > 0) {
    mantissa[pos++] = '.';
    for (int i = 0; i < spec->precision; i++) {
      frac_part *= 10.0L;
      mantissa[pos++] = (char)('0' + (int)frac_part);
      frac_part -= (int)frac_part;
    }
    if (frac_part >= 0.5L) {
      while (pos > 0 && mantissa[pos - 1] == '9') {
        mantissa[--pos] = '0';
      }
      if (pos == 0 || mantissa[pos - 1] == '.') {
        mantissa[pos++] = '1';
      } else {
        mantissa[pos - 1]++;
      }
    }
  }
  mantissa[pos] = '\0';
  // Формирование части "e+XX" / "E+XX"
  char exponent_part[16];
  snprintf(exponent_part, sizeof(exponent_part), "%c%+03d", e_char, exponent);
  // Формирование итоговой строки
  char temp_buffer[256];
  pos = snprintf(
      temp_buffer, sizeof(temp_buffer), "%s%s%s",
      (negative ? "-"
                : (spec->flag_plus ? "+" : (spec->flag_space ? " " : ""))),
      mantissa, exponent_part);

  return align_and_pad(buffer, temp_buffer, pos, spec, 0);
}

/* -------------------------------------------------------------------
   handle_gG: основной интерес — обрабатывает '%g'/'%G'
   ------------------------------------------------------------------- */
int handle_gG(char *buffer, FormatSpec *spec, long double val) {
  if (spec->precision < 0) spec->precision = 6;
  if (spec->precision == 0) spec->precision = 1;

  int negative = (val < 0.0L);
  if (negative) val = -val;
  char sign_char =
      negative ? '-' : (spec->flag_plus ? '+' : (spec->flag_space ? ' ' : 0));

  if (val == 0.0L) {
    return format_zero(buffer, spec, sign_char);
  }

  int exponent = (val != 0.0L) ? (int)floorl(log10l(val)) : 0;

  char result_body[512];
  int body_len = format_floating(result_body, val, spec, exponent);

  return align_and_pad(buffer, result_body, body_len, spec, sign_char);
}

int handle_xX(char *buffer, FormatSpec *spec, va_list *args) {
  unsigned int value = va_arg(*args, unsigned int);
  if (value == 0 && spec->precision == 0) {
    buffer[0] = '\0';
    return 0;
  }

  char number[64];
  convert_to_base(number, value, 16, spec->specifier == 'X');

  const char *prefix = (spec->flag_hash && value != 0)
                           ? (spec->specifier == 'x' ? "0x" : "0X")
                           : NULL;

  return format_number_with_prefix(buffer, number, spec, prefix);
}

int handle_o(char *buffer, FormatSpec *spec, va_list *args) {
  unsigned long long value = 0;

  switch (spec->length) {
    case 'h':
      value = (unsigned short)va_arg(*args, unsigned int);
      break;
    case 'l':
      value = va_arg(*args, unsigned long);
      break;
    default:
      value = va_arg(*args, unsigned int);
      break;
  }

  char number[64];
  convert_to_base(number, value, 8, 0);

  const char *prefix = (spec->flag_hash && value != 0) ? "0" : NULL;
  return format_number_with_prefix(buffer, number, spec, prefix);
}

int handle_p(char *buffer, FormatSpec *spec, va_list *args) {
  void *ptr = va_arg(*args, void *);

  if (!ptr) {
    char *nil_representation = "(nil)";
    int len = s21_strlen(nil_representation);
    s21_strncpy(buffer, nil_representation, len);
    return len;
  }

  unsigned long long addr = (unsigned long long)ptr;
  char number[64];
  convert_to_base(number, addr, 16, 0);

  const char *prefix = "0x";
  return format_number_with_prefix(buffer, number, spec, prefix);
}