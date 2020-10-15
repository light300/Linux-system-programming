.PHONY: all clean

CC = gcc

all: a.out
a.out: main.c ctx.c ctx.h rw.c rw.h
	$(CC) -o $@ $^
clean:
	rm -rf a.out
