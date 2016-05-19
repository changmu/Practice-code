BIN= threadpool
all: $(BIN)

threadpool: threadpool.c
	gcc $< -o $@ -lpthread

.PHONY: clean
clean:
	-rm -f $(BIN)
