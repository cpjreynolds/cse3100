CFLAGS = -g
CC = cc

all: prog1 prog2

prog1: prog1.o buffer.o 
	$(CC) $(CFLAGS) -o $@ $^ -lrt -lpthread

prog2: prog2.o buffer.o 
	$(CC) $(CFLAGS) -o $@ $^ -lrt -lpthread

%.o : %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *~ *.o prog1 prog2
