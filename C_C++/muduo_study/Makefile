.PHONY: clean all

BIN	= Thread_test boost_bind
CC	= gcc
CXX	= g++
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la -lpthread
CFLAGS	+= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)
CXXFLAGS+= -std=c++11 -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)

boost_bind: boost_bind.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LINKLIB)
Thread_test: Thread.o Thread_test.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
