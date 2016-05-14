.PHONY: clean all

BIN	= server client
CC	= gcc
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
server:server.o
	$(CC) $(CFLAGS) $(LINKLIB) -o $@
client:client.o
	$(CC) $(CFLAGS) $(LINKLIB) -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
