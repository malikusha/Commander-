all: mc2

mc2: mc2.o
	gcc mc2.o -o mc2

mc2.o: mc2.c mc2.h
	gcc -g -c mc2.c

clean:
	rm -f *.o mc2
