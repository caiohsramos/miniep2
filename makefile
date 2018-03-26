all:
	gcc -c pthreads.c -pthread
	gcc pthreads.o -o frog -pthread

clean:
	rm frog *.o

