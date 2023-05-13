//SALVAJES
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NUMITER 3

int *caldero;
sem_t *vacio, *lleno, *sem_mutex;

int getServingsFromPot()
{
	sem_wait(sem_mutex);
	if ((*caldero) == 0) {
		printf("Caldero vacio, salvaje esperando ...\n");
		sem_post(vacio);
		sem_wait(lleno);
	}
	printf("Salvaje %d tiene una porcion, el caldero tiene ahora %d\n", getpid(), (*caldero));
	(*caldero) = (*caldero) - 1;
	sem_post(sem_mutex);
}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages()
{
	for (int i = 0; i < NUMITER; i++){
		getServingsFromPot();
		eat();
	}

}

int main(int argc, char *argv[])
{
	int fd;
	
	// Abrimos los semáforos ya creado
	vacio = sem_open("/vacio", 0);
	lleno = sem_open("/lleno", 0);
	sem_mutex = sem_open("/mutex", 0);
	
	// Mapemaos la memoria comp.
	fd = shm_open("/caldero", O_RDWR, 0);
	caldero = (int *) mmap(NULL, sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED,
	                    fd, 0); 
	close(fd);

	savages();

	// Cerramos semáforos.
	sem_close(vacio);
	sem_close(lleno);
	sem_close(sem_mutex);

	return 0;
}