.PHONY: clean all makeJsonLib

BIN	= checkHardware
OBJS= cpu_stat.o mem_stat.o ip_stat.o \
	  net_stat.o main.o partition_stat.o

CC	= gcc
CXX	= g++
LIBDIR	= ./lib
LINKLIB	= -ljson -lthreadpool -lpthread
CFLAGS	+= -Wall -fPIC -g -static -I $(LIBDIR) -L $(LIBDIR)
CXXFLAGS+= -Wall -Wno-unused-result -fPIC -g -static -I $(LIBDIR) -L $(LIBDIR)

all:makeJsonLib makeThreadPollLib $(BIN)
# g++ -static -I. -Llib curl_test.cc  /usr/local/lib/libcurl.a -lz -lrt  -lpthread -ljson
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
