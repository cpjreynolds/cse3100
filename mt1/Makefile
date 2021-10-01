CFLAGS=-g -std=c99
CC=cc

all: subsetsum hdist xlist

hdist: hamming.o hammingCode.o
	$(CC) $(CFLAGS) -o $@ $?

subsetsum: subsetsum.o subsetsumCode.o
	$(CC) $(CFLAGS) -o $@ $?

xlist: xlist.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf *.o subsetsum hdist xlist *.dSYM *~ _minted-mt1
