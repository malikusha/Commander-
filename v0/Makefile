all: mc0

mc0: mc0.o
	gcc mc0.o -o mc0

mc0.o: mc0.c mc0.h
	gcc -c mc0.c

clean:
	rm -f *.o mc0
