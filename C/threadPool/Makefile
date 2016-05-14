all: ThreadPool

ThreadPool: threadpool.c
	gcc $< -o $@ -lpthread

.PHONY: clean
clean:
	-rm -f ThreadPool
