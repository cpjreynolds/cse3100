CC=cc
CFLAGS=-O3

all: matrix

matrix: matrix.o main.o
	$(CC) $(CFLAGS) -o $@ $^

%.o : %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.o matrix *~
