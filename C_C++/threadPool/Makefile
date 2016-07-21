BIN= threadpool
all: $(BIN)

threadpool: threadpool.c
	gcc $< -g -o $@ -lpthread

.PHONY: clean
clean:
	-rm -f $(BIN)
