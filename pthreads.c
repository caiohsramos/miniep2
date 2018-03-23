#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define N 3
#define M 3
#define P (M + N)

enum { LIVRE, OCUPADA };

typedef struct thread_data {
	int thread_id;
	int pos_inicial;
} THREAD_DATA;

//variaveis globais
int cnt = 0;
int *lagoa = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void inicia_lagoa() {
	lagoa = (int*)malloc(sizeof(int)*(P+1));
	if(lagoa == NULL) printf("\nmalloc falhou\n");
	int i;
	for(i = 0; i <= P; i++) {
		lagoa[i] = OCUPADA;
	}
	lagoa[P/2] = LIVRE;
}

void *sapo(void* arg) {
	THREAD_DATA *data = (THREAD_DATA*)arg;
	pthread_mutex_lock(&lock);
	sleep(1);
	printf("Sou sapo na posição %d\n", data->pos_inicial);
	if(lagoa[data->pos_inicial] == LIVRE) 
		printf("Estou numa posicao livre\n");
	else 
		printf("Estou numa posicao ocupada\n");
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);	
}

void *ra(void *arg) {
	THREAD_DATA *data = (THREAD_DATA*)arg;
	pthread_mutex_lock(&lock);
	sleep(1);
	printf("Sou ra na posição %d\n", data->pos_inicial);
	if(lagoa[data->pos_inicial] == LIVRE) 
		printf("Estou numa posicao livre\n");
	else 
		printf("Estou numa posicao ocupada\n");
	pthread_mutex_unlock(&lock);
	pthread_exit(NULL);	
}

int main(int argc, char *argv[]) {
    pthread_t thread[P+1];
	int t;

	THREAD_DATA *data_array = (THREAD_DATA*)malloc(sizeof(THREAD_DATA)*(P+1));
	if(data_array == NULL) 
		printf("\nmalloc falhou\n");

	lagoa = (int*)malloc(sizeof(int)*(P+1));
	if(lagoa == NULL) 
		printf("\nmalloc falhou\n");


	inicia_lagoa();

   	pthread_mutex_lock(&lock);
	// thread[0] vai ser a gerenciadora
	for(t = 1; t <= N; t++) {
		//inicializa as ras
		data_array[t].thread_id = t;
		data_array[t].pos_inicial = t-1;
		pthread_create(&thread[t], NULL, ra, (void*)&data_array[t]);	
	}
	for(t = N+1; t <= P; t++) {
		//inicializa os sapos
		data_array[t].thread_id = t;
		data_array[t].pos_inicial = t;
		pthread_create(&thread[t], NULL, sapo, (void*)&data_array[t]);	
	}
	pthread_mutex_unlock(&lock);
	
	for(t = 1; t <= P; t++)
		    pthread_join(thread[t],NULL);
	//pthread_join(threadGerencia,NULL);
	
	free(data_array);
	free(lagoa);

	pthread_exit(0);
	return 0;
}
