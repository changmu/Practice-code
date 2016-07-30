.PHONY: clean all makeJsonLib

BIN	= checkHardware
OBJS= cpu_stat.o mem_stat.o ip_stat.o disk_stat.o \
	  net_stat.o main.o partition_stat.o

CC	= gcc
CXX	= g++
LIBDIR	= ./lib
LINKLIB	= -ljson -lthreadpool -lpthread
CFLAGS	+= -Wall -fPIC -O2 -static -I $(LIBDIR) -L $(LIBDIR)
CXXFLAGS+= -Wall -Wno-unused-result -fPIC -O2 -static -I $(LIBDIR) -L $(LIBDIR)

all:makeJsonLib makeThreadPollLib $(BIN)

checkHardware:$(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LINKLIB) /usr/local/lib/libcurl.a -lz -lrt

makeJsonLib:
	make -C jsoncpp
	cp jsoncpp/*.a lib/
makeThreadPollLib:
	make -C threadpool
	cp threadpool/*.a lib/


.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LINKLIB)

clean:
	-rm -f *.o $(BIN) *.out *.a
	make -C jsoncpp clean
	make -C threadpool clean
