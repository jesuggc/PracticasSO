#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define CAPACITY 2
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")


pthread_cond_t vacio;
pthread_mutex_t mutex;

int colaVip = 0;
int aforoActual = 0;
int turnoN = 0;
int turnoVip = 0;
int contadorTicketN = 0;
int contadorTicketV = 0;


typedef struct {
	int id;
	int isvip;
} param;

void enter_normal_client(int id) {

	pthread_mutex_lock(&mutex);
	int miTurno = contadorTicketN++;

	while ((miTurno != turnoN) || (colaVip > 0) || (aforoActual >= CAPACITY)) {
		pthread_cond_wait(&vacio, &mutex);
	}

	aforoActual++;
	turnoN++;
	printf("Entra normal con id %d \n", id);
	pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id) {
	pthread_mutex_lock(&mutex);
	int miTurno = contadorTicketV++;
	colaVip++;
	while (miTurno != turnoVip || aforoActual >= CAPACITY) {
		pthread_cond_wait(&vacio, &mutex);
	}
	aforoActual++;
	colaVip--;
	turnoVip++;
	printf("Entra vip con id %d \n", id);
	pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip) {
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip) {
	pthread_mutex_lock(&mutex);
	if (aforoActual == CAPACITY) pthread_cond_broadcast(&vacio);
	aforoActual--;
	pthread_mutex_unlock(&mutex);
}

void *client(void *arg) {
	int id = ((param *)arg)->id;
	int vip = ((param *)arg)->isvip;
	
	if (vip) enter_vip_client(id);
	else enter_normal_client(id);

	dance(id, vip);
	disco_exit(id,vip);
}

int main(int argc, char *argv[])
{
	param *parametros;
	FILE *fichero;
	int size, isvip;
	int arr[100];
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&vacio, NULL);
	pthread_t *clientes;
	int prueba = 0;
	fichero = fopen("ejemplo.txt", "r");
	if (fichero == NULL) {
		printf("Error abrir fichero");
		exit(1);
	}
	
	fscanf(fichero, "%d", &size);
	clientes = malloc(size * sizeof(pthread_t));
	for (int i = 0; i < size; i++) {
		parametros = malloc(sizeof(param));
		parametros->id = i;

		fscanf(fichero, "%d", &parametros->isvip);

		pthread_create(&clientes[i], NULL, client, parametros);
	}

	for (int i = 0; i < size; i++)
	{
		pthread_join(clientes[i], NULL);
	}
	pthread_mutex_destroy(&mutex);

	return 0;
}
