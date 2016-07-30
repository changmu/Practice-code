CFLAGS = -O2 -fPIC
LDLIBS = -lpthread

ifdef DEBUG
CFLAGS += -O2
LDFLAGS += -O2
endif

TARGETS: libthreadpool.a

all: $(TARGETS)

threadpool.o: threadpool.c threadpool.h

# Short-hand aliases
static: libthreadpool.a

libthreadpool.o: threadpool.c threadpool.h
	g++ -c ${CFLAGS} -o $@ $<

libthreadpool.a: libthreadpool.o
	ar rcs $@ $^

clean:
	-rm -f *.a *.o
