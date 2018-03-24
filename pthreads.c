#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

#define N 3
#define M 3
#define P (M + N)
#define LIMITE 100

enum { LIVRE, OCUPADA, SAPO, RA };

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

int deadlock() {
	if(cnt > LIMITE || cnt < 0) return 1;
	else return 0;

}

int tenta_pular(int genero, int pos) {
	if(genero == SAPO) { //sapo p/ direita

	} else { //ra p/ esquerda

	}

	return 0; //nao pulou

	//return 1; //pulou
}

void *sapo(void* arg) {
	THREAD_DATA *data = (THREAD_DATA*)arg;
	while(!deadlock()) {
		pthread_mutex_lock(&lock);
		//sleep(1);
		printf("Sou sapo na posição %d\n", data->pos_inicial);
		if(tenta_pular(SAPO, data->pos_inicial)) cnt = 0;
		else cnt++;
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(NULL);	
}

void *ra(void *arg) {
	THREAD_DATA *data = (THREAD_DATA*)arg;
	while(!deadlock()) {
		pthread_mutex_lock(&lock);
		//sleep(1);
		printf("Sou ra na posição %d\n", data->pos_inicial);
		if(tenta_pular(RA, data->pos_inicial)) cnt = 0;
		else cnt ++;
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(NULL);	
}

void *gerenciador(void *arg) {
	
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
	data_array[0].thread_id = 0;
	data_array[0].pos_inicial = -1;
	pthread_create(&thread[0], NULL, gerenciador, (void*)&data_array[0]);	

	//inicializa as ras
	for(t = 1; t <= N; t++) {
		data_array[t].thread_id = t;
		data_array[t].pos_inicial = t-1;
		pthread_create(&thread[t], NULL, ra, (void*)&data_array[t]);	
	}

	//inicializa os sapos
	for(t = N+1; t <= P; t++) {
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
