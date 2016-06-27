CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lm

cliserv: client.o server.o
	${CC} ${CFLAGS} client.o -o client; 
	${CC} ${CFLAGS} server.o -o server;
	rm *.o

client.o: client.cpp CS450Header6.h 450UtilsUDP.h
	${CC} ${CFLAGS} -c client.cpp

server.o: server.cpp CS450Header6.h 450UtilsUDP.h
	${CC} ${CFLAGS} -c server.cpp
