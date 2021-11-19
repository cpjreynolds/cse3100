CC=cc
CFLAGS=-g
LFLAGS=-pthread

all: dltest

dltest: dllist.o dltest.o
	$(CC) $(CFLAGS) -o $@  $^ $(LFLAGS)

%.o : %.c dllist.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o dltest
