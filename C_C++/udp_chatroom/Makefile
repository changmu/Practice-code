.PHONY: clean all

BIN	= client server
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR) -std=c++11

all:$(BIN)
server:server.o
	$(CXX) $(CFLAGS) $^ -o $@ $(LINKLIB)
client:client.o
	$(CXX) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
