server client: server.c server.o client.c client.o
	gcc -std=c99 -o server client server.c client.c

client.o: client.c
	gcc -std=c99 -c memchunk.c

server.o: server.c
  gcc -std=c99 -c server.c

clean:
	rm -rf *.o testmem memchunk.h.gch *.c~ *.h~ Makefile~
