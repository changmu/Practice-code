.PHONY: clean all

BIN	= libjson.a
OBJS= json_reader.o json_value.o json_writer.o
CC	= gcc
CXX	= g++
LIB_DIR	= .
LINKLIB	= .
CFLAGS	+= -Wall -fPIC -static -O2 -I $(LIB_DIR)
CXXFLAGS+= -Wall -fPIC -static -O2 -I $(LIB_DIR)

all:$(BIN)

libjson.a:$(OBJS)
	ar rcs $@ $^

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN) *.out
