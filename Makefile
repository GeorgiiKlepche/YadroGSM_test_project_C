CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -g
INC = include
SRC = src
TEST = test
HEAD = $(INC)/club_manager.h
SRCS = $(SRC)/main.c \
       $(SRC)/events.c
OBJS = $(SRCS:.c=.o)
EXEC = club_manager

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(SRC)/%.o: $(SRC)/%.c $(HEAD)
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

style:
	clang-format -style="{BasedOnStyle: Google, IndentWidth: 4}" -i $(INC)/*.h $(SRC)/*.c $(TEST)/*.c

clean:
	rm -rf $(SRC)/*.o $(EXEC)