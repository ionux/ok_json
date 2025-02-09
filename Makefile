CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -Iinclude

SRC = src/ok_json.c
TEST_SRC = test/ok_json_tests.c
OBJ = $(SRC:.c=.o)
TEST_OBJ = $(TEST_SRC:.c=.o)

all: ok_json.a test

ok_json.a: $(OBJ)
	ar rcs $@ $^

test: ok_json.a $(TEST_OBJ)
	$(CC) $(CFLAGS) -o test/ok_json_test_runner $(TEST_OBJ) ok_json.a
	./test/ok_json_test_runner

clean:
	rm -f $(OBJ) $(TEST_OBJ) ok_json.a test/ok_json_test_runner
