CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3 -lm

ClosestPair: point.o plist.o closest_pair.o # plist_unit.o
	${CC} ${CFLAGS} -o $@ $^
