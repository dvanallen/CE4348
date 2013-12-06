/*Ben Privat and Daniel Van Allen
 * Project 3: Mars Rover Simulation
 * CE4348: OS Concepts
 * Due 2013-12-07
*/

#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUFF_SIZE 100
#define REPLY_SIZE 1000

class Node
{
	public:
		Node(char* pName, Node* pLeft = NULL, Node* pRight = NULL)
		{
			left = pLeft;
			right = pRight;
			name = pName;
		}
		~Node() {}
		
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
int replyFunct(int sd, char id, char* reply)
{
	int count = 0;
	char msg[BUFF_SIZE];
	sprintf(msg, "%c%u!%s", id, (unsigned) strlen(reply), reply);
	
	if ((count = write(sd, msg, strlen(msg)+1)) == -1)
	{
		std::cout << "Error: Can't write to client socket.\n";
		return -1;
	}
	return count;
}

int main(int argc, char** argv)
{
	/** Server Setup **/
	/* port number from argv */
	int port;
	/* length of internet socket struct, for allocation on accept() */
	int addrlen;
	/* socket file descriptors for server and eventual connecting client */
	int sd_server, sd_client;
	/* describes the socket used by the server */
	struct sockaddr_in sin;
	/* describes the socket used to communicate with the client */
	struct sockaddr_in pin;
	/* Used to retrieve the hostname of this server */
	char host[80];

	/** Client Setup and Communication **/
	/* Stores number of characters read from client */
	int read_count = 0;
	/* stores the data sent by the client */
	char buffer[BUFF_SIZE];
	/* stores the data to be sent to the client */
	char reply[REPLY_SIZE];
	/* seed RNG for temperature */
	srand(time(NULL));
	/* Some messages from client do not require a reply
	   This flag is set to false when this is the case */
	bool doReply = false;
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
	
	
	/* Check if any other parameters were given, and exit if so */
	if (argc != 2)
	{
		std::cout << "Usage: ./server <port number>" << std::endl;
		return 1;
	}
	/* take port number from argv */
	port = atoi(argv[1]);
	
	/* Attempt to create a stream socket */
	if ((sd_server = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
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
	if (bind(sd_server, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		std::cout << "Error: Cannot bind to port.\n";
		return 1;
	}

	/* set queue size of pending connections */ 
	listen(sd_server, 5);

	/* Get hostname and announce the server's startup */
	gethostname(host, 80); 
	std::cout << "Server is running on " << host << ":" << port << std::endl;

	/* store length for use in accept() */
	addrlen = sizeof(pin);
	/* blocks until a client attempts to connect.
	   If successful, sd_client describes the server-client connection */
	if ((sd_client = accept(sd_server, (struct sockaddr *)  &pin, (socklen_t*)&addrlen)) == -1) 
	{
		std::cout << "Error: Cannot accept incoming connection.";
		return 1;
	}

	while(!isDone)
	{
		doReply = true;
		if ((read_count = read(sd_client, buffer, sizeof(buffer))) == -1)
		{
			std::cout << "Error: Could not read data from client socket.\n";
			return 1;
		}
		
		if (read_count == 0)
		{
			std::cout << "Error: No message from client.\n";
			isDone = true;
		}
		else
		{
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
					sprintf(reply, "Rover is facing %s", curDirection->getName());
					break;
				
				/* Temperature */
				case 'T': 
					sprintf(reply, "Air temperature at Mars Rover is %d C", rand()%100 - 50);
					break;
				
				/* Quit */
				case 'Q': 
					isDone = true;
					doReply = false;
					break;
				
				default: 
					std::cout << "Error: Unknown message ID.\n";
					doReply = false;
					break;
			}
		
			if (doReply)
			{
				if (replyFunct(sd_client, buffer[0], reply) == -1)
				{
					std::cout << "Error: Cannot send reply.\n"; 
					return 1;
				}
			}
		}
	}
	
	close(sd_client);
	close(sd_server);
	std::cout << "DONE\n";
	return 0;
}
