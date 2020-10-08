.PHONY: all clean
SRCS = $(wildcard *.c)
BIN	 = myshell

all:$(BIN)
$(BIN):$(SRCS)
	gcc -Wall -o $@ $^

clean:
	rm -f $(BIN)
