CC=cc
CFLAGS=-O3

OS:=$(shell uname)
ifneq ($(OS),Darwin)
	LDFLAGS=-pthread -lrt
endif

all: rpair detSeq detMT philo

rpair: rpair.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

detSeq: matrix.o detSeq.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

detMT: matrix.o detMT.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

philo : philo.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

%.o : %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -rf *.o rpair detSeq detMT philo *~
