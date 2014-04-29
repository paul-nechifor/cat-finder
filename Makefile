CC=gcc
CFLAGS=-std=c99 -Wall -Wextra

cat-finder:
	$(CC) -o cat-finder cat-finder.c $(CFLAGS)

.PHONY: clean

clean:
	rm -f cat-finder
