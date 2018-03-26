all:
	gcc -c pthreads.c -pthread
	gcc pthreads.o -o 1 -pthread

run:
	@./script.sh
clean:
	rm 1 *.o

