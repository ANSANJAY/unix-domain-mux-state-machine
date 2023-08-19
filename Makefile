#! /bin/bash
CC=gcc

server:server.o
	${CC} -o server server.o

server.o:server.c declarations.h headers.h 
	${CC} -c server.c

clean:
	rm *.o
	rm server