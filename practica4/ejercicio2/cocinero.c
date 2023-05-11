#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>

#define M 10

int finish = 0;
int* caldero;
sem_t* caldero_acceso;
sem_t* caldero_porciones;

void putServingsInPot(int servings)
{
	sem_wait(caldero_acceso);
	servings = M;
	printf("Cocinero %d putting servings in pot. \n", getpid());
	sem_post(caldero_porciones);
	sem_post(caldero_acceso);
}

void cook(void)
{
	while(!finish) {
		sem_wait(caldero_porciones);
		putServingsInPot(M);
	}
}

void handler(int signo)
{

	finish = 1;
}

int main(int argc, char *argv[])
{
	sem_close(caldero_porciones);
	sem_close(caldero_acceso);
	sem_unlink("/caldero_porciones");
	sem_unlink("/caldero_acceso");
	munmap(caldero, sizeof(int));
	shm_unlink("/caldero");

	caldero_acceso = sem_open("/caldero_acceso", O_CREAT | O_EXCL, 0644, 1);
	printf("creado 1\n");
	caldero_porciones = sem_open("/caldero_porciones", O_CREAT | O_EXCL, 0644, 0);
	printf("creado 2\n");
	if ( caldero_porciones == SEM_FAILED || caldero_acceso == SEM_FAILED ) {
		perror("Error creating semaphores");
		printf("hola\n");
		exit(EXIT_FAILURE);
	}

	int fd = shm_open("/caldero", O_CREAT | O_RDWR, 0660);
	printf("creado pot\n");
	if(fd == -1){
		perror("Error creating shared memory");
		exit(EXIT_FAILURE);
	}

	ftruncate(fd, sizeof(int));
	caldero = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	printf("creado pot map\n");
	if (caldero == MAP_FAILED) {
		perror("Error mapping shared memory");
		exit(EXIT_FAILURE);
	}

	signal(SIGTERM, handler);
	signal(SIGINT, handler);

	cook();

	if (munmap(caldero, sizeof(int)) == -1) {
        perror("Error unmapping shared memory");
    }
    if (shm_unlink("/caldero") == -1) {
        perror("Error unlinking shared memory");
    }
    if (sem_close(caldero_acceso) == -1) {
        perror("Error closing semaphore pot_access");
    }
    if (sem_close(caldero_porciones) == -1) {
        perror("Error closing semaphore pot_servings");
    }
    if (sem_unlink("/caldero_acceso") == -1) {
        perror("Error unlinking semaphore pot_access");
    }
    if (sem_unlink("/caldero_porciones") == -1) {
        perror("Error unlinking semaphore pot_servings");
    }

    return 0;
}
