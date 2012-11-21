############################# Makefile ##########################
all: compila
compila:
	gcc -c -std=gnu99 encode.c -I.
	gcc -lm -std=gnu99 -o encode cliente.c encode.o -I.
	gcc -o cliente cliente.c encode.o -lm
	gcc -o servidor servidor.c



