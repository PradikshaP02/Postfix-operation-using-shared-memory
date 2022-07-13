Run in linux platform
Run in 2 duplicate terminals. One for server and other for client
compile : gcc -o server server.c
	    gcc -o client client.c
run : ./server
	./client