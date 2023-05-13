//COCINERO
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


#define M 10

int finish = 0;
int *caldero;
sem_t *vacio, *lleno, *sem_mutex;

void putServingsInPot(int servings)
{
	sem_wait(vacio);
	(*caldero) = servings;
	printf("Cocinero %d putting servings in pot. \n", getpid());
	sem_post(lleno);
}

void cook(void)
{
	while(!finish) {
		putServingsInPot(M);
	}
}

void handler(int signo)
{
	finish = 1;
}

int main(int argc, char *argv[]) {
	// Por queeeee?????
	// sem_close(caldero_porciones);
	// sem_close(caldero_acceso);
	// sem_unlink("/caldero_porciones");
	// sem_unlink("/caldero_acceso");
	// munmap(caldero, sizeof(int));
	// shm_unlink("/caldero");
	
	// Creamos estructura para eliminar recursos
	struct sigaction sa;
	sigset_t set;
	sigfillset(&set);
	sa.sa_mask = set;
	sa.sa_flags = 0;
	sa.sa_handler = handler;
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGINT, &sa, NULL);

	// Creamos e inicializamos semÃ¡foros a compartir 
	sem_mutex = sem_open("/mutex", O_CREAT, 0700, 1);
	vacio = sem_open("/vacio", O_CREAT, 0700, 1);
	lleno = sem_open("/lleno", O_CREAT, 0700, 0);

	if ( sem_mutex == SEM_FAILED || vacio == SEM_FAILED || lleno == SEM_FAILED) {
		perror("Error creating semaphores");
		exit(EXIT_FAILURE);
	}

	// Inicializar shared memory
	int fd = shm_open("/caldero", O_CREAT|O_EXCL|O_RDWR, S_IRUSR | S_IWUSR);
	if(fd == -1){
		perror("Error creating shared memory");
		exit(EXIT_FAILURE);
	}
	// Truncamos a cero
	ftruncate(fd, sizeof(int));
	// Creamos buffer en shared memory
	caldero = (int *) mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (caldero == MAP_FAILED) {
		perror("Error mapping shared memory");
		exit(EXIT_FAILURE);
	}
	// Inicializamos pot a 0
	(*caldero) = 0;
	close(fd);

	cook();

	sem_close(vacio);
	sem_close(lleno);
	sem_close(sem_mutex);

	sem_unlink("/vacio");
	sem_unlink("/lleno");
	sem_unlink("/mutex");
	munmap(caldero, sizeof(int));
	shm_unlink("/caldero");

    return 0;
}