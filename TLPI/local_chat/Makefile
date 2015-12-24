all: a.out b.out

a.out:
	gcc -g -I /home/qiu/.lib server.c
b.out:
	gcc -g -I /home/qiu/.lib client.c -o b.out

clean:
	-rm *.out *.o
	-rm *fifo*
