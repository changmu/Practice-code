.PHONY: clean all

BIN	= miniftpd
CC	= gcc
CXX	= g++
LIB_DIR	= /home/qiu/.lib
LINKLIB	= -la -lcrypt
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
miniftpd: main.o session.o ftpproto.o privaparent.o str.o \
	tunable.o parseconfig.o privsock.o hash.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
