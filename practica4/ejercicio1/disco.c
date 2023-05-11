#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 5
#define M
#define VIPSTR(vip) ((vip) ? "  vip  " : "not vip")

pthread_cond_t vacio;
pthread_mutex_t mutex;

int colaVip = 0;
int aforoActual = 0;
int turnoN = 0;
int turnoVip = 0;
int ticketN = 0;
int ticketVip = 0;

pthread_t clientes[M];
typedef struct
{
	int id;
	int isvip;
} param;

void enter_normal_client(int id)
{
	pthread_mutex_lock(&mutex);
	int Nticket = ticketN;
	ticketN++;
	while ((Nticket != turnoN) && (colaVip < 0) && (aforoActual > CAPACITY))
	{
		pthread_cond_wait(&vacio, &mutex);
	}
	aforoActual++;
	turnoN++;
	printf("Entra normal con id %d \n", id);

	pthread_mutex_unlock(&mutex);
}

void enter_vip_client(int id)
{
	pthread_mutex_lock(&mutex);
	int VipTicket = ticketVip;
	ticketVip++;
	colaVip++;
	while (VipTicket != turnoVip && aforoActual > CAPACITY)
	{
		pthread_cond_wait(&vacio, &mutex);
	}
	aforoActual++;
	colaVip--;
	turnoVip++;
	printf("Entra vip con id %d \n", id);
	pthread_mutex_unlock(&mutex);
}

void dance(int id, int isvip)
{
	printf("Client %2d (%s) dancing in disco\n", id, VIPSTR(isvip));
	sleep((rand() % 3) + 1);
}

void disco_exit(int id, int isvip)
{
	pthread_mutex_lock(&mutex);
	if (aforoActual == CAPACITY) pthread_cond_broadcast(&vacio);

	aforoActual--;
	pthread_mutex_unlock(&mutex);
}

void *client(void *arg)
{

	if (((param *)arg)->isvip) enter_vip_client(((param *)arg)->id);
	else enter_normal_client(((param *)arg)->id);

	dance(((param *)arg)->id, ((param *)arg)->isvip);
	disco_exit(((param *)arg)->id, ((param *)arg)->isvip);
}

int main(int argc, char *argv[])
{
	param *parametros;
	FILE *fichero;
	int size, isvip;
	int arr[100];
	int thread_mutex_inti(mutex, null);
	pthread_cond_init(&vacio, NULL);

	fichero = fopen("ejemplo.txt", "r");
	if (fichero == NULL)
	{
		printf("Error abrir fichero");
		exit(1);
	}

	fscanf(fichero, "%d", &size);
	for (int i = 0; i < size; i++)
	{
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
