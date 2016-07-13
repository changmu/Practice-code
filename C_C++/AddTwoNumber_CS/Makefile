.PHONY: clean all

BIN	= server client
CC	= gcc
CXX	= g++
LIB_DIR	= 
LINKLIB	= 
CFLAGS	+= -Wall -Wextra -Werror -Wno-unused-parameter -g -I $(LIB_DIR) -L $(LIB_DIR) -std=c11
CXXFLAGS+= -Wall -Wextra -Werror -Wno-unused-parameter -g -I $(LIB_DIR) -L $(LIB_DIR) -std=c++11

all:$(BIN)
server:server.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LINKLIB)
client:client.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
