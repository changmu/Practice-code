all: Server Client

FLAGS = -I/home/qiu/.lib -L/home/qiu/.lib
LINK = -la

Server: server.c
	gcc $(FLAGS) $< -o $@ $(LINK)

Client: client.c
	gcc $(FLAGS) $< -o $@ $(LINK)

.PHONY: clean
clean:
	-rm -f Client Server
