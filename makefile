all:
	gcc -c pthreads.c -pthread
	gcc pthreads.o -o 1 -pthread

run:
	./1

clean:
	rm 1 *.o

