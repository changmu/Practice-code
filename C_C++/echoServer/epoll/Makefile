.PHONY: clean all

BIN	= server client
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)
CXXFLAGS= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR) -std=c++11

all:$(BIN)
server:server.o
#	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LINKLIB)
client:client.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c  $< -o $@

clean:
	-rm -f *.o $(BIN)
