.PHONY: clean all

BIN=server client
CC=gcc
LIB_DIR=$(HOME)/github/.lib
CFLAGS=-Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
