CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3
#
# coco: word_count.o smap.o
# 	${CC} ${CFLAGS} -o $@ $^

coco: cooccur.o smap.o
	${CC} ${CFLAGS} -o $@ $^
