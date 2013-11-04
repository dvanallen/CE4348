/* Daniel Van Allen
 * CE4348.001: OS Concepts
 * Project 2: DMV Threads
 * Due: 2013-11-09
 */

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

#define MAX_CUSTOMERS 100

int ticket[MAX_CUSTOMERS];
int cust_count, ticket_count;

sem_t desk_line, desk, desk_cust_ready, agent_line;
sem_t mutex1, mutex2, mutex3, mutex4;
sem_t agent[2], agent_cust_ready[2], announcer[MAX_CUSTOMERS], exam[MAX_CUSTOMERS], finished[MAX_CUSTOMERS];

/* Define actions taken by each thread actor */
void *customerThread(void *arg) {
	int id = (int)arg;
	printf("Customer %d Created\n", id);
}

void *iDeskThread() {
	printf("Information desk created\n");
}

void *announcerThread() {
	printf("Announcer created\n");
}

void *agentThread(void *arg) {
	int id = (int)arg;
	printf("Agent %d created\n", id);
}

/* Initialize semaphores and threads */
int main() {
	sem_init(&desk_line, 0, 10);
	sem_init(&desk, 0, 1);
	sem_init(&desk_cust_ready, 0, 0);
	sem_init(&agent_line, 0, 10);
	sem_init(&agent[0], 0, 1);
	sem_init(&agent[1], 0, 1);
	sem_init(&agent_cust_ready[0], 0, 0);
	sem_init(&agent_cust_ready[1], 0, 0);
	sem_init(&mutex1, 0, 1);
	sem_init(&mutex2, 0, 1);
	sem_init(&mutex3, 0, 1);
	sem_init(&mutex4, 0, 1);
	int y;
	for(y = 0; y < MAX_CUSTOMERS; y++) {
        	sem_init(&announcer[y], 0, 1);
	        sem_init(&exam[y], 0, 1);
	        sem_init(&finished[y], 0, 0);
	}

	pthread_t agents[2];
	pthread_t desk;
	pthread_t announcer;
	pthread_t customers[MAX_CUSTOMERS];
	int x;	
	for(x = 0; x < 2; x++) {
        	int agentErr = pthread_create(&agents[x], NULL, &agentThread, (void*)x);
                if(agentErr != 0) {
                	printf("Error creating agent thread #%d\n", x);
                        pthread_exit(NULL);
                }
        }
        int deskErr = pthread_create(&desk, NULL, &iDeskThread, NULL);
        if(deskErr != 0) {
	        printf("Error creating desk thread");
        	pthread_exit(NULL);
	}
	int anncErr = pthread_create(&announcer, NULL, &announcerThread, NULL);
	if(anncErr != 0) {
		printf("Error creating announcer thread");
		pthread_exit(NULL);
	}
	for(x = 0; x < MAX_CUSTOMERS; x++) {
                int custErr = pthread_create(&customers[x], NULL, &customerThread, (void*)x);
                if(custErr != 0) {
                        printf("Error creating customer thread #%d\n", x);
                        pthread_exit(NULL);
                }
        }
        
        pthread_exit(NULL);
}

