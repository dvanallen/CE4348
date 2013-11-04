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
	printf("Created thread #%d\n", id);
}

void *informationDesk(void *arg) {

}

void *announcer(void *arg) {

}

void *agent(void *arg) {

}

int main() {
        pthread_t customerThreads[MAX_CUSTOMERS];
        int x;
	for(x = 0; x < MAX_CUSTOMERS; x++) {
                int err = pthread_create(&customerThreads[x], NULL, &customer, (void*)x);
                if(err != 0) {
                        printf("Error creating thread #%d\n", x);
                        pthread_exit(NULL);
                }
        }
        
        pthread_exit(NULL);
}

