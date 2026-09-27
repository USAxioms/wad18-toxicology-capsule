CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -O2 -pedantic
LDFLAGS = -lm

OBJECTS = toxicology_core_wad18.o

all: test_toxicology_wad18

test_toxicology_wad18: $(OBJECTS) test_toxicology_wad18.c
	$(CC) $(CFLAGS) -c test_toxicology_wad18.c -o test_toxicology_wad18.o
	$(CC) $(CFLAGS) -o test_toxicology_wad18 $(OBJECTS) test_toxicology_wad18.o $(LDFLAGS)

toxicology_core_wad18.o: toxicology_core_wad18.c toxicology_core_wad18.h wad18_common.h
	$(CC) $(CFLAGS) -c toxicology_core_wad18.c -o toxicology_core_wad18.o

test: all
	./test_toxicology_wad18

clean:
	rm -f *.o test_toxicology_wad18

.PHONY: all test clean
