all: file_server file_client

file_client: file_client.c
	gcc -Wall -m32 -o file_client file_client.c

file_server: file_server.c
	gcc -Wall -m32 -o file_server file_server.c

ps:
	ps -u lingbo | grep file_server

clean:
	-rm -f *.o file_server file_client *~
