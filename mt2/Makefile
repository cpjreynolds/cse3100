all: Q1 Q2server Q2client Q2service

Q1 : Q1.c
	$(CC) $(CFLAGS) -o $@ $?

Q2server : Q2server.c
	$(CC) $(CFLAGS) -o $@ $?

Q2client : Q2client.c
	$(CC) $(CFLAGS) -o $@ $?

Q2service : Q2service.c
	$(CC) $(CFLAGS) -o $@ $?

clean:
	rm -rf *.o Q1 Q2client Q2server Q2service foobar.db *~
