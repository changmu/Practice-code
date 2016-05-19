.PHONY: clean all

BIN	= mqueue
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la -lrt
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
mqueue:mqueue.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
