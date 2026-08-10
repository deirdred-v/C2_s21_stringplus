#include <check.h>
#include <string.h>

#include "../s21_string.h"

START_TEST(memchr_base) {
  const char data[] = "hello, world";
  int c = 'o';
  size_t n = sizeof(data);

  void *result_std = memchr(data, c, n);
  void *result_my = s21_memchr(data, c, n);

  ck_assert_ptr_eq(result_my, result_std);

  if (result_std != NULL) {
    ck_assert_int_eq(*(char *)result_std, *(char *)result_my);
  }
}
END_TEST

START_TEST(memchr_not_found) {
  const char data[] = "hello, world";
  int c = 'x';
  size_t n = sizeof(data);

  void *result_std = memchr(data, c, n);
  void *result_my = s21_memchr(data, c, n);

  ck_assert_ptr_eq(result_my, result_std);
}
END_TEST

START_TEST(memchr_lim_range) {
  const char data[] = "hello, world";
  int c = 'o';
  size_t n = 5;

  void *result_std = memchr(data, c, n);
  void *result_my = s21_memchr(data, c, n);

  ck_assert_ptr_eq(result_my, result_std);
}
END_TEST
/////////////////////////////////////////////////////////////////////////
START_TEST(memcmp_equal) {
  char str1[] = "Hello, world!";
  char str2[] = "Hello, world!";

  ck_assert_int_eq(s21_memcmp(str1, str2, strlen(str1)),
                   memcmp(str1, str2, strlen(str1)));
}
END_TEST

START_TEST(memcmp_first_small) {
  char str1[] = "Hello";
  char str2[] = "Hellp";

  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
}
END_TEST

START_TEST(memcmp_first_bigger) {
  char str1[] = "Hellp";
  char str2[] = "Hello";

  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
}
END_TEST

START_TEST(memcmp_diff_len) {
  char str1[] = "Hello";
  char str2[] = "Hello, world!";

  ck_assert_int_eq(s21_memcmp(str1, str2, strlen(str1)),
                   memcmp(str1, str2, strlen(str1)));
}
END_TEST

START_TEST(memcmp_zero_len) {
  char str1[] = "";
  char str2[] = "";

  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));
}
END_TEST

START_TEST(memcmp_ultralong) {
  char str1[10000];
  char str2[10000];

  memset(str1, 'A', 10000);
  memset(str2, 'A', 10000);
  str2[5000] = 'B';

  ck_assert_int_eq(s21_memcmp(str1, str2, 10000), memcmp(str1, str2, 10000));
}
END_TEST

START_TEST(memcmp_single_diff) {
  char str1[] = "abcde";
  char str2[] = "abCde";

  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
}
END_TEST

START_TEST(memcmp_null_byte) {
  char str1[] = "abc\0def";
  char str2[] = "abc\0xyz";

  ck_assert_int_eq(s21_memcmp(str1, str2, 8), memcmp(str1, str2, 8));
}
END_TEST
///////////////////////////////////////////////////////////////////////////////////////////////////////
START_TEST(memcpy_simple) {
  char src[] = "Hello, world!";
  char dest1[50];
  char dest2[50];

  ck_assert_mem_eq(s21_memcpy(dest1, src, strlen(src) + 1),
                   memcpy(dest2, src, strlen(src) + 1), strlen(src) + 1);
}
END_TEST

START_TEST(memcpy_empty) {
  char src[] = "";
  char dest1[10];
  char dest2[10];

  ck_assert_mem_eq(s21_memcpy(dest1, src, 1), memcpy(dest2, src, 1), 1);
}
END_TEST

START_TEST(memcpy_large_block) {
  char src[10000];
  char dest1[10000];
  char dest2[10000];

  memset(src, 'A', sizeof(src));

  ck_assert_mem_eq(s21_memcpy(dest1, src, sizeof(src)),
                   memcpy(dest2, src, sizeof(src)), sizeof(src));
}
END_TEST

START_TEST(memcpy_with_nulls) {
  char src[] = "Hello\0world";
  char dest1[20];
  char dest2[20];

  ck_assert_mem_eq(s21_memcpy(dest1, src, sizeof(src)),
                   memcpy(dest2, src, sizeof(src)), sizeof(src));
}
END_TEST

START_TEST(memcpy_partial) {
  char src[] = "Partial copy";
  char dest1[50] = {0};
  char dest2[50] = {0};

  ck_assert_mem_eq(s21_memcpy(dest1, src, 7), memcpy(dest2, src, 7), 7);
}
END_TEST

START_TEST(memcpy_zero_len) {
  char src[] = "Hello";
  char dest1[10];
  char dest2[10];

  ck_assert_mem_eq(s21_memcpy(dest1, src, 0), memcpy(dest2, src, 0), 0);
}
END_TEST

START_TEST(memcpy_diff_len) {
  char src[] = "Short";
  char dest1[50];
  char dest2[50];

  ck_assert_mem_eq(s21_memcpy(dest1, src, strlen(src)),
                   memcpy(dest2, src, strlen(src)), strlen(src));
}
END_TEST

START_TEST(memcpy_struct) {
  struct Data {
    int a;
    double b;
    char c[10];
  };
  struct Data src = {42, 3.14, "Hello"};
  struct Data dest1, dest2;

  ck_assert_mem_eq(s21_memcpy(&dest1, &src, sizeof(struct Data)),
                   memcpy(&dest2, &src, sizeof(struct Data)),
                   sizeof(struct Data));
}
END_TEST
/////////////////////////////////////////////////////////////////////////////////////
START_TEST(memset_full_fill) {
  char str1[10] = "abcdefghi";
  char str2[10] = "abcdefghi";

  ck_assert_mem_eq(s21_memset(str1, 'X', 9), memset(str2, 'X', 9), 10);
}
END_TEST

START_TEST(memset_part_fill) {
  char str1[10] = "abcdefghi";
  char str2[10] = "abcdefghi";

  ck_assert_mem_eq(s21_memset(str1, 'Y', 4), memset(str2, 'Y', 4), 10);
}
END_TEST

START_TEST(memset_fill_with_zeros) {
  char str1[10] = "abcdefghi";
  char str2[10] = "abcdefghi";

  ck_assert_mem_eq(s21_memset(str1, 0, 10), memset(str2, 0, 10), 10);
}
END_TEST

START_TEST(memset_empty_string) {
  char str1[1] = "";
  char str2[1] = "";

  ck_assert_mem_eq(s21_memset(str1, 'A', 1), memset(str2, 'A', 1), 1);
}
END_TEST

START_TEST(memset_single_byte) {
  char str1[10] = "abcdefghi";
  char str2[10] = "abcdefghi";

  ck_assert_mem_eq(s21_memset(str1, 'Q', 1), memset(str2, 'Q', 1), 10);
}
END_TEST

START_TEST(memset_fill_with_null) {
  char str1[10] = "abcdefghi";
  char str2[10] = "abcdefghi";

  ck_assert_mem_eq(s21_memset(str1, '\0', 10), memset(str2, '\0', 10), 10);
}
END_TEST
/////////////////////////////////////////////////////////////////////////////////////
START_TEST(strchr_found) {
  const char *test_str = "Hello, world!";
  char test_char = 'o';

  ck_assert_ptr_eq(s21_strchr(test_str, test_char),
                   strchr(test_str, test_char));
}
END_TEST

