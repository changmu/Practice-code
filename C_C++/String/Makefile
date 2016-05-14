objo =  main.o String.o
objc = $(objo:.o=.c)

a.out: $(objo)
	cc -g -o a.out $(objo)

$(objo): $(objc)
	cc -g -c $(objc)

clean:
	-rm *.o *.out
