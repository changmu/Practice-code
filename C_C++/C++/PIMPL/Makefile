.PHONY: clean all

BIN	= main
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la
CFLAGS	+= -Wall -Wextra -Werror -Wno-unused-parameter \
	   -march=native -Wwrite-strings -Wshadow -g -I $(LIB_DIR) -L $(LIB_DIR)
CXXFLAGS+= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR) -std=c++14

all:$(BIN)
main:main.o widget.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
