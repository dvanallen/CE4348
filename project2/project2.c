/* Daniel Van Allen
 * CE4348.001: OS Concepts
 * Project 2: DMV Threads
 * Due: 2013-11-09
 */

#include <stdio.h>
#include <pthread.h>

#define MAX_CUSTOMERS 100

/* Initialize Threads */
int main() {
	pthread_t customerThreads[MAX_CUSTOMERS];
	for(int x = 0; x < MAX_CUSTOMERS; x++) {
		int err = pthread_create(&customerThreads[x], NULL, &customer, (void*)id);
		if(err != 0) {
			printf("Error creating thread #%d\n", x);
			pthread_exit(NULL);
		}
	}
	
	pthread_exit(NULL);
}

void *customer(void *arg) {
	int id = arg;
	printf("Created thread #%\n", id);
}

void *informationDesk(void *arg) {

}

void *announcer(void *arg) {

}

void *agent(void *arg) {

}