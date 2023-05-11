#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/mman.h>

#define NUMITER 3

// int *pot;
// sem_t* pot_access;
// sem_t* pot_servings;

int getServingsFromPot(sem_t *pot_access, sem_t *pot_servings, int *pot)
{
	sem_wait(pot_access);
	if (*pot == 0) {
		printf("Caldero vacio, salvaje esperando ...\n");
		sem_post(pot_access);
		sem_wait(pot_servings);
	}
	// } else {
		int servings = *pot;
		printf("Salvaje %d tiene una porcion, el caldero tiene ahora %d\n", getpid(), *pot);
		*pot = *pot - 1;
		sem_post(pot_access);
		return servings;
	// }
	

}

void eat(void)
{
	unsigned long id = (unsigned long) getpid();
	printf("Savage %lu eating\n", id);
	sleep(rand() % 5);
}

void savages(sem_t *pot_access, sem_t *pot_servings, int *pot)
{
	for (int i = 0; i < NUMITER; i++){
		
		getServingsFromPot(pot_access, pot_servings, pot);
		eat();
	}

}

int main(int argc, char *argv[])
{

	
	sem_t *caldero_acceso = sem_open("/caldero_acceso", 0);
	sem_t *caldero_porciones = sem_open("/caldero_porciones", 0);
	int shmid = shmget("/caldero", sizeof(int), 0666);
	int *caldero = (int*) shmat(shmid, NULL, 0);

	if (caldero_acceso == SEM_FAILED || caldero_porciones == SEM_FAILED || caldero == (int*) -1) {
		printf("Error opening shared resources. Did you run the cook first?\n");
		return -1;
	}

	savages(caldero_acceso, caldero_porciones, caldero);

	// Desvinculamos la memoria compartida
	shmdt(caldero);

	return 0;
}


