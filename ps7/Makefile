CC=gcc
CFLAGS=-O3

OS:=$(shell uname)
ifneq ($(OS),Darwin)
	LDFLAGS=-pthread -lrt
endif

all: matrix

matrix: matrix.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.o matrix *~
