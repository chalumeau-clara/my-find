CC ?= gcc
CPPFLAGS = -D_GNU_SOURCE
CFLAGS = -Wall -Werror -Wextra -std=c99 -pedantic -g

BIN = ast_evaluation
OBJS = main.o $(BIN).o memory.o parser.o

all: $(BIN)
$(BIN): $(OBJS)
clean:
	$(RM) $(BIN) $(OBJS)
