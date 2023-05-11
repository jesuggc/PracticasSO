#include <stdio.h>
#include <stdlib.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */

int main(int argc, char **argv)
{
	char *args[] = {"ls", NULL}; //definimos argumentos aqui que se pasaria al ejer
	int pid1, pid2;
	pid1 = fork();

	if (pid1 == 0)
	{
		int ppid = getppid();
		printf("Soy el hijo 1 con pid %d y con padre %d \n", pid1, ppid);
		// Cambia el ejecutable del proceso hijo 1 a "ls"
	}
	else
	{

		wait(NULL);
		int ppid = getppid();
		printf("Soy el padre con pid %d y con padre %d \n", pid1, ppid);
	}

		pid2 = fork(); // Crea el segundo proceso hijo
		if (pid2 == 0)
		{
			int ppid = getppid();
		printf("Soy el hijo 2 con pid %d y con padre %d \n", pid2, ppid);
		execvp(args[0], args);
		
		}
		else
		{	
			wait(NULL);
			int ppid = getppid();
			printf("Soy el padre con pid %d y con padre %d \n", pid2, ppid);
		}
	
	return 0;
}
