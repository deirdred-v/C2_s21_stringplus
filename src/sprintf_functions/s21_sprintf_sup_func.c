#include "../s21_string.h"

// выравнивание по width с учётом флага '-'

int s21_handle_width(char *buffer, int length, FormatSpec *spec) {
  if (spec->width <= length) {
    return length;
  }

  int total_len = spec->width;
  int pad_count = total_len - length;

  if (spec->flag_minus) {
    for (int i = 0; i < pad_count; i++) {
      buffer[length + i] = ' ';
    }
  } else {
    char pad_char = ' ';

    for (int i = length - 1; i >= 0; i--) {
      buffer[i + pad_count] = buffer[i];
    }

    for (int i = 0; i < pad_count; i++) {
      buffer[i] = pad_char;
    }
  }

  buffer[total_len] = '\0';
  return total_len;
}

long double s21_round_half_to_even(long double val, int precision) {
  long double factor = powl(10.0L, precision);
  val *= factor;
  val = roundl(val);
  return val / factor;
}

int format_floating(char *result_body, long double val, FormatSpec *spec,
                    int exponent) {
  int frac_digits = spec->precision - exponent - 1;
  if (frac_digits < 0) frac_digits = 0;

  long double rounded_val = s21_round_half_to_even(val, frac_digits);
  long long int_part = (long long)rounded_val;
  long double frac_part = rounded_val - (long double)int_part;

  int body_len = 0;
  char int_buf[64];
  int int_len = 0;
  if (int_part == 0) {
    int_buf[int_len++] = '0';
  } else {
    while (int_part > 0) {
      int_buf[int_len++] = (char)('0' + (int_part % 10));
      int_part /= 10;
    }
    for (int i = 0; i < int_len / 2; i++) {
      char tmp = int_buf[i];
      int_buf[i] = int_buf[int_len - 1 - i];
      int_buf[int_len - 1 - i] = tmp;
    }
  }

  for (int i = 0; i < int_len; i++) {
    result_body[body_len++] = int_buf[i];
  }

  if (frac_digits > 0 || spec->flag_hash) {
    result_body[body_len++] = '.';
  }
  for (int i = 0; i < frac_digits; i++) {
    frac_part *= 10.0L;
    int digit = (int)frac_part;
    result_body[body_len++] = (char)('0' + digit);
    frac_part -= digit;
  }

  if (!spec->flag_hash && frac_digits > 0) {
    while (body_len > 0 && result_body[body_len - 1] == '0') {
      body_len--;
    }
    if (body_len > 0 && result_body[body_len - 1] == '.') {
      body_len--;
    }
  }

  result_body[body_len] = '\0';
  return body_len;
}

int align_and_pad(char *dest, const char *src, int length,
                  const FormatSpec *spec, int sign) {
  int pad_char = (spec->flag_zero && !spec->flag_minus) ? '0' : ' ';
  int total_length = length + (sign ? 1 : 0);
  int pad_count =
      (spec->width > total_length) ? (spec->width - total_length) : 0;
  int pos = 0;

  if (!spec->flag_minus && pad_char == ' ') {
    for (int i = 0; i < pad_count; i++) {
      dest[pos++] = ' ';
    }
  }

  if (sign) {
    dest[pos++] = sign;
  }

  if (!spec->flag_minus && pad_char == '0') {
    for (int i = 0; i < pad_count; i++) {
      dest[pos++] = '0';
    }
  }

  for (int i = 0; i < length; i++) {
    dest[pos++] = src[i];
  }

  if (spec->flag_minus) {
    for (int i = 0; i < pad_count; i++) {
      dest[pos++] = ' ';
    }
  }

  dest[pos] = '\0';
  return pos;
}

int format_zero(char *buffer, FormatSpec *spec, char sign_char) {
  int pos = 0;
  if (sign_char) buffer[pos++] = sign_char;
  buffer[pos++] = '0';
  if (spec->flag_hash) buffer[pos++] = '.';
  buffer[pos] = '\0';
  return align_and_pad(buffer, buffer, pos, spec, 0);
}

int convert_to_base(char *buffer, unsigned long long value, int base,
                    int is_upper) {
  const char *digits = is_upper ? "0123456789ABCDEF" : "0123456789abcdef";
  int pos = 0;
  if (value == 0) {
    buffer[pos++] = '0';
  } else {
    while (value > 0) {
      buffer[pos++] = digits[value % base];
      value /= base;
    }
  }
  buffer[pos] = '\0';

  for (int i = 0; i < pos / 2; i++) {
    char tmp = buffer[i];
    buffer[i] = buffer[pos - 1 - i];
    buffer[pos - 1 - i] = tmp;
  }
  return pos;
}

int format_number_with_prefix(char *buffer, const char *number,
                              FormatSpec *spec, const char *prefix) {
  int number_len = s21_strlen(number);
  int prefix_len = prefix ? s21_strlen(prefix) : 0;

  int total_number_len = prefix_len + number_len;

  int padding_zeros = 0;
  if (spec->precision > number_len) {
    padding_zeros = spec->precision - number_len;
  }
  total_number_len += padding_zeros;

  int padding_spaces = 0;
  if (spec->width > total_number_len) {
    padding_spaces = spec->width - total_number_len;
  }

  char pad_char =
      (spec->flag_zero && !spec->flag_minus && spec->precision < 0) ? '0' : ' ';

  int pos = 0;

  if (!spec->flag_minus && pad_char == ' ') {
    for (int i = 0; i < padding_spaces; i++) {
      buffer[pos++] = ' ';
    }
  }

  if (prefix) {
    for (int i = 0; i < prefix_len; i++) {
      buffer[pos++] = prefix[i];
    }
  }

  if (pad_char == '0') {
    for (int i = 0; i < padding_spaces; i++) {
      buffer[pos++] = '0';
    }
  }

  for (int i = 0; i < padding_zeros; i++) {
    buffer[pos++] = '0';
  }

  for (int i = 0; i < number_len; i++) {
    buffer[pos++] = number[i];
  }

  if (spec->flag_minus) {
    for (int i = 0; i < padding_spaces; i++) {
      buffer[pos++] = ' ';
    }
  }

  buffer[pos] = '\0';
  return pos;
}
