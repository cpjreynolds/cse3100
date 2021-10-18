.PHONY: all
CFLAGS=-g -pedantic -Werror
CC=gcc

all: shell

shell: shellp.c shellp.h
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o shell *~
