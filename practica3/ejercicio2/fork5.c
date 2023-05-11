#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// programa que crea N hijos siguiendo un grado de dependencias. Usa las
// versiones de execl y execlp 


int main(int argc, char **argv)
{
	int pid1=fork();
	if (pid1==0){
		execlp("echo", "echo", "Soy el hijo 1", NULL);
		exit(-1);
	}

	wait(NULL);

	int pid2=fork();
	if (pid2==0){
		execl("/bin/echo", "echo", "Soy el hijo 2", NULL);
		exit(-1);
	}

	int pid5=fork();
	if (pid5==0){
		execlp("echo", "echo", "Soy el hijo 5", NULL);
		exit(-1);
	}

	int pid7=fork();
	if (pid7==0){
		execlp("echo", "echo", "Soy el hijo 7", NULL);
		exit(-1);
	}

	waitpid(pid2,NULL,0);
	int pid4=fork();
	if (pid4==0){
		waitpid(pid2,NULL,0);
		execl("/bin/echo", "echo", "Soy el hijo 4", NULL);
		exit(-1);
	}

	int pid3=fork();
	if (pid3==0){
		execlp("echo", "echo", "Soy el hijo 3", NULL);
		exit(-1);
	}
	
	waitpid(pid4,NULL,0);
	waitpid(pid5,NULL,0);
	int pid6=fork();
	if (pid6==0){
		execl("/bin/echo", "echo", "Soy el hijo 6", NULL);
		exit(-1);
	}

	waitpid(pid3,NULL,0);
	waitpid(pid6,NULL,0);
	waitpid(pid7,NULL,0);
	int pid8=fork();
	if (pid8==0){
		execl("/bin/echo", "echo", "Soy el hijo 8", NULL);
		exit(-1);
	}

	waitpid(pid8,NULL,0);
	printf("Soy el padre, PID = %d\n", getpid());

	return 0;
}