START_TEST(strchr_not_found) {
  const char *test_str = "Hello, world!";
  char test_char_not_found = 'x';

  ck_assert_ptr_eq(s21_strchr(test_str, test_char_not_found),
                   strchr(test_str, test_char_not_found));
}
END_TEST

START_TEST(strchr_empt_str) {
  const char *test_str = "";
  char test_char = 'a';

  ck_assert_ptr_eq(s21_strchr(test_str, test_char),
                   strchr(test_str, test_char));
}
END_TEST

START_TEST(strchr_mult_char) {
  const char *test_str = "aaaaa";
  char test_char = 'a';

  ck_assert_ptr_eq(s21_strchr(test_str, test_char),
                   strchr(test_str, test_char));
}
END_TEST

START_TEST(strchr_null_char) {
  const char *test_str = "Hello";
  int null_char = '\0';

  ck_assert_ptr_eq(s21_strchr(test_str, null_char),
                   strchr(test_str, null_char));
}
END_TEST
///////////////////////////////////////////////////////////////////////////////////////////////////////////
START_TEST(strcspn_no_match) {
  const char *test_str1 = "Hello, world!";
  const char *test_str2 = "xyz";

  ck_assert_uint_eq(s21_strcspn(test_str1, test_str2),
                    strcspn(test_str1, test_str2));
}
END_TEST

START_TEST(strcspn_first_char) {
  const char *test_str1 = "Hello, world!";
  const char *test_str2 = "H";

  ck_assert_uint_eq(s21_strcspn(test_str1, test_str2),
                    strcspn(test_str1, test_str2));
}
END_TEST

START_TEST(strcspn_mult_chars) {
  const char *test_str1 = "Hello, world!";
  const char *test_str2 = "lo";

  ck_assert_uint_eq(s21_strcspn(test_str1, test_str2),
                    strcspn(test_str1, test_str2));
}
END_TEST

START_TEST(strcspn_all_char_match) {
  const char *test_str1 = "abc";
  const char *test_str2 = "abc";

  ck_assert_uint_eq(s21_strcspn(test_str1, test_str2),
                    strcspn(test_str1, test_str2));
}
END_TEST

START_TEST(strcspn_empt_first_str) {
  const char *test_str1 = "";
  const char *test_str2 = "abc";

  ck_assert_uint_eq(s21_strcspn(test_str1, test_str2),
                    strcspn(test_str1, test_str2));
}
END_TEST

START_TEST(strcspn_empt_second_str) {
  const char *test_str1 = "Hello, world!";
  const char *test_str2 = "";

  ck_assert_uint_eq(s21_strcspn(test_str1, test_str2),
                    strcspn(test_str1, test_str2));
}
END_TEST

START_TEST(strcspn_rej_mid) {
  const char *str1 = "Hello world";
  const char *reject1 = " ";
  ck_assert_int_eq(s21_strcspn(str1, reject1), strcspn(str1, reject1));
}
END_TEST
//////////////////////////////////////////////////////////////////////////////////
START_TEST(strerror_cyclic) {
  for (int err_code = -200; err_code <= 200; err_code++) {
    char *s21_result = s21_strerror(err_code);
    char *original_result = strerror(err_code);

    ck_assert_str_eq(s21_result, original_result);
  }
}
END_TEST

