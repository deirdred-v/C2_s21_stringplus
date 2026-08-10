CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11
TFLAGS = -lcheck -lsubunit -lm
GCOVFLAGS = -fprofile-arcs -ftest-coverage
TPATH = tests

STRING_FUNC = $(wildcard ./string_functions/*.c)
SPRINTF_FUNC = $(wildcard ./sprintf_functions/*.c) 
ALL_FUNC = $(STRING_FUNC) $(SPRINTF_FUNC)
LIBNAME = s21_string.a
S21_TEST = s21_test
GCOV = test_gcov

all: $(LIBNAME) gcov_report

s21_string.a: 
	$(CC) $(CFLAGS) $(ALL_FUNC) -c
	ar rc $(LIBNAME) *.o
	ranlib $(LIBNAME)

install_lib:
	sudo apt-get install check
	sudo apt install libsubunit-dev
	sudo apt-get install lcov

test: $(S21_TEST)
	./s21_test

$(S21_TEST): $(TPATH)/test.c $(LIBNAME)
	$(CC) $(CFLAGS) $< -o $@ -L. $(LIBNAME) $(TFLAGS) 

############################################################################################
gcov_report: clean
	$(CC) $(CFLAGS) $(GCOVFLAGS) $(ALL_FUNC) -c
	ar rc $(LIBNAME) *.o
	ranlib $(LIBNAME)
	$(CC) $(CFLAGS) $(TPATH)/test.c -o $(S21_TEST) -L. $(LIBNAME) $(TFLAGS) -lgcov
	./$(S21_TEST)
	lcov --capture --directory . --output-file coverage.info --rc lcov_branch_coverage=1
	genhtml coverage.info --output-directory coverage --rc lcov_branch_coverage=1
###############################################################################################

clean:
	rm -rf *.o *.a $(S21_TEST) *.gcno *.gcda coverage coverage.info
