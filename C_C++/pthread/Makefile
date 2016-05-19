.PHONY: clean all

BIN	= main
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la -lpthread
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
main:main.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
