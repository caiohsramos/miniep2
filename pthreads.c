//Caio Henrique Silva Ramos - NUSP 9292991

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>

#define LIMITE 50

enum { LIVRE, SAPO, RA };

typedef struct thread_data {
	int thread_id;
	int pos_inicial;
} THREAD_DATA;

//variaveis globais
int N; //numero de ras
int M; //numero de sapos
int P; //ras + sapos
int cnt = 0;
int *lagoa = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void inicia_lagoa() {
	lagoa = (int*)malloc(sizeof(int)*(P+1));
	if(lagoa == NULL) printf("\nmalloc falhou\n");
	int i;
	for(i = 0; i < N; i++) {
		lagoa[i] = RA;
	}
	lagoa[i++] = LIVRE;
	for(; i <= P; i++) {
		lagoa[i] = SAPO; 
	}
}

int deadlock() {
	if(cnt > LIMITE) return 1;
	else return 0;

}

void dummy() {
	usleep(30);
}

int tenta_pular(int genero, int *pos_end) {
	int pos = *pos_end;
	//ra p/ direita
	if(genero == RA) { 
		//pulo duplo
		if(((pos+2) < (P+1)) && (lagoa[pos+2] == LIVRE) && (lagoa[pos+1] != LIVRE)) {
			lagoa[pos] = LIVRE;
			lagoa[pos+2] = RA;
			*pos_end = pos+2;
			return 1;
		//pulo simples
		} else if(((pos+1) < (P+1)) && (lagoa[pos+1] == LIVRE)) {
			lagoa[pos] = LIVRE;
			lagoa[pos+1] = RA;
			*pos_end = pos+1;
			return 1;
		} else return 0;
	//sapo p/ esquerda
	} else { 
		//pulo simples
		if(((pos-1) >= 0) && (lagoa[pos-1] == LIVRE)) {
			lagoa[pos] = LIVRE;
			lagoa[pos-1] = SAPO;
			*pos_end = pos-1;
			return 1;
		//pulo duplo
		} else if(((pos-2) >= 0) && (lagoa[pos-2] == LIVRE) && (lagoa[pos-1] != LIVRE)) {
			//faz pulo duplo
			lagoa[pos] = LIVRE;
			lagoa[pos-2] = SAPO;
			*pos_end = pos-2;
			return 1;
		} else return 0;
	}

}

void *sapo(void* arg) {
	THREAD_DATA *data = (THREAD_DATA*)arg;
	int pos = data->pos_inicial;
	while(!deadlock()) {
		pthread_mutex_lock(&lock);
		if(tenta_pular(SAPO, &pos)) cnt = 0;
		else cnt++;
		pthread_mutex_unlock(&lock);
		dummy();
	}
	pthread_exit(NULL);	
}

void *ra(void *arg) {
	THREAD_DATA *data = (THREAD_DATA*)arg;
	int pos = data->pos_inicial;
	while(!deadlock()) {
		pthread_mutex_lock(&lock);
		if(tenta_pular(RA, &pos)) cnt = 0;
		else cnt ++;
		pthread_mutex_unlock(&lock);
		dummy();
	}
	pthread_exit(NULL);	
}


int main(int argc, char *argv[]) {
	if(argc < 3) {
		printf("Numero de argumentos insuficiente\n");
		return 1;
	}
	N = atoi(argv[1]);
	M = atoi(argv[2]);
	P = M + N;

    pthread_t thread[P];
	int t;
	srand(time(NULL));

	THREAD_DATA *data_array = (THREAD_DATA*)malloc(sizeof(THREAD_DATA)*P);
	if(data_array == NULL) 
		printf("\nmalloc falhou\n");

	lagoa = (int*)malloc(sizeof(int)*(P+1));
	if(lagoa == NULL) 
		printf("\nmalloc falhou\n");


	inicia_lagoa();

   	pthread_mutex_lock(&lock);
	//inicializa as ras
	for(t = 0; t < N; t++) {
		data_array[t].thread_id = t;
		data_array[t].pos_inicial = t;
		pthread_create(&thread[t], NULL, ra, (void*)&data_array[t]);	
	}

	//inicializa os sapos
	for(t = N; t < P; t++) {
		data_array[t].thread_id = t;
		data_array[t].pos_inicial = t+1;
		pthread_create(&thread[t], NULL, sapo, (void*)&data_array[t]);	
	}
	pthread_mutex_unlock(&lock);
	
	for(t = 0; t < P; t++)
		    pthread_join(thread[t],NULL);

	int i;	
	for(i = 0; i <= P; i++) {
		switch(lagoa[i]) {
			case SAPO:
				printf("S");
				break;
			case RA:
				printf("R");
				break;
			case LIVRE:
				printf("*");
				break;
			default:
				break;
		};
	}
	printf("\n");
	free(data_array);
	free(lagoa);

	pthread_exit(0);
	return 0;
}
