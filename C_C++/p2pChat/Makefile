.PHONY: clean all

BIN=server client
CC=gcc
LIB_DIR=$(HOME)/github/.lib
LINK= -la
CFLAGS=-Wall -g -I $(LIB_DIR) -L $(LIB_DIR) -la

all:$(BIN)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LINK)

clean:
	-rm -f *.o $(BIN)
