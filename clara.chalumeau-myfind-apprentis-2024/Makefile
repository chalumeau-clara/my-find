CC ?= gcc
CPPFLAGS = -D_GNU_SOURCE
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -fsanitize=address -g

MYFINDSRC = src/myfind.c src/main.c src/ast_evaluation.c src/expression.c src/memory.c src/parser.c
MYFINDOBJ = src/myfind.o src/main.o src/ast_evaluation.o src/expression.o src/memory.o src/parser.o
MYFINDBIN = myfind

all: $(MYFINDBIN)

$(MYFINDBIN): $(MYFINDOBJ)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $(MYFINDBIN) $^

check:
	./tests/test.sh

clean:
	$(RM) $(MYFINDOBJ) $(MYFINDBIN) $(TESTBIN)

