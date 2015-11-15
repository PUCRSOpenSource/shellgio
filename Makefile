IDIR = ./include
SDIR = ./src
ODIR = ./obj

CC = gcc
CFLAGS = -Wall -g -I$(IDIR)

_DEPS = fat.h shell.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = fat.o shell.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: main

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) $^ -o $@

.PHONY: clean

clean:
	rm -f fat.part
	rm -f $(ODIR)/*.o
	rm -rf main

ifeq ($(shell uname), Darwin)
run: main
	./main
	hexdump -x fat.part
else
run: main
	./main
	hexdump fat.part
endif