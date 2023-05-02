#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*programa que crea dos hijos: uno no cambia de ejecutable y otro si */


int main(int argc, char **argv)
{

	pid_t pid1, pid2;
    pid1 = fork();

    if (pid1 == 0) {
        printf("Soy hijo: pid=%d, padre=%d\n", getpid(), getppid());

        pid2 = fork();
        if (pid2 == 0) {
            printf("Soy hijo 2: pid=%d, padre=%d\n", getpid(), getppid());
			if (execvp(argv[1], &argv[1]) < 0) {
                perror("execvp");
				exit(1);
            }
           
        } else {
            wait(NULL);
            printf("Soy padre 2: pid=%d, padre=%d\n", getpid(), getpid());
            exit(EXIT_SUCCESS);
        }
    } else {
        wait(NULL);
        printf("Soy padre: pid=%d, padre=%d\n", pid1, getpid());
        pid2 = fork();
        if (pid2 == 0) {
            printf("Soy hijo 2: pid=%d, padre=%d\n", getpid(), getppid());
            char *args[] = { argv[1], argv[2], NULL };
        } else {
            wait(NULL);
            printf("Soy padre 2: pid=%d, padre=%d\n", pid2, getpid());
            exit(EXIT_SUCCESS);
        }
    }

	return 0;
}


    
