.PHONY:clean all
BIN=a b
CC=gcc
CFLAGS=-Wall -g
all: $(BIN)
.c.o:
	$(CC) $(CFLAGS) $^ -o $@
clean:
	@-rm -f $(BIN) *.o
