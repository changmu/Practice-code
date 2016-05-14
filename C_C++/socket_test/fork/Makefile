all: Server Client

CFLAGS = -I/home/qiu/.lib

Server: server.c
	gcc $(CFLAGS) $< -o $@

Client: client.c
	gcc $(CFLAGS) $< -o $@

.PHONY:clean
clean:
	-rm -f Server Client *.out
