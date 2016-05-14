.PHONY: clean all

BIN	= server client
CC	= gcc
LIB_DIR	= $(HOME)/.lib
LINKLIB	= -la
CFLAGS	= -Wall -g -I $(LIB_DIR) -L $(LIB_DIR)

all:$(BIN)
	$(CC) -o ${BIN} $(CFLAGS) ${LINKLIB} $^
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o $(BIN)
