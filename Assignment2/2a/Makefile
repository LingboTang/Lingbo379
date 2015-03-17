all: router pktgen

pktgen: pktgen.c
	gcc -Wall -m32 -o pktgen pktgen.c

router: router.c
	gcc -Wall -m32 -o router router.c

clean:
	rm -rf *.o *~ *.gch pktgen router
