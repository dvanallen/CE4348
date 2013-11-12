/* Daniel Van Allen
 * CE4348.001: OS Concepts
 * Project 2: DMV Threads
 * Due: 2013-11-09
 */

#include <stdio.h>
#include <queue>
#include <semaphore.h>
#include <pthread.h>

#define MAX_CUSTOMERS 100

int custDone = 0, agentDone = 0;
long ticket_count = 0;
long ticketnr[MAX_CUSTOMERS];

std::queue<long> queue1, queue2, queue3, queue4;

sem_t desk_line, desk, desk_ready, agent_line, agent, waiting_room;
sem_t mutex1, mutex2, mutex3, mutex4, mutex5, mutex6;
sem_t agent_cust_ready[2], announcer[MAX_CUSTOMERS], exam[MAX_CUSTOMERS], finished[MAX_CUSTOMERS], desk_finished[MAX_CUSTOMERS], agent_prompt[MAX_CUSTOMERS];

/* Define actions taken by each thread actor */
void* iDeskThread(void*) {
	long custnr = 100;
	int count;

	printf("Information desk created\n");
	for(count = 0; count < (MAX_CUSTOMERS); count++) {
		sem_wait(&desk_ready);
		sem_wait(&mutex1);
			custnr = queue1.back();
			queue1.pop();
		sem_post(&mutex1);
		sem_wait(&mutex2);
			ticketnr[custnr] = ticket_count;
			ticket_count++;
		sem_post(&mutex2);
		sem_post(&desk_finished[custnr]);
		sem_post(&desk);
	}
	pthread_exit(NULL);
}

void* announcerThread(void*) {
	long ticketnr = 100;
	int count;

	printf("Announcer created\n");
	for(count = 0; count < (MAX_CUSTOMERS); count++) {
		sem_wait(&agent_line);
		sem_wait(&waiting_room);
		sem_wait(&mutex5);
			ticketnr = queue4.front();
			queue4.pop();
		sem_post(&mutex5);
		printf("Announcer calls number %ld\n", ticketnr);
		sem_post(&announcer[ticketnr]);	
	}
	pthread_exit(NULL);
	
}

void* agentThread(void *arg) {
	long id = (long)arg;
	long custnr = 100;
	printf("Agent %ld created\n", id);
	while(true) {
                sem_wait(&mutex6);
                if(custDone == (MAX_CUSTOMERS)) {
                	sem_post(&mutex6);
                	pthread_exit(NULL);
                }
		sem_wait(&mutex4);
			queue3.push(id);
			sem_post(&agent);
		sem_post(&mutex4);
		sem_wait(&agent_cust_ready[id]);
		sem_wait(&mutex3);
			custnr = queue2.front();
			queue2.pop();
			custDone++;
			if(custDone == (MAX_CUSTOMERS)) {
				agentDone = 1;
			}
		sem_post(&mutex3);
		sem_post(&mutex6);
		printf("Agent %ld serving customer %ld\n", id, custnr);
		printf("Agent %ld asks customer %ld to take photo and eye exam\n", id, custnr);
		sem_post(&agent_prompt[custnr]);
		sem_wait(&exam[custnr]);
		printf("Agent %ld gives license to customer %ld\n", id, custnr);
		sem_post(&finished[custnr]);
		
	}
	
	pthread_exit(NULL);

}
void* customerThread(void *arg) {
        long custnr = (long)arg;

        printf("Customer %ld Created\n", custnr);
        sem_wait(&desk_line);
        printf("Customer %ld enters DMV\n", custnr);
        sem_wait(&desk);
        sem_post(&desk_line);
        sem_wait(&mutex1);
                queue1.push(custnr);
                sem_post(&desk_ready);
        sem_post(&mutex1);
        sem_wait(&desk_finished[custnr]);
        
	printf("Customer %ld gets number %ld, waits to be called\n", custnr, ticketnr[custnr]);
	
	sem_wait(&mutex5);
		queue4.push(ticketnr[custnr]);
		sem_post(&waiting_room);
	sem_post(&mutex5);
	sem_wait(&announcer[ticketnr[custnr]]);
	sem_wait(&agent);
	sem_post(&agent_line);
	sem_wait(&mutex4);
		long agent_id = queue3.back();
		queue3.pop();
	sem_post(&mutex4);
	printf("Customer %ld being served by agent %ld\n", custnr, agent_id);
	sem_wait(&mutex3);
		queue2.push(custnr);
		sem_post(&agent_cust_ready[agent_id]);
	sem_post(&mutex3);
	sem_wait(&agent_prompt[custnr]);
	printf("Customer %ld completes photo and eye exam for agent %ld\n", custnr, agent_id);
	sem_post(&exam[custnr]);
	sem_wait(&finished[custnr]);
	printf("Customer %ld gets license and departs\n", custnr);
	pthread_exit((void*) custnr);
}

int main() {
	/* Initialize semaphores */
	sem_init(&desk_line, 0, 10);
	sem_init(&desk, 0, 1);
	sem_init(&desk_ready, 0, 0);
	sem_init(&agent_line, 0, 10);
	sem_init(&mutex1, 0, 1);
	sem_init(&mutex2, 0, 1);
	sem_init(&mutex3, 0, 1);
	sem_init(&mutex4, 0, 1);
	sem_init(&mutex5, 0, 1);
	sem_init(&mutex6, 0, 1);
	
	int y;

	/*Initialize Threads */
	pthread_t agents[2];
	pthread_t desk;
	pthread_t announcer;
	pthread_t customers[MAX_CUSTOMERS];
	pthread_attr_t attr;
	void* threadStatus;
	int threadErr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	long x;	
	for(x = 0; x < 2; x++) {
        	int agentErr = pthread_create(&agents[x], &attr, &agentThread, (void*)x);
                if(agentErr != 0) {
                	printf("Error creating agent thread #%ld\n", x);
                        pthread_exit(NULL);
                }
        }
        int deskErr = pthread_create(&desk, &attr, &iDeskThread, NULL);
        if(deskErr != 0) {
	        printf("Error creating desk thread");
        	pthread_exit(NULL);
	}
	int anncErr = pthread_create(&announcer, &attr, &announcerThread, NULL);
	if(anncErr != 0) {
		printf("Error creating announcer thread");
		pthread_exit(NULL);
	}
	long z;
	for(z = 0; z < MAX_CUSTOMERS; z++) {
                int custErr = pthread_create(&customers[z], &attr, &customerThread, (void*)z);
                if(custErr != 0) {
                        printf("Error creating customer thread #%ld\n", z);
                        pthread_exit(NULL);
                }
        }

	/* Join threads and Exit. */
	int w;
	pthread_attr_destroy(&attr);
	for(w = 0; w < MAX_CUSTOMERS; w++) {
		threadErr = pthread_join(customers[w], &threadStatus);
		printf("Customer %ld was joined\n", (long)threadStatus);
	}
	
	if((long)threadStatus == (MAX_CUSTOMERS - 1)){
		threadErr = pthread_join(agents[0], &threadStatus);
		threadErr = pthread_join(agents[1], &threadStatus);
		threadErr = pthread_join(desk, &threadStatus);	
		threadErr = pthread_join(announcer, &threadStatus);
	}

	pthread_exit(NULL);
}

