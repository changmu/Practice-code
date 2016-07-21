.PHONY: clean all

BIN	= makeThreadPool main
OBJS= threadpool.o condition.o
CC	= gcc
CXX	= g++
LIB_DIR	= .
LINKLIB	= -lpthread -lrt
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
main:main.o threadpool.o condition.o
	$(CC) $(CFLAGS) $^ -o $@ $(LINKLIB)

makeThreadPool:$(OBJS)
	ar rcs libthreadpool.a $^

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN) libthreadpool.a 
