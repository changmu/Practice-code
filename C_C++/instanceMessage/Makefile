all: Server Client

Flags = -std=c++11

Server: server.cpp
	g++ $(Flags) $< -o $@

Client: client.cpp
	g++ $(Flags) $< -o $@

.PHONY: clean
clean:
	-rm -rf *.out Client Server
