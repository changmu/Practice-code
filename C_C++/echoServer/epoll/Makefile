.PHONY: clean all

BIN	= server client cclient
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
server:server.o
	$(CXX) $(CFLAGS) $^ -o $@ $(LINKLIB)
client:client.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)
cclient:cclient.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
