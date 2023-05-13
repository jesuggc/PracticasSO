#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */

int main(int argc, char **argv)
{
	char *args[] = {"ls", NULL}; //definimos argumentos aqui que se pasaria al ejer
	int pid1, pid2;
	pid1 = fork();

	if (pid1 == 0) printf("Soy el hijo 1 con pid %d y con padre %d \n", pid1, getppid());
	else {
		wait(NULL);
		printf("Soy el padre con pid %d y con padre %d \n", pid1, getppid());
	}

	pid2 = fork();
	
	if (pid2 == 0) {
		printf("Soy el hijo 2 con pid %d y con padre %d \n", pid2, getppid());
		// execvp(args[0],args);
		execvp(argv[1], &argv[0]);	
	}
	else {	
		wait(NULL);
		printf("Soy el padre con pid %d y con padre %d \n", pid2, getppid());
	}
	
	return 0;
}
