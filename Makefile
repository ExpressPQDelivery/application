client_epqd: echo_client.c
	gcc -o client_epqd echo_client.c -lssl -lcrypto -lresolv -pthread -DMODE=1

client_tls: echo_client.c 
	gcc -o client_tls echo_client.c -lssl -lcrypto -lresolv -pthread -DMODE=0

server: echo_mpserv.c
	gcc -o server echo_mpserv.c -lssl -lcrypto

all: server client_tls client_epqd

clean:
	rm server client_tls client_epqd