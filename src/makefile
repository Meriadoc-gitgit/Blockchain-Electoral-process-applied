CFLAGS = -Wall -g -Wno-unused-parameter
CC = gcc

PROGRAMS = test temp

.PHONY: all clean 

all: $(PROGRAMS)

test: src.o main.o
	$(CC) -o $@ $(CFLAGS) $^

main.o: main.c 
	gcc $(CFLAGS) -c main.c

src.o: src.c 
	gcc $(CFLAGS) -c src.c

clean:
	rm -f *.o *~ $(PROGRAMS)