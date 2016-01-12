all: Server Client

LibDir = /home/qiu/.lib
Flags = -I $(LibDir) -L $(LibDir)
Link = -la -lpthread

Server: server.c
	gcc $(Flags) $< -o $@ $(Link)

Client: client.c
	gcc $(Flags) $< -o $@ $(Link)

.PHONY: clean
clean:
	-rm -f *.out Client Server
