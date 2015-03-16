all: router pktgen

pktgen: pktgen.c
	gcc -o pktgen pktgen.c

router: router.c
	gcc -o router router.c

clean:
	rm -rf *.o *~ *.gch pktgen router *.txt
