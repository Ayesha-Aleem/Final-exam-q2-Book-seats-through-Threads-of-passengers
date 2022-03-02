#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<semaphore.h>
#include<pthread.h>

#define MAX_SEATS 10
#define PASSENGER_NAME_LEN 20

sem_t mutex;

struct passenger {
	int seats;
	char name[PASSENGER_NAME_LEN];
	int bought;
};

struct passenger* passengers;

int boughtSeats = 0;

void* book(void* u) {
	int* index = (int*) u;

	sem_wait(&mutex);

	printf("\nEnter your name: ");
	scanf("%s", &passengers[*index].name);

	printf("Enter number of seats: ");
	scanf("%d", &passengers[*index].seats);

	if(passengers[*index].seats + boughtSeats > MAX_SEATS) {
		passengers[*index].bought = 0;
	} else {
		passengers[*index].bought = 1;
		boughtSeats += passengers[*index].seats;
	}

	sem_post(&mutex);

}

int main() {
	pthread_t* tids;
	int num_of_passengers;
	int i;
	struct passenger p;

	sem_init(&mutex, 0, 1);

	printf("Enter number of passengers: ");
	scanf("%d", &num_of_passengers);

	tids =  malloc(num_of_passengers * sizeof(pthread_t));
	passengers =  malloc(num_of_passengers * sizeof(struct passenger));

	printf("\nTotal available seats: %d\n", MAX_SEATS);

	for(i = 0; i < num_of_passengers; i++) {
		int* index = (int*) malloc(sizeof(int));
		*index = i;

		pthread_create(&tids[i], NULL, &book, index);
	}

	for(i = 0; i < num_of_passengers; i++) {
		pthread_join(tids[i], NULL);
	}

	// Output
	printf("\n\nTotal Seats Available: %d\n", MAX_SEATS);
	for(i = 0; i < num_of_passengers; i++) {
		printf("%s: ", passengers[i].name);
		if(passengers[i].bought) {
			printf("%d seats booked\n", passengers[i].seats);
		} else {
			printf("%d seets are not available\n", passengers[i].seats);
		}
	}

	return 0;
}
