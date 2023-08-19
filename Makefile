#! /bin/bash
CC=gcc

client:client.o
	${CC} -o client client.o

server:server.o
	${CC} -o server server.o

client.o:client.c declarations.h headers.h 
	${CC} -c client.c

server.o:server.c declarations.h headers.h 
	${CC} -c server.c

clean:
	rm *.o
	rm server
	rm client