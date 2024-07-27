CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -g
GCOVFLAGS = -fprofile-arcs -ftest-coverage
TESTFLAGS = -lcheck -lm
INC = include
SRC = src
TEST = test
HEAD = $(INC)/club_manager.h
SRCS = $(SRC)/main.c \
       $(SRC)/events.c \
       $(SRC)/parser.c
OBJS = $(SRCS:.c=.o)
UNIT_TEST = $(TEST)/test.c
EXEC = club_manager

ifeq ($(shell uname -s), Linux)
    TESTFLAGS += -lpthread -lrt -lsubunit
endif

.PHONY: all clean test gcov_report

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(SRC)/%.o: $(SRC)/%.c $(HEAD)
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

test: clean $(SRC)/events.o $(SRC)/parser.o $(UNIT_TEST)
	$(CC) $(GCOVFLAGS) -I$(INC) $(UNIT_TEST) $(SRC)/events.o $(SRC)/parser.o -o test.out $(TESTFLAGS)
	./test.out

gcov_report: clean $(SRC)/events.o $(SRC)/parser.o $(UNIT_TEST)
	$(CC) $(GCOVFLAGS) $(UNIT_TEST) $(SRC)/events.c $(SRC)/parser.c -o report.out -ldl $(TESTFLAGS)
	./report.out
	lcov -t "gcov_report" -o coverage_table.info -c -d .
	genhtml -o ./report coverage_table.info

style:
	clang-format -style="{BasedOnStyle: Google, IndentWidth: 4}" -i $(INC)/*.h $(SRC)/*.c $(TEST)/*.c

clean:
	rm -rf $(SRC)/*.o $(EXEC) test.out report.out coverage_table.info report *.gcda *.gcno *.log
