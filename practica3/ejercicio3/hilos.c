#include <stdlib.h>
#include <stdio.h>

//nueva anadir
#include <pthread.h>

 struct estructura
{

	int thread_num;
	char priority;

};

void* thread_usuario(void *arg)
{
	struct estructura *ptrestructura = arg;
	int thread_num = ptrestructura->thread_num;
	char priority = ptrestructura->priority;

	free(arg); 
	
	pthread_t thread_id = pthread_self();
	printf("Thread %lu: number %d, priority %c\n", thread_id, thread_num, priority);
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t arrayhilos[10];

	int i = 0;

	for (i = 0; i < 10; i++)
	{
		/*
		struct estructura auxestructura;
		struct estructura *ptrestructura = &auxestructura;
		*/
		struct estructura *ptrestructura =malloc(sizeof(struct estructura));

		ptrestructura->thread_num = i;

		if (i % 2 == 0) ptrestructura->priority = 'P';
		else ptrestructura->priority = 'N';
		pthread_create(&arrayhilos[i], NULL, thread_usuario, (void *)ptrestructura);
	}

	/*

	La función pthread_join espera a que un hilo se termine de ejecutar y
	sincroniza el hilo llamador con el hilo que está esperando a que finalice.
	En otras palabras, cuando se llama a pthread_join con un identificador de hilo,
	la función se bloquea hasta que el hilo con ese identificador termine su ejecución y
	luego retorna. Además, pthread_join también libera los recursos asociados con el hilo
	que ha finalizado su ejecución, por lo que es importante llamar a esta función después
	de que el hilo haya terminado para evitar fugas de memoria.

	*/

	for (i = 0; i < 10; i++)
	{

		pthread_join(arrayhilos[i], NULL);
	}
	return 0;
}
