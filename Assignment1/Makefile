testprogram: testmem.c memchunk.o
	gcc -m32 -o testmem testmem.c memchunk.c

memchunk.o: memchunk.c memchunk.h
	gcc -m32 -c memchunk.c
	gcc -m32 -c memchunk.h

clean:
	rm -rf *.o testmem memchunk.h.gch *.c~ *.h~ Makefile~
