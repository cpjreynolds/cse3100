CLI = client.o
SRV = server.o
OBJS = 
CFLAGS = -g 

all: ircd irc

irc: $(CLI) $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

ircd: $(SRV) $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^

clean:
	rm -rf *.o ircd irc *~
