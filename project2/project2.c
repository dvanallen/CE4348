/* Daniel Van Allen
 * CE4348.001: OS Concepts
 * Project 2: DMV Threads
 * Due: 2013-11-09
 */

#include <stdio.h>
#include <pthread.h>

#define MAX_CUSTOMERS 100

void *customer(void *arg) {
	int id = (int)arg;
	printf("Customer %d Created\n", id);
}

void *iDesk() {
	printf("Information desk created\n");
}

void *announcer() {
	printf("Announcer created\n");
}

void *agent(void *arg) {
	int id = (int)arg;
	printf("Agent %d created\n", id);
}

int main() {
	pthread_t agentThreads[2];
	pthread_t iDeskThread;
	pthread_t announcerThread;
	pthread_t customerThreads[MAX_CUSTOMERS];
	int x;	
	
	for(x = 0; x < 2; x++) {
                int agentErr = pthread_create(&agentThreads[x], NULL, &agent, (void*)x);
                        if(agentErr != 0) {
                                printf("Error creating agent thread #%d\n", x);
                                pthread_exit(NULL);
                        }
        }

        int deskErr = pthread_create(&iDeskThread, NULL, &iDesk, NULL);
        if(deskErr != 0) {
	        printf("Error creating desk thread");
        	pthread_exit(NULL);
	}

	int anncErr = pthread_create(&announcerThread, NULL, &announcer, NULL);
	if(anncErr != 0) {
		printf("Error creating announcer thread");
		pthread_exit(NULL);
	}

	for(x = 0; x < MAX_CUSTOMERS; x++) {
                int custErr = pthread_create(&customerThreads[x], NULL, &customer, (void*)x);
                if(custErr != 0) {
                        printf("Error creating customer thread #%d\n", x);
                        pthread_exit(NULL);
                }
        }
        
        pthread_exit(NULL);
}

