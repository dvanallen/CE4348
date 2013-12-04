#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdio.h>

#define BUFF_SIZE 100

class Node
{
	public:
		Node(char* pName, Node* pLeft = NULL, Node* pRight = NULL)
		{
			left = pLeft;
			right = pRight;
			name = pName;
		}
		~Node();
		
		void setNodes(Node* pLeft, Node* pRight)
		{
			left = pLeft;
			right = pRight;
		}
		
		Node* getLeft()
		{
			return left;
		}
		
		Node* getRight()
		{
			return right;
		}
		
		char* getName()
		{
			return name;
		}
		
	private:
		Node* left;
		Node* right;
		char* name;
};

/* Replies to the client at sck, tagging the reply payload with id */
int reply(sockaddr_in sck, char id, char* reply)
{
	char* msg;
	sprintf(msg, "%s%d!%s", id, strlen(reply), reply);
	
	if ((count = write(sck, msg, strlen(msg)+1)) == -1)
	{
		std::cout << "Error: Can't write to client socket.\n";
		return -1;
	}
	
	return count;
}

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
	
	/* Create a node for each direction
	   Create a circular linked list for easy traversal */
	Node north("North");
	Node south("South");
	Node east("East", &south, &north);
	Node west("West", &north, &south);
	north.setNodes(&west, &east);
	south.setNodes(&east, &west);
	Node* curDirection = &north;
	
	int read_count = 0;
	char buffer[BUFF_SIZE];
	char* reply;
	srand(time(NULL));
	
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
	
	while(!isDone)
	{
		if ((count = read(client_sock, buffer, sizeof(buffer))) == -1)
		{
			std::cout << "Error: Could not read data from client socket.\n";
			return 1;
		}
		
		switch(buffer[0])
		{
			/* Turn left */
			case 'L': 
				curDirection = curDirection->getLeft();
				sprintf(reply, "Rover has turned left 90 degrees.");
				break;
			
			/* Turn right */
			case 'R': 
				curDirection = curDirection->getRight();
				sprintf(reply, "Rover has turned right 90 degrees.");
				break;
				
			/* Picture */
			case 'P': 
				break;
				
			/* Direction */
			case 'D': 
				sprintf(reply, "Rover is facing %s", curDirection.getName());
				break;
				
			/* Temperature */
			case 'T': 
				sprintf(reply, "Air temperature at Mars Rover is %d C", rand()%100 - 50);
				break;
				
			/* Quit */
			case 'Q': 
				isDone = true;
				break;
				
			default: 
				std::cout << "Error: Unknown message ID.\n";
				return 1;
		}
		
		if (reply(client_sock, buffer[0], reply) == -1)
		{
			return 1;
		}
	}
	
	close(client_sock);
	close(server_sock);
	
	return 0;
}
