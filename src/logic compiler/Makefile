CC      = gcc
CFLAGS  = -Wall -g -O -D_DEFAULT_SOURCE -std=c99 -pedantic -Werror
BUILD   = evaluate
LDFLAGS = -lm

.SUFFIXES: .c .o

.PHONY: all clean

all: $(BUILD)

evaluate: evaluate.o parse_tree.o assembly_writer.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(wildcard *.o)
	rm -f evaluate
