all:server_p server_s server_f

server_p: server_p.c
	gcc -o server_p server_p.c -lpthread
server_s: server_s.c
	gcc -o server_s server_s.c
server_f: server_f.c
	gcc -o server_f server_f.c

clean:
	-rm -f *.o server_p server_f server_s