///////////////////////////////////////////////////////////////////////////////////////////////////
START_TEST(strlen_basic) {
  char str[] = "Hello, world!";

  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(strlen_empty_str) {
  char str[] = "";

  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(strlen_single) {
  char str[] = "A";

  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(strlen_with_sp) {
  char str[] = "Hello, world!  ";

  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(strlen_special_char) {
  char str[] = "\t\n\r\0";

  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST

START_TEST(strlen_unicode) {
  char str[] = "Привет мир!";

  ck_assert_int_eq(s21_strlen(str), strlen(str));
}
END_TEST
//////////////////////////////////////////////////////////////////////////////////
START_TEST(strncat_basic) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  char src[] = "world!";
  size_t n = 6;

  ck_assert_str_eq(s21_strncat(dest1, src, n), strncat(dest2, src, n));
}
END_TEST

START_TEST(strncat_partial) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  char src[] = "world!";
  size_t n = 3;

  ck_assert_str_eq(s21_strncat(dest1, src, n), strncat(dest2, src, n));
}
END_TEST

START_TEST(strncat_zero) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  char src[] = "world!";
  size_t n = 0;

  ck_assert_str_eq(s21_strncat(dest1, src, n), strncat(dest2, src, n));
}
END_TEST

START_TEST(strncat_empty_src) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  char src[] = "";
  size_t n = 5;

  ck_assert_str_eq(s21_strncat(dest1, src, n), strncat(dest2, src, n));
}
END_TEST

START_TEST(strncat_empty_dest) {
  char dest1[50] = "";
  char dest2[50] = "";
  char src[] = "world!";
  size_t n = 6;

  ck_assert_str_eq(s21_strncat(dest1, src, n), strncat(dest2, src, n));
}
END_TEST

START_TEST(strncat_large) {
  char dest1[50] = "Hello, ";
  char dest2[50] = "Hello, ";
  char src[] = "world!";
  size_t n = 50;

  ck_assert_str_eq(s21_strncat(dest1, src, n), strncat(dest2, src, n));
}
END_TEST
////////////////////////////////////////////////////////////////////////////////////////
START_TEST(strncmp_equal_strings) {
  char str1[] = "Hello, world!";
  char str2[] = "Hello, world!";
  size_t n = 5;

  ck_assert_int_eq(s21_strncmp(str1, str2, n), strncmp(str1, str2, n));
}
END_TEST

START_TEST(strncmp_diff_str) {
  char str1[] = "Hello, world!";
  char str2[] = "Hello, universe!";
  size_t n = 7;

  ck_assert_int_eq(s21_strncmp(str1, str2, n), strncmp(str1, str2, n));
}
END_TEST

START_TEST(strncmp_partial_match) {
  char str1[] = "abcdef";
  char str2[] = "abcxyz";
  size_t n = 3;

  ck_assert_int_eq(s21_strncmp(str1, str2, n), strncmp(str1, str2, n));
}
END_TEST

START_TEST(strncmp_one_empt_str) {
  char str1[] = "Hello, world!";
  char str2[] = "";
  size_t n = 5;

  ck_assert_int_eq(s21_strncmp(str1, str2, n), strncmp(str1, str2, n));
}
END_TEST

START_TEST(strncmp_null_terminator) {
  char str1[] = "Hello\0world!";
  char str2[] = "Hello\0universe!";
  size_t n = 10;

  ck_assert_int_eq(s21_strncmp(str1, str2, n), strncmp(str1, str2, n));
}
END_TEST

START_TEST(strncmp_zero) {
  char str1[] = "\0abcder";
  char str2[] = "\0abcderd";
  size_t n = 6;

  ck_assert_int_eq(s21_strncmp(str1, str2, n), strncmp(str1, str2, n));
}
END_TEST

///////////////////////////////////////////////////////////////////////////////////////
START_TEST(strncpy_less_n) {
  char src[] = "Hello";
  char dest1[10] = {0};
  char dest2[10] = {0};
  size_t n = 8;

  ck_assert_str_eq(s21_strncpy(dest1, src, n), strncpy(dest2, src, n));
}
END_TEST

START_TEST(strncpy_more_n) {
  char src[] = "Hello, world!";
  char dest1[20] = {0};
  char dest2[20] = {0};
  size_t n = 5;

  ck_assert_str_eq(s21_strncpy(dest1, src, n), strncpy(dest2, src, n));
}
END_TEST

START_TEST(strncpy_empt_str) {
  char src[] = "";
  char dest1[10] = {0};
  char dest2[10] = {0};
  size_t n = 5;

  ck_assert_str_eq(s21_strncpy(dest1, src, n), strncpy(dest2, src, n));
}
END_TEST

START_TEST(strncpy_n_zero) {
  char src[] = "Hello";
  char dest1[10] = {0};
  char dest2[10] = {0};
  size_t n = 0;

  ck_assert_str_eq(s21_strncpy(dest1, src, n), strncpy(dest2, src, n));
}
END_TEST

START_TEST(strncpy_null_char) {
  char src[] = "Hi\0there";
  char dest1[10] = {0};
  char dest2[10] = {0};
  size_t n = 8;

  ck_assert_mem_eq(s21_strncpy(dest1, src, n), strncpy(dest2, src, n), 10);
}
END_TEST
///////////////////////////////////////////////////////////////
START_TEST(strpbrk_single_match) {
  char str[] = "abcdef";
  char accept[] = "d";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(strpbrk_multi_match) {
  char str[] = "abcdef";
  char accept[] = "dc";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(strpbrk_no_match) {
  char str[] = "abcdef";
  char accept[] = "xyz";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(strpbrk_empt_acc) {
  char str[] = "abcdef";
  char accept[] = "";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(strpbrk_empt_str) {
  char str[] = "";
  char accept[] = "abcdef";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(strpbrk_empt) {
  char str[] = "";
  char accept[] = "";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(strpbrk_first_char_match) {
  char str[] = "abcdef";
  char accept[] = "a";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST

START_TEST(strpbrk_null_terminator) {
  char str[] = "abc\0def";
  char accept[] = "d";

  ck_assert_ptr_eq(s21_strpbrk(str, accept), strpbrk(str, accept));
}
END_TEST
/////////////////////////////////////////////////////////////////////////
START_TEST(strrchr_single_match) {
  char str[] = "abcdef";
  char ch = 'c';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_multi_match) {
  char str[] = "abacada";
  char ch = 'a';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_no_match) {
  char str[] = "abcdef";
  char ch = 'z';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_null_terminator) {
  char str[] = "abc\0def";
  char ch = '\0';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_empt_str) {
  char str[] = "";
  char ch = 'a';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_start) {
  char str[] = "abcdef";
  char ch = 'a';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_end) {
  char str[] = "abcdef";
  char ch = 'f';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_null_middle) {
  char str[] = "abc\0def";
  char ch = 'd';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST

START_TEST(strrchr_single_null) {
  char str[] = "\0";
  char ch = '\0';

  ck_assert_ptr_eq(s21_strrchr(str, ch), strrchr(str, ch));
}
END_TEST
////////////////////////////////////////////////////////////////////////////
START_TEST(strstr_substring_mid) {
  char str[] = "Hello, world!";
  char substr[] = "wo";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_substring_nf) {
  char str[] = "Hello, world!";
  char substr[] = "planet";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_substring_start) {
  char str[] = "Hello, world!";
  char substr[] = "Hello";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_substring_end) {
  char str[] = "Hello, world!";
  char substr[] = "world!";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_empt_substring) {
  char str[] = "Hello, world!";
  char substr[] = "";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_empt_str) {
  char str[] = "";
  char substr[] = "Hello";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_both_empt) {
  char str[] = "";
  char substr[] = "";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_substring_longer) {
  char str[] = "Short";
  char substr[] = "Longer substring";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_single_char_substring) {
  char str[] = "Hello, world!";
  char substr[] = "o";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST

START_TEST(strstr_repeat_char) {
  char str[] = "aaaaabaaaa";
  char substr[] = "ab";

  ck_assert_ptr_eq(s21_strstr(str, substr), strstr(str, substr));
}
END_TEST
/////////////////////////////////////////////////////////////////////////
START_TEST(strtok_basic_test) {
  char str1[] = "Hello, world World, hello";
  char str2[] = "Hello, world World, hello";
  const char delim[] = " ";

  char *tok1 = s21_strtok(str1, delim);
  char *tok2 = strtok(str2, delim);

  while (tok1 != NULL && tok2 != NULL) {
    ck_assert_str_eq(tok1, tok2);
    tok1 = s21_strtok(NULL, delim);
    tok2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(tok1, tok2);
}
END_TEST

START_TEST(strtok_basic_test_null) {
  char str1[] = "Hello, world World, hello";
  char str2[] = "Hello, world World, hello";
  const char delim[] = "\0";

  char *tok1 = s21_strtok(str1, delim);
  char *tok2 = strtok(str2, delim);

  while (tok1 != NULL && tok2 != NULL) {
    ck_assert_str_eq(tok1, tok2);
    tok1 = s21_strtok(NULL, delim);
    tok2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(tok1, tok2);
}
END_TEST

START_TEST(strtok_empty_string) {
  char str1[] = "";
  char str2[] = "";
  const char delim[] = " ";

  char *tok1 = s21_strtok(str1, delim);
  char *tok2 = strtok(str2, delim);

  ck_assert_ptr_eq(tok1, tok2);
}
END_TEST

START_TEST(strtok_no_delimiters) {
  char str1[] = "HelloWorld";
  char str2[] = "HelloWorld";
  const char delim[] = ",";

  char *tok1 = s21_strtok(str1, delim);
  char *tok2 = strtok(str2, delim);

  while (tok1 != NULL && tok2 != NULL) {
    ck_assert_str_eq(tok1, tok2);
    tok1 = s21_strtok(NULL, delim);
    tok2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(tok1, tok2);
}
END_TEST

START_TEST(strtok_multi_delimiters) {
  char str1[] = ";;Hello;;;World;;;";
  char str2[] = ";;Hello;;;World;;;";
  const char delim[] = ";";

  char *tok1 = s21_strtok(str1, delim);
  char *tok2 = strtok(str2, delim);

  while (tok1 != NULL && tok2 != NULL) {
    ck_assert_str_eq(tok1, tok2);
    tok1 = s21_strtok(NULL, delim);
    tok2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(tok1, tok2);
}
END_TEST

START_TEST(strtok_only_delimiters) {
  char str1[] = ";;;;;;";
  char str2[] = ";;;;;;";
  const char delim[] = ";";

  char *tok1 = s21_strtok(str1, delim);
  char *tok2 = strtok(str2, delim);

  ck_assert_ptr_eq(tok1, tok2);
}
END_TEST
/////////////////////////////////////////////////////////////////////////
////////////////////////SPRINTF//////////////////////////////////////////
START_TEST(test_sprintf) {
  char s21_buf[100];
  char orig_buf[100];

  char *value = NULL;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%s", value),
                   sprintf(orig_buf, "%s", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_c) {
  char s21_buf[100];
  char orig_buf[100];

  char value = 'A';
  ck_assert_int_eq(s21_sprintf(s21_buf, "%c", value),
                   sprintf(orig_buf, "%c", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_d) {
  char s21_buf[100];
  char orig_buf[100];

  int value = -123;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%d", value),
                   sprintf(orig_buf, "%d", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 123.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%f", value),
                   sprintf(orig_buf, "%f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_s) {
  char s21_buf[100];
  char orig_buf[100];

  char *value = "Hello, world!";
  ck_assert_int_eq(s21_sprintf(s21_buf, "%s", value),
                   sprintf(orig_buf, "%s", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_u) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 4294967295;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%u", value),
                   sprintf(orig_buf, "%u", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_percent) {
  char s21_buf[100];
  char orig_buf[100];

  ck_assert_int_eq(s21_sprintf(s21_buf, "%%"), sprintf(orig_buf, "%%"));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%g", value),
                   sprintf(orig_buf, "%g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%G", value),
                   sprintf(orig_buf, "%G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e) {
  char s21_buf[100];
  char orig_buf[100];

  double value = -12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%e", value),
                   sprintf(orig_buf, "%e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%E", value),
                   sprintf(orig_buf, "%E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%x", value),
                   sprintf(orig_buf, "%x", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%X", value),
                   sprintf(orig_buf, "%X", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%o", value),
                   sprintf(orig_buf, "%o", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_p) {
  char s21_buf[100];
  char orig_buf[100];

  void *value = (void *)0x12345678;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%p", value),
                   sprintf(orig_buf, "%p", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

// Все спецификаторы с флагом -
START_TEST(test_sprintf_c_minus) {
  char s21_buf[100];
  char orig_buf[100];

  char value = 'A';
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-10c", value),
                   sprintf(orig_buf, "%-10c", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_d_minus) {
  char s21_buf[100];
  char orig_buf[100];

  int value = -123;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-10d", value),
                   sprintf(orig_buf, "%-10d", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_minus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-15f", value),
                   sprintf(orig_buf, "%-15f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_s_minus) {
  char s21_buf[100];
  char orig_buf[100];

  char *value = "Hello";
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-10s", value),
                   sprintf(orig_buf, "%-10s", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_u_minus) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 12345;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-8u", value),
                   sprintf(orig_buf, "%-8u", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_minus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-10g", value),
                   sprintf(orig_buf, "%-10g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_minus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-12G", value),
                   sprintf(orig_buf, "%-12G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_minus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-15e", value),
                   sprintf(orig_buf, "%-15e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_minus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-20E", value),
                   sprintf(orig_buf, "%-20E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x_minus) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-10x", value),
                   sprintf(orig_buf, "%-10x", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X_minus) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-10X", value),
                   sprintf(orig_buf, "%-10X", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o_minus) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-10o", value),
                   sprintf(orig_buf, "%-10o", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_p_minus) {
  char s21_buf[100];
  char orig_buf[100];

  void *value = (void *)0x12345678;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%-15p", value),
                   sprintf(orig_buf, "%-15p", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST
// Все спецификаторы с флагом +
START_TEST(test_sprintf_d_plus) {
  char s21_buf[100];
  char orig_buf[100];

  int value = 123;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%+d", value),
                   sprintf(orig_buf, "%+d", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_plus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%+f", value),
                   sprintf(orig_buf, "%+f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_plus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 0.0;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%+g", value),
                   sprintf(orig_buf, "%+g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_plus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%+G", value),
                   sprintf(orig_buf, "%+G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_plus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%+e", value),
                   sprintf(orig_buf, "%+e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_plus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%+E", value),
                   sprintf(orig_buf, "%+E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST
// Флаг пробел
START_TEST(test_sprintf_d_space) {
  char s21_buf[100];
  char orig_buf[100];

  int value = 123;
  ck_assert_int_eq(s21_sprintf(s21_buf, "% d", value),
                   sprintf(orig_buf, "% d", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_space) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "% 010f", value),
                   sprintf(orig_buf, "% 010f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_space) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "% g", value),
                   sprintf(orig_buf, "% g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_space) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "% G", value),
                   sprintf(orig_buf, "% G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_space) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "% e", value),
                   sprintf(orig_buf, "% e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_space) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "% E", value),
                   sprintf(orig_buf, "% E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST
// Флаг #
START_TEST(test_sprintf_f_hash) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#f", value),
                   sprintf(orig_buf, "%#f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_hash) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 145.678;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#.3g", value),
                   sprintf(orig_buf, "%#.3g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_hash) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#G", value),
                   sprintf(orig_buf, "%#G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_hash) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#e", value),
                   sprintf(orig_buf, "%#e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_hash) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#E", value),
                   sprintf(orig_buf, "%#E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x_hash) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#x", value),
                   sprintf(orig_buf, "%#x", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X_hash) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#X", value),
                   sprintf(orig_buf, "%#X", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o_hash) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%#o", value),
                   sprintf(orig_buf, "%#o", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST
// Флаг 0
START_TEST(test_sprintf_d_zero) {
  char s21_buf[100];
  char orig_buf[100];

  int value = 123;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010d", value),
                   sprintf(orig_buf, "%010d", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_zero) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010f", value),
                   sprintf(orig_buf, "%010f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_zero_plus) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%+010f", value),
                   sprintf(orig_buf, "%+010f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_u_zero) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 12345;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010u", value),
                   sprintf(orig_buf, "%010u", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_zero) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010g", value),
                   sprintf(orig_buf, "%010g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_zero) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010G", value),
                   sprintf(orig_buf, "%010G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_zero) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010e", value),
                   sprintf(orig_buf, "%010e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_zero) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010E", value),
                   sprintf(orig_buf, "%010E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x_zero) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010x", value),
                   sprintf(orig_buf, "%010x", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X_zero) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010X", value),
                   sprintf(orig_buf, "%010X", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o_zero) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%010o", value),
                   sprintf(orig_buf, "%010o", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST
// Ширина
START_TEST(test_sprintf_c_width) {
  char s21_buf[100];
  char orig_buf[100];

  char value = 'A';
  ck_assert_int_eq(s21_sprintf(s21_buf, "%5c", value),
                   sprintf(orig_buf, "%5c", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_d_width) {
  char s21_buf[100];
  char orig_buf[100];

  int value = 123;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10d", value),
                   sprintf(orig_buf, "%10d", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_width) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10f", value),
                   sprintf(orig_buf, "%10f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_s_width) {
  char s21_buf[100];
  char orig_buf[100];

  char *value = "Hello";
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10s", value),
                   sprintf(orig_buf, "%10s", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_u_width) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 12345;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10u", value),
                   sprintf(orig_buf, "%10u", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_width) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10g", value),
                   sprintf(orig_buf, "%10g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_width) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10G", value),
                   sprintf(orig_buf, "%10G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_width) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10e", value),
                   sprintf(orig_buf, "%10e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_width) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10E", value),
                   sprintf(orig_buf, "%10E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x_width) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10x", value),
                   sprintf(orig_buf, "%10x", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X_width) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10X", value),
                   sprintf(orig_buf, "%10X", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o_width) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10o", value),
                   sprintf(orig_buf, "%10o", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

// Ширина *
START_TEST(test_sprintf_c_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  char value = 'A';
  int width = 5;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*c", width, value),
                   sprintf(orig_buf, "%*c", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_d_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  int value = -123;
  int width = -10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*d", width, value),
                   sprintf(orig_buf, "%*d", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*f", width, value),
                   sprintf(orig_buf, "%*f", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_s_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  char *value = "Hello";
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*s", width, value),
                   sprintf(orig_buf, "%*s", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_u_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 12345;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*u", width, value),
                   sprintf(orig_buf, "%*u", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*g", width, value),
                   sprintf(orig_buf, "%*g", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*G", width, value),
                   sprintf(orig_buf, "%*G", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*e", width, value),
                   sprintf(orig_buf, "%*e", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*E", width, value),
                   sprintf(orig_buf, "%*E", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*x", width, value),
                   sprintf(orig_buf, "%*x", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*X", width, value),
                   sprintf(orig_buf, "%*X", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o_width_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  int width = 10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*o", width, value),
                   sprintf(orig_buf, "%*o", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_d_width_star_zero) {
  char s21_buf[100];
  char orig_buf[100];

  int value = -123;
  int width = 0.0;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%*d", width, value),
                   sprintf(orig_buf, "%*d", width, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

// Точность
START_TEST(test_sprintf_d_precision) {
  char s21_buf[100];
  char orig_buf[100];

  int value = 123;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.5d", value),
                   sprintf(orig_buf, "%.5d", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_precision) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.14159;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.4f", value),
                   sprintf(orig_buf, "%.4f", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_s_precision) {
  char s21_buf[100];
  char orig_buf[100];

  char *value = "Hello, world!";
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.5s", value),
                   sprintf(orig_buf, "%.5s", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_u_precision) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 12345;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.8u", value),
                   sprintf(orig_buf, "%.8u", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_precision) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 123.678;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.3g", value),
                   sprintf(orig_buf, "%.3g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_precision) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.6G", value),
                   sprintf(orig_buf, "%.6G", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_precision) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.5e", value),
                   sprintf(orig_buf, "%.5e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_precision) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.5E", value),
                   sprintf(orig_buf, "%.5E", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x_precision) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.8x", value),
                   sprintf(orig_buf, "%.8x", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X_precision) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.8X", value),
                   sprintf(orig_buf, "%.8X", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o_precision) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.5o", value),
                   sprintf(orig_buf, "%.5o", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

// Точность через *
START_TEST(test_sprintf_d_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  int value = 123;
  int precision = 5;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*d", precision, value),
                   sprintf(orig_buf, "%.*d", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_f_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = -3.14159;
  int precision = 4;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*f", precision, value),
                   sprintf(orig_buf, "%.*f", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_s_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  char *value = "Hello, world!";
  int precision = 5;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*s", precision, value),
                   sprintf(orig_buf, "%.*s", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_u_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 12345;
  int precision = 8;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*u", precision, value),
                   sprintf(orig_buf, "%.*u", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_g_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int precision = 6;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*g", precision, value),
                   sprintf(orig_buf, "%.*g", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_G_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int precision = 6;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*G", precision, value),
                   sprintf(orig_buf, "%.*G", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int precision = 15;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*e", precision, value),
                   sprintf(orig_buf, "%.*e", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_E_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  int precision = 5;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*E", precision, value),
                   sprintf(orig_buf, "%.*E", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_x_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  int precision = 8;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*x", precision, value),
                   sprintf(orig_buf, "%.*x", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_X_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 305419896;
  int precision = 8;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*X", precision, value),
                   sprintf(orig_buf, "%.*X", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_o_precision_star) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned int value = 255;
  int precision = 5;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.*o", precision, value),
                   sprintf(orig_buf, "%.*o", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

// Длина h
START_TEST(test_sprintf_h_d) {
  char s21_buf[100];
  char orig_buf[100];

  short value = 12345;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%hd", value),
                   sprintf(orig_buf, "%hd", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_h_u) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned short value = 54321;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%hu", value),
                   sprintf(orig_buf, "%hu", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_h_x) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned short value = 0x7FFF;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%hx", value),
                   sprintf(orig_buf, "%hx", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_h_X) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned short value = 0x7FFF;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%hX", value),
                   sprintf(orig_buf, "%hX", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_h_o) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned short value = 07777;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%ho", value),
                   sprintf(orig_buf, "%ho", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

// Длина l
START_TEST(test_sprintf_l_d) {
  char s21_buf[100];
  char orig_buf[100];

  long value = 1234567890;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%ld", value),
                   sprintf(orig_buf, "%ld", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_u) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned long value = 9876543210;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lu", value),
                   sprintf(orig_buf, "%lu", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_x) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned long value = 0xFFFFFFFF;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lx", value),
                   sprintf(orig_buf, "%lx", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_X) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned long value = 0xFFFFFFFF;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lX", value),
                   sprintf(orig_buf, "%lX", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_o) {
  char s21_buf[100];
  char orig_buf[100];

  unsigned long value = 077777777777;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lo", value),
                   sprintf(orig_buf, "%lo", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_f) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 3.141592653589793;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lf", value),
                   sprintf(orig_buf, "%lf", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_g) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lg", value),
                   sprintf(orig_buf, "%lg", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_G) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 12345.6789;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lG", value),
                   sprintf(orig_buf, "%lG", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_e) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 1.234567e10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%le", value),
                   sprintf(orig_buf, "%le", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_l_E) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 1.234567e10;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%lE", value),
                   sprintf(orig_buf, "%lE", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

// Длина L
START_TEST(test_sprintf_L_f) {
  char s21_buf[100];
  char orig_buf[100];

  long double value = 3.141592653589793238462643383279;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%Lf", value),
                   sprintf(orig_buf, "%Lf", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_L_g) {
  char s21_buf[100];
  char orig_buf[100];

  long double value = 12345.67890123456789L;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%Lg", value),
                   sprintf(orig_buf, "%Lg", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_L_G) {
  char s21_buf[100];
  char orig_buf[100];

  long double value = 12345.67890123456789L;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%LG", value),
                   sprintf(orig_buf, "%LG", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_L_e) {
  char s21_buf[100];
  char orig_buf[100];

  long double value = 1.234567890123456789e+10L;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%Le", value),
                   sprintf(orig_buf, "%Le", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_L_E) {
  char s21_buf[100];
  char orig_buf[100];

  long double value = 1.234567890123456789e+10L;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%LE", value),
                   sprintf(orig_buf, "%LE", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST
///////////////////////////////////////////////////////////////////////////////
// Дополнительные тесты
START_TEST(test_sprintf_L_E_t) {
  char s21_buf[128];
  char orig_buf[128];
  unsigned int value = 0;

  int s21_len = s21_sprintf(s21_buf, "%.0x", value);
  int orig_len = sprintf(orig_buf, "%.0x", value);

  ck_assert_int_eq(s21_len, orig_len);
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_L_E_tt) {
  char s21_buf[128];
  char orig_buf[128];
  unsigned int value = 0;

  int s21_len = s21_sprintf(s21_buf, "%.3x", value);
  int orig_len = sprintf(orig_buf, "%.3x", value);

  ck_assert_int_eq(s21_len, orig_len);
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_sprintf_e_zero_val) {
  char s21_buf[100];
  char orig_buf[100];
  double value = 0.0;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%.3e", value),
                   sprintf(orig_buf, "%.3e", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(sprintf_specifier_Le_zero_precision) {
  char buffer1[100];
  char buffer2[100];
  long double num = 0.0;

  s21_sprintf(buffer1, "%.Lg", num);
  sprintf(buffer2, "%.Lg", num);
  ck_assert_str_eq(buffer1, buffer2);
}
END_TEST

START_TEST(test_null_p) {
  char s21_buf[100];
  char orig_buf[100];

  void *value = NULL;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%p", value),
                   sprintf(orig_buf, "%p", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_separator) {
  char s21_buf[100];
  char orig_buf[100];
  int value = 5;

  ck_assert_int_eq(s21_sprintf(s21_buf, "|%-5d|", value),
                   sprintf(orig_buf, "|%-5d|", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_precision_negative) {
  char s21_buf[100];
  char orig_buf[100];

  double value = -3.14159;
  int precision = -20;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10.*f", precision, value),
                   sprintf(orig_buf, "%10.*f", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_f_zero) {
  char s21_buf[100];
  char orig_buf[100];

  double value = 0;
  int precision = 0;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%10.*f", precision, value),
                   sprintf(orig_buf, "%10.*f", precision, value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(test_g_negative) {
  char s21_buf[100];
  char orig_buf[100];

  double value = -145.678;
  ck_assert_int_eq(s21_sprintf(s21_buf, "%g", value),
                   sprintf(orig_buf, "%g", value));
  ck_assert_str_eq(s21_buf, orig_buf);
}
END_TEST

START_TEST(sprintf_width_star_G) {
  char buffer1[100];
  char buffer2[100];
  double num = 0.000123456;
  int width = 20;

  s21_sprintf(buffer1, "%*G", width, num);
  sprintf(buffer2, "%*G", width, num);
  ck_assert_str_eq(buffer1, buffer2);
}
END_TEST

Suite *memchr_suite() {
  Suite *s;
  TCase *tc_memchr;

  s = suite_create("memchr");
  tc_memchr = tcase_create("memchr");
  tcase_add_test(tc_memchr, memchr_base);
  tcase_add_test(tc_memchr, memchr_not_found);
  tcase_add_test(tc_memchr, memchr_lim_range);

  suite_add_tcase(s, tc_memchr);
  return s;
}

Suite *memcmp_suite() {
  Suite *s;
  TCase *tc_memcmp;

  s = suite_create("memcmp");
  tc_memcmp = tcase_create("memcmp");
  tcase_add_test(tc_memcmp, memcmp_equal);
  tcase_add_test(tc_memcmp, memcmp_first_small);
  tcase_add_test(tc_memcmp, memcmp_first_bigger);
  tcase_add_test(tc_memcmp, memcmp_diff_len);
  tcase_add_test(tc_memcmp, memcmp_zero_len);
  tcase_add_test(tc_memcmp, memcmp_ultralong);
  tcase_add_test(tc_memcmp, memcmp_single_diff);
  tcase_add_test(tc_memcmp, memcmp_null_byte);

  suite_add_tcase(s, tc_memcmp);
  return s;
}

Suite *memcpy_suite() {
  Suite *s;
  TCase *tc_memcpy;

  s = suite_create("memcpy");
  tc_memcpy = tcase_create("memcpy");
  tcase_add_test(tc_memcpy, memcpy_simple);
  tcase_add_test(tc_memcpy, memcpy_empty);
  tcase_add_test(tc_memcpy, memcpy_large_block);
  tcase_add_test(tc_memcpy, memcpy_with_nulls);
  tcase_add_test(tc_memcpy, memcpy_partial);
  tcase_add_test(tc_memcpy, memcpy_zero_len);
  tcase_add_test(tc_memcpy, memcpy_diff_len);
  tcase_add_test(tc_memcpy, memcpy_struct);

  suite_add_tcase(s, tc_memcpy);
  return s;
}

Suite *memset_suite() {
  Suite *s;
  TCase *tc_memset;

  s = suite_create("memset");
  tc_memset = tcase_create("memset");
  tcase_add_test(tc_memset, memset_full_fill);
  tcase_add_test(tc_memset, memset_part_fill);
  tcase_add_test(tc_memset, memset_fill_with_zeros);
  tcase_add_test(tc_memset, memset_empty_string);
  tcase_add_test(tc_memset, memset_single_byte);
  tcase_add_test(tc_memset, memset_fill_with_null);

  suite_add_tcase(s, tc_memset);
  return s;
}

Suite *strchr_suite() {
  Suite *s;
  TCase *tc_strchr;

  s = suite_create("strchr");
  tc_strchr = tcase_create("strchr");
  tcase_add_test(tc_strchr, strchr_found);
  tcase_add_test(tc_strchr, strchr_not_found);
  tcase_add_test(tc_strchr, strchr_empt_str);
  tcase_add_test(tc_strchr, strchr_mult_char);
  tcase_add_test(tc_strchr, strchr_null_char);

  suite_add_tcase(s, tc_strchr);
  return s;
}
Suite *strcspn_suite() {
  Suite *s;
  TCase *tc_strcspn;

  s = suite_create("strcspn");
  tc_strcspn = tcase_create("strcspn");
  tcase_add_test(tc_strcspn, strcspn_no_match);
  tcase_add_test(tc_strcspn, strcspn_first_char);
  tcase_add_test(tc_strcspn, strcspn_mult_chars);
  tcase_add_test(tc_strcspn, strcspn_all_char_match);
  tcase_add_test(tc_strcspn, strcspn_empt_first_str);
  tcase_add_test(tc_strcspn, strcspn_empt_second_str);
  tcase_add_test(tc_strcspn, strcspn_rej_mid);

  suite_add_tcase(s, tc_strcspn);
  return s;
}

Suite *strerror_suite() {
  Suite *s;
  TCase *tc_strerror;

  s = suite_create("strerror");
  tc_strerror = tcase_create("strerror");
  tcase_add_test(tc_strerror, strerror_cyclic);

  suite_add_tcase(s, tc_strerror);
  return s;
}

Suite *strlen_suite() {
  Suite *s;
  TCase *tc_strlen;

  s = suite_create("strlen");
  tc_strlen = tcase_create("strlen");
  tcase_add_test(tc_strlen, strlen_basic);
  tcase_add_test(tc_strlen, strlen_empty_str);
  tcase_add_test(tc_strlen, strlen_single);
  tcase_add_test(tc_strlen, strlen_with_sp);
  tcase_add_test(tc_strlen, strlen_special_char);
  tcase_add_test(tc_strlen, strlen_unicode);
  suite_add_tcase(s, tc_strlen);
  return s;
}

Suite *strncat_suite() {
  Suite *s;
  TCase *tc_strncat;

  s = suite_create("strncat");
  tc_strncat = tcase_create("strncat");
  tcase_add_test(tc_strncat, strncat_basic);
  tcase_add_test(tc_strncat, strncat_partial);
  tcase_add_test(tc_strncat, strncat_zero);
  tcase_add_test(tc_strncat, strncat_empty_src);
  tcase_add_test(tc_strncat, strncat_empty_dest);
  tcase_add_test(tc_strncat, strncat_large);

  suite_add_tcase(s, tc_strncat);
  return s;
}

Suite *strncmp_suite() {
  Suite *s;
  TCase *tc_strncmp;

  s = suite_create("strncmp");
  tc_strncmp = tcase_create("strncmp");
  tcase_add_test(tc_strncmp, strncmp_equal_strings);
  tcase_add_test(tc_strncmp, strncmp_diff_str);
  tcase_add_test(tc_strncmp, strncmp_partial_match);
  tcase_add_test(tc_strncmp, strncmp_one_empt_str);
  tcase_add_test(tc_strncmp, strncmp_null_terminator);
  tcase_add_test(tc_strncmp, strncmp_zero);

  suite_add_tcase(s, tc_strncmp);
  return s;
}

Suite *strncpy_suite() {
  Suite *s;
  TCase *tc_strncpy;

  s = suite_create("strncpy");
  tc_strncpy = tcase_create("strncpy");
  tcase_add_test(tc_strncpy, strncpy_less_n);
  tcase_add_test(tc_strncpy, strncpy_more_n);
  tcase_add_test(tc_strncpy, strncpy_empt_str);
  tcase_add_test(tc_strncpy, strncpy_n_zero);
  tcase_add_test(tc_strncpy, strncpy_null_char);

  suite_add_tcase(s, tc_strncpy);
  return s;
}

Suite *strpbrk_suite() {
  Suite *s;
  TCase *tc_strpbrk;

  s = suite_create("strpbrk");
  tc_strpbrk = tcase_create("strpbrk");
  tcase_add_test(tc_strpbrk, strpbrk_single_match);
  tcase_add_test(tc_strpbrk, strpbrk_multi_match);
  tcase_add_test(tc_strpbrk, strpbrk_no_match);
  tcase_add_test(tc_strpbrk, strpbrk_empt_acc);
  tcase_add_test(tc_strpbrk, strpbrk_empt_str);
  tcase_add_test(tc_strpbrk, strpbrk_empt);
  tcase_add_test(tc_strpbrk, strpbrk_first_char_match);
  tcase_add_test(tc_strpbrk, strpbrk_null_terminator);

  suite_add_tcase(s, tc_strpbrk);
  return s;
}

Suite *strrchr_suite(void) {
  Suite *s = suite_create("strrchr");
  TCase *tc_strrchr = tcase_create("strrchr");

  tcase_add_test(tc_strrchr, strrchr_single_match);
  tcase_add_test(tc_strrchr, strrchr_multi_match);
  tcase_add_test(tc_strrchr, strrchr_no_match);
  tcase_add_test(tc_strrchr, strrchr_null_terminator);
  tcase_add_test(tc_strrchr, strrchr_empt_str);
  tcase_add_test(tc_strrchr, strrchr_start);
  tcase_add_test(tc_strrchr, strrchr_end);
  tcase_add_test(tc_strrchr, strrchr_null_middle);
  tcase_add_test(tc_strrchr, strrchr_single_null);

  suite_add_tcase(s, tc_strrchr);
  return s;
}

Suite *strstr_suite(void) {
  Suite *s = suite_create("strstr");
  TCase *tc_strstr = tcase_create("strstr");

  tcase_add_test(tc_strstr, strstr_substring_mid);
  tcase_add_test(tc_strstr, strstr_substring_nf);
  tcase_add_test(tc_strstr, strstr_substring_start);
  tcase_add_test(tc_strstr, strstr_substring_end);
  tcase_add_test(tc_strstr, strstr_empt_substring);
  tcase_add_test(tc_strstr, strstr_empt_str);
  tcase_add_test(tc_strstr, strstr_both_empt);
  tcase_add_test(tc_strstr, strstr_substring_longer);
  tcase_add_test(tc_strstr, strstr_single_char_substring);
  tcase_add_test(tc_strstr, strstr_repeat_char);

  suite_add_tcase(s, tc_strstr);
  return s;
}

Suite *strtok_suite(void) {
  Suite *s = suite_create("strtok");
  TCase *tc_strtok = tcase_create("strtok");

  tcase_add_test(tc_strtok, strtok_basic_test);
  tcase_add_test(tc_strtok, strtok_basic_test_null);
  tcase_add_test(tc_strtok, strtok_empty_string);
  tcase_add_test(tc_strtok, strtok_no_delimiters);
  tcase_add_test(tc_strtok, strtok_multi_delimiters);
  tcase_add_test(tc_strtok, strtok_only_delimiters);

  suite_add_tcase(s, tc_strtok);
  return s;
}

Suite *sprintf_suite(void) {
  Suite *s = suite_create("sprintf");
  TCase *tc_sprintf = tcase_create("sprintf");
  tcase_add_test(tc_sprintf, test_sprintf);
  tcase_add_test(tc_sprintf, test_sprintf_c);
  tcase_add_test(tc_sprintf, test_sprintf_d);
  tcase_add_test(tc_sprintf, test_sprintf_f);
  tcase_add_test(tc_sprintf, test_sprintf_s);
  tcase_add_test(tc_sprintf, test_sprintf_u);
  tcase_add_test(tc_sprintf, test_sprintf_percent);
  tcase_add_test(tc_sprintf, test_sprintf_g);
  tcase_add_test(tc_sprintf, test_sprintf_G);
  tcase_add_test(tc_sprintf, test_sprintf_e);
  tcase_add_test(tc_sprintf, test_sprintf_E);
  tcase_add_test(tc_sprintf, test_sprintf_x);
  tcase_add_test(tc_sprintf, test_sprintf_X);
  tcase_add_test(tc_sprintf, test_sprintf_o);
  tcase_add_test(tc_sprintf, test_sprintf_p);

  tcase_add_test(tc_sprintf, test_sprintf_c_minus);
  tcase_add_test(tc_sprintf, test_sprintf_d_minus);
  tcase_add_test(tc_sprintf, test_sprintf_f_minus);
  tcase_add_test(tc_sprintf, test_sprintf_s_minus);
  tcase_add_test(tc_sprintf, test_sprintf_u_minus);
  tcase_add_test(tc_sprintf, test_sprintf_g_minus);
  tcase_add_test(tc_sprintf, test_sprintf_G_minus);
  tcase_add_test(tc_sprintf, test_sprintf_e_minus);
  tcase_add_test(tc_sprintf, test_sprintf_E_minus);
  tcase_add_test(tc_sprintf, test_sprintf_x_minus);
  tcase_add_test(tc_sprintf, test_sprintf_X_minus);
  tcase_add_test(tc_sprintf, test_sprintf_o_minus);
  tcase_add_test(tc_sprintf, test_sprintf_p_minus);

  tcase_add_test(tc_sprintf, test_sprintf_d_plus);
  tcase_add_test(tc_sprintf, test_sprintf_f_plus);
  tcase_add_test(tc_sprintf, test_sprintf_g_plus);
  tcase_add_test(tc_sprintf, test_sprintf_G_plus);
  tcase_add_test(tc_sprintf, test_sprintf_e_plus);
  tcase_add_test(tc_sprintf, test_sprintf_E_plus);

  tcase_add_test(tc_sprintf, test_sprintf_d_space);
  tcase_add_test(tc_sprintf, test_sprintf_f_space);
  tcase_add_test(tc_sprintf, test_sprintf_g_space);
  tcase_add_test(tc_sprintf, test_sprintf_G_space);
  tcase_add_test(tc_sprintf, test_sprintf_e_space);
  tcase_add_test(tc_sprintf, test_sprintf_E_space);

  tcase_add_test(tc_sprintf, test_sprintf_f_hash);
  tcase_add_test(tc_sprintf, test_sprintf_g_hash);
  tcase_add_test(tc_sprintf, test_sprintf_G_hash);
  tcase_add_test(tc_sprintf, test_sprintf_e_hash);
  tcase_add_test(tc_sprintf, test_sprintf_E_hash);
  tcase_add_test(tc_sprintf, test_sprintf_x_hash);
  tcase_add_test(tc_sprintf, test_sprintf_X_hash);
  tcase_add_test(tc_sprintf, test_sprintf_o_hash);

  tcase_add_test(tc_sprintf, test_sprintf_d_zero);
  tcase_add_test(tc_sprintf, test_sprintf_f_zero);
  tcase_add_test(tc_sprintf, test_sprintf_f_zero_plus);
  tcase_add_test(tc_sprintf, test_sprintf_u_zero);
  tcase_add_test(tc_sprintf, test_sprintf_g_zero);
  tcase_add_test(tc_sprintf, test_sprintf_G_zero);
  tcase_add_test(tc_sprintf, test_sprintf_e_zero);
  tcase_add_test(tc_sprintf, test_sprintf_E_zero);
  tcase_add_test(tc_sprintf, test_sprintf_x_zero);
  tcase_add_test(tc_sprintf, test_sprintf_X_zero);
  tcase_add_test(tc_sprintf, test_sprintf_o_zero);

  tcase_add_test(tc_sprintf, test_sprintf_c_width);
  tcase_add_test(tc_sprintf, test_sprintf_d_width);
  tcase_add_test(tc_sprintf, test_sprintf_f_width);
  tcase_add_test(tc_sprintf, test_sprintf_s_width);
  tcase_add_test(tc_sprintf, test_sprintf_u_width);
  tcase_add_test(tc_sprintf, test_sprintf_g_width);
  tcase_add_test(tc_sprintf, test_sprintf_G_width);
  tcase_add_test(tc_sprintf, test_sprintf_e_width);
  tcase_add_test(tc_sprintf, test_sprintf_E_width);
  tcase_add_test(tc_sprintf, test_sprintf_x_width);
  tcase_add_test(tc_sprintf, test_sprintf_X_width);
  tcase_add_test(tc_sprintf, test_sprintf_o_width);

  tcase_add_test(tc_sprintf, test_sprintf_c_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_d_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_f_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_s_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_u_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_g_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_G_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_e_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_E_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_x_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_X_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_o_width_star);
  tcase_add_test(tc_sprintf, test_sprintf_d_width_star_zero);

  tcase_add_test(tc_sprintf, test_sprintf_d_precision);
  tcase_add_test(tc_sprintf, test_sprintf_f_precision);
  tcase_add_test(tc_sprintf, test_sprintf_s_precision);
  tcase_add_test(tc_sprintf, test_sprintf_u_precision);
  tcase_add_test(tc_sprintf, test_sprintf_g_precision);
  tcase_add_test(tc_sprintf, test_sprintf_G_precision);
  tcase_add_test(tc_sprintf, test_sprintf_e_precision);
  tcase_add_test(tc_sprintf, test_sprintf_E_precision);
  tcase_add_test(tc_sprintf, test_sprintf_x_precision);
  tcase_add_test(tc_sprintf, test_sprintf_X_precision);
  tcase_add_test(tc_sprintf, test_sprintf_o_precision);

  tcase_add_test(tc_sprintf, test_sprintf_d_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_f_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_s_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_u_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_g_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_G_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_e_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_E_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_x_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_X_precision_star);
  tcase_add_test(tc_sprintf, test_sprintf_o_precision_star);

  tcase_add_test(tc_sprintf, test_sprintf_h_d);
  tcase_add_test(tc_sprintf, test_sprintf_h_u);
  tcase_add_test(tc_sprintf, test_sprintf_h_x);
  tcase_add_test(tc_sprintf, test_sprintf_h_X);
  tcase_add_test(tc_sprintf, test_sprintf_h_o);

  tcase_add_test(tc_sprintf, test_sprintf_l_d);
  tcase_add_test(tc_sprintf, test_sprintf_l_f);
  tcase_add_test(tc_sprintf, test_sprintf_l_u);
  tcase_add_test(tc_sprintf, test_sprintf_l_g);
  tcase_add_test(tc_sprintf, test_sprintf_l_G);
  tcase_add_test(tc_sprintf, test_sprintf_l_e);
  tcase_add_test(tc_sprintf, test_sprintf_l_E);
  tcase_add_test(tc_sprintf, test_sprintf_l_x);
  tcase_add_test(tc_sprintf, test_sprintf_l_X);
  tcase_add_test(tc_sprintf, test_sprintf_l_o);

  tcase_add_test(tc_sprintf, test_sprintf_L_f);
  tcase_add_test(tc_sprintf, test_sprintf_L_g);
  tcase_add_test(tc_sprintf, test_sprintf_L_G);
  tcase_add_test(tc_sprintf, test_sprintf_L_e);
  tcase_add_test(tc_sprintf, test_sprintf_L_E);

  tcase_add_test(tc_sprintf, test_sprintf_L_E_t);
  tcase_add_test(tc_sprintf, test_sprintf_L_E_tt);
  tcase_add_test(tc_sprintf, test_sprintf_e_zero_val);
  tcase_add_test(tc_sprintf, sprintf_specifier_Le_zero_precision);
  tcase_add_test(tc_sprintf, test_null_p);
  tcase_add_test(tc_sprintf, test_separator);
  tcase_add_test(tc_sprintf, test_precision_negative);
  tcase_add_test(tc_sprintf, test_f_zero);
  tcase_add_test(tc_sprintf, test_g_negative);

  tcase_add_test(tc_sprintf, sprintf_width_star_G);
  //////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////

  suite_add_tcase(s, tc_sprintf);
  return s;
}
int main() {
  int number_failed;
  Suite *s_memchr, *s_memcmp, *s_memcpy, *s_memset, *s_strchr, *s_strcspn,
      *s_strerror, *s_strlen, *s_strncat, *s_strncmp, *s_strncpy, *s_strpbrk,
      *s_strrchr, *s_strstr, *s_strtok, *s_sprintf;
  SRunner *sr;

  s_memchr = memchr_suite();
  s_memcmp = memcmp_suite();
  s_memcpy = memcpy_suite();
  s_memset = memset_suite();
  s_strchr = strchr_suite();
  s_strcspn = strcspn_suite();
  s_strerror = strerror_suite();
  s_strlen = strlen_suite();
  s_strncat = strncat_suite();
  s_strncmp = strncmp_suite();
  s_strncpy = strncpy_suite();
  s_strpbrk = strpbrk_suite();
  s_strrchr = strrchr_suite();
  s_strstr = strstr_suite();
  s_strtok = strtok_suite();
  s_sprintf = sprintf_suite();

  sr = srunner_create(s_memchr);
  srunner_add_suite(sr, s_memcmp);
  srunner_add_suite(sr, s_memcpy);
  srunner_add_suite(sr, s_memset);
  srunner_add_suite(sr, s_strchr);
  srunner_add_suite(sr, s_strcspn);
  srunner_add_suite(sr, s_strerror);
  srunner_add_suite(sr, s_strlen);
  srunner_add_suite(sr, s_strncat);
  srunner_add_suite(sr, s_strncmp);
  srunner_add_suite(sr, s_strncpy);
  srunner_add_suite(sr, s_strpbrk);
  srunner_add_suite(sr, s_strrchr);
  srunner_add_suite(sr, s_strstr);
  srunner_add_suite(sr, s_strtok);
  srunner_add_suite(sr, s_sprintf);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? 0 : 1;
}