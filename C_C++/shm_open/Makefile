.PHONY: clean all

BIN	= shm_open
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la -lrt
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
shm_open:shm_open.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
