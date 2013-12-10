/*Ben Privat and Daniel Van Allen
 * Project 3: Mars Rover Simulation
 * CE4348: OS Concepts
 * Due 2013-12-10
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <fstream>

/* Run as:  client host port
 *  *
 *   * where host is the machine to connect to and port is its port number */
#define BUFSIZE     100       /* anything large enough for your messages */
#define MSG_SIZE 10000

/* Parse the file length string from the message and return it as an int */
int getFileLength(char msg[])
{
	std::string length = "";
	/* Starting at 1 (after the message ID) read off the digits of
	   the length string.  '!' delimits the end of the length, so quit
	   once we find it. */
	for (int i = 1; i < BUFSIZE; i++)
	{
		if (msg[i] == '!')
			break;
		length += msg[i];
	}
	
	return atoi(length.c_str());
}

/* Parse the payload from the message received from the server and return it */
const char* extractMsg(std::string str)
{
	/* find the '!', which marks the beginning of the payload.
	   Take the substring from after the '!' to the end of the string
	   and convert to a c string */
	return str.substr(str.find('!')+1, std::string::npos).c_str();
}

int main(int argc, char *argv[])
{
   char hostname[100];
   char buf[BUFSIZE];
   char query[BUFSIZE];
   char msg_id;
   //char msg[MSG_SIZE];
   std::string msg = "";
   std::string extractedMsg = "";
   int sd;
   int port;
   int count, cur_count;
   int length;
   bool isDone = false;
   struct sockaddr_in pin;
   struct hostent *hp;

   /* check for command line arguments */
   if (argc != 3)
   {
      printf("Usage: client host port\n");
      exit(1);
   }

   /* get hostname and port from argv */
   strcpy(hostname, argv[1]);
   port = atoi(argv[2]);

   /* create anhi broke mine a bit
    Internet domain stream socket */
   if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
      perror("Error creating socket");
      exit(1);
   }

   /* lookup host machine information */
   if ((hp = gethostbyname(hostname)) == 0) {
      perror("Error on gethostbyname call");
      exit(1);
   }

   /* fill in the socket address structure with host information */
   memset(&pin, 0, sizeof(pin));
   pin.sin_family = AF_INET;
   pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
   pin.sin_port = htons(port); /* convert to network byte order */


   printf("Connecting to %s:%d\n\n", hostname, port);

   /* connect to port on host */
   if (connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1) {
      perror("Error on connect call");
      exit(1);
   }
   /* ask user for a selection */
   printf("Input your selection using one letter in {L,R,P,D,T,Q}.\n");
   printf("L: Turn Rover Left 90 degrees.\n");
   printf("R: Turn Rover Right 90 degrees.\n");
   printf("P: Take a Picture of the martian ASCII landscape.\n");
   printf("D: Return the direction the rover is facing.\n");
   printf("T: Return the air temperature at the rover's location.\n");
   printf("Q: Quit.\n");

   while(!isDone)
   {
		msg = "";
		extractedMsg = "";
	   gets(query);
	   if(query[0] == 'Q' || query[0] == 'q') {
	      isDone = true;
	   }
	   
	   /* send the query to the server */
	   if ( (count = write(sd, query, strlen(query)+1)) == -1) {
	      perror("Error on write call");
	      exit(1);
	   }
	   printf("Client sent %d bytes\n", count);
	   
		/* wait for a message to come back from the server */
		if ( (count = read(sd, buf, BUFSIZE)) == -1) {
			perror("Error on read call");
			exit(1);
		}
		printf("Client read %d bytes\n", count);

		length = getFileLength(buf);
		//printf("Length %d.\n", length);
		//printf("buf is: %s\n", buf);
		msg = extractMsg(buf);
		//printf("Msg is: %s\n", msg.c_str());
		cur_count = count;
		msg_id = buf[0];
		while (cur_count < length)
		{
			if ( (count = read(sd, buf, BUFSIZE)) == -1) {
				perror("Error on read call");
				exit(1);
			}
			printf("Client read %d bytes\n", count);
			if (cur_count < (length -1))
			{
			 	std::string buffer(buf);
				std::string buffer2 = buffer.substr(0, (std::string::npos - 1));
				memcpy(buf,buffer2.c_str(),buffer2.size());
			}
			msg += buf;
	                //printf("buf is: %s\n", buf);
			//printf("Msg is: %s\n", msg.c_str());
			cur_count += count;
		}
		if (msg_id == 'P' || msg_id == 'p')
		{
			std::ofstream output("MARS_ROVER_IMAGE.jpg", std::fstream::binary);
			output.write(msg, length);
			output.close();
			printf("Client read image to MARS_ROVER_IMAGE.jpg in the current directory.\n");
		}
		else
		{
			printf("\n\n%s\n\n", msg);
		}
   }

   /* close the socket */
   close(sd);
   exit(0);
}

