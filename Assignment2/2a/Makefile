pktgen: pktgen.c pktgen.o
	gcc -m32 -o pktgen pktgen.c

router: router.c router.o
	gcc -m32 -o router router.c

pktgen.o: pktgen.c pktgen.h
	gcc -m32 -c pktgen.c
	gcc -m32 -c pktgen.h

router.o: router.c router.h pktgen.h
	gcc -m32 -c router.c
	gcc -m32 -c router.h
	gcc -m32 -c pktgen.h

clean:
	rm -rf *.o *~ *.gch