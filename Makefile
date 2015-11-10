SDIR = ./src
CC = gcc

all: tp2

run: tp2
	./tp2
	hexdump fat.part

tp2: $(SDIR)/tp2.c
	$(CC) $^ -o $@

.PHONY: clean

clean:
	rm -f fat.part
	rm -f tp2
