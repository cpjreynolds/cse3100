CC=gcc
CFLAGS=-O2

OS:=$(shell uname)
ifneq ($(OS),Darwin)
	LDFLAGS=-pthread -lrt
endif

all: matrix

matrix: matrix.o main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< $(LDFLAGS)

clean:
	rm -rf *.o matrix *~
