LIBS = -lpthread -lncurses

all : client server

client : client.o game.o
	gcc -o client client.o game.o $(LIBS)

server : server.o game.o
	gcc -o server server.o game.o $(LIBS)


server.o : game.h server.c
	gcc -Wall -c -o server.o server.c

client.o : game.h client.c
	gcc -c -o client.o client.c $(LIBS)

game.o : game.h game.c
	gcc -c -o game.o game.c

clean :
	rm *.o server client
