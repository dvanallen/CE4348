#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char** argv)
{
	/* port number from argv */
	int port;
	/* length of internet socket struct, for allocation on accept() */
	int addrlen;
	/* socket file descriptors for server and eventual connecting client */
	int server_sock, client_sock;
	/* describes the socket used by the server */
	struct sockaddr_in sin;
	/* describes the socket used to communicate with the client */
	struct sockaddr_in pin;
	/* Used to retrieve the hostname of this server */
	char host[80];
	/* Flag to continue reading from the client until it chooses to quit */
	bool isDone = false;
	
	/* Check if any other parameters were given, and exit if so */
	if (argc != 2)
	{
		std::cout << "Usage: ./server <port number>" << std::endl;
		return 1;
	}
	/* take port number from argv */
	port = atoi(argv[1]);
	
	/* Attempt to create a stream socket */
	if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		std::cout << "Error: Cannot create socket.\n";
		return 1;
	}
   
	/* Fill out the server's socket structure */
	sin.sin_family = AF_INET;
	/* any address on this host */
	sin.sin_addr.s_addr = INADDR_ANY;
	/* convert to network byte order */
	sin.sin_port = htons(port);        

	/* Attemp to bind the socket using the socket info in sin */
	if (bind(server_sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		std::cout << "Error: Cannot bind to port.\n";
		return 1;
	}

	/* set queue size of pending connections */ 
	listen(server_sock, 5);

	/* Get hostname and announce the server's startup */
	gethostname(host, 80); 
	std::cout << "Server is running on " << host << ":" << port << std::endl;

	/* store length for use in accept() */
	addrlen = sizeof(pin);
	/* blocks until a client attempts to connect.
	   If successful, client_sock describes the server-client connection */
	if ((client_sock = accept(server_sock, (struct sockaddr *)  &pin, &addrlen) == -1) 
	{
		std::cout << "Error: Cannot accept incoming connection.";
		return 1;
	}
	
	std::cout << "Got client, exiting.\n";
	
	close(client_sock);
	close(server_sock);
	
	return 0;
}