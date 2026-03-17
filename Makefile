.PHONY: all test coverage clean

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -Iinclude \
         -Wconversion -Wsign-conversion -Wfloat-equal -Wcast-qual \
         -Wcast-align -Wpointer-arith -Wshadow -Wlogical-op -Wundef \
         -Wswitch-default -Wswitch-enum -Wunreachable-code -O0 \
         -DOK_JSON_DEBUG

SRC = src/ok_json.c
TEST_SRC = test/ok_json_tests.c
OBJ = $(SRC:.c=.o)

all: ok_json.a test

ok_json.a: $(OBJ)
	ar rcs $@ $^

# The test file includes src/ok_json.c directly (Unity/Ceedling pattern), so
# the implementation and tests form a single translation unit.  Link only the
# test source — no separate object file, no ok_json.a dependency.
test: $(TEST_SRC) $(SRC)
	$(CC) $(CFLAGS) -o test/ok_json_test_runner $(TEST_SRC)
	./test/ok_json_test_runner

coverage: CFLAGS += --coverage
coverage: clean test
	gcovr -r . --xml-pretty -o coverage.xml
	gcovr -r .

clean:
	rm -f $(OBJ) ok_json.a test/ok_json_test_runner \
	      *.gcno *.gcda src/*.gcno src/*.gcda test/*.gcno test/*.gcda coverage.xml

fuzz: test/fuzz_target.c $(SRC)
	clang -g -O1 -fsanitize=fuzzer,address,undefined -Iinclude test/fuzz_target.c -o fuzzer
	./fuzzer -max_total_time=30 -max_len=4096
