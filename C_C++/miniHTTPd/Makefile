.PHONY: clean all

BIN	= MiniHTTPd
CC	= gcc
CXX	= g++
LINKLIB	= -lpthread
CFLAGS	+= -Wall -Wextra  -g
CXXFLAGS+= -Wall -Wextra -g -std=c++11

all:$(BIN)
MiniHTTPd:main.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
