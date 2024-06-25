# This is the default target, which will be built when
# you invoke make
# Opcion para compilar para arm, para uso del verificador 
.PHONY: all
	all: server

server: main.o protect.o
	arm-linux-gnueabihf-g++ -g -o server main.o protect.o
	cp -f server ../../eprom

main.o:
	arm-linux-gnueabihf-g++ -g -c main.c

protect.o:
	arm-linux-gnueabihf-g++ -g -c protect.c

.PHONY: clean
clean:
	rm server main.o protect.o
