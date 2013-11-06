#include <stdio.h>
#include <pthread.h>
#include <windows.h>

pthread_mutex_t read, write;
const int N = 10;
const int writters = 2;

int numOfReads = 0;



void *pisatelj(void* n)
{
	pthread_mutex_lock(&write);
	printf("P%d:Z \n", (int)n);
	Sleep(2 * (int)n);
	printf("P%d:K \n", (int)n);
	pthread_mutex_unlock(&write);
	return NULL;
}


void*bralec(void *n)
{
	pthread_mutex_lock(&read);
	if (numOfReads == 0)
		pthread_mutex_lock(&write);
	numOfReads++;
	pthread_mutex_unlock(&read);

	printf("B%d:Z \n", (int)n);
	Sleep((int)n);
	printf("B%d:K \n", (int)n);
	/*if (numOfReads < N){
		numOfReads++;
		pthread_mutex_unlock(&read);
	}else*/
	pthread_mutex_lock(&read);
	numOfReads--;
	pthread_mutex_unlock(&read);

	if (numOfReads == 0){
		pthread_mutex_unlock(&write);
	}
	return NULL;
}

int main() {

	pthread_t threads[N];
	pthread_mutex_init(&read, NULL);
	pthread_mutex_init(&write, NULL);
	//pthread_mutex_lock(&read); // Nothing to read

	for (int i = 0; i < N; i++)
		pthread_create(&threads[i],NULL, i>(N-3)?pisatelj:bralec,(void *)i);

	for (int i = 0; i < N; i++)
		pthread_join(threads[i],NULL);
	


	pthread_mutex_destroy(&read);
	pthread_mutex_destroy(&write);
	getchar();
	return EXIT_SUCCESS;

}

