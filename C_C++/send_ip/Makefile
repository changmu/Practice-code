.PHONY: clean all

BIN = server client
ALL: $(BIN)

CXX = g++
CC = gcc

server: server.o
	$(CXX) $^ -o $@
client: client.o
	$(CXX) $^ -o $@

.cpp.o:
	$(CXX) -c $^ -o $@

clean:
	-rm -f *.o $(BIN) *.out

