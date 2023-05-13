#include <stdio.h>
#include <stdlib.h>
#include <signal.h> //nueva
#include <unistd.h>
#include <sys/wait.h>

/*programa que temporiza la ejecución de un proceso hijo */
pid_t gpid = 0;
void sigalarm_handler(int sig) {
    printf("Finalizacion por recepcion de señal\n");
    
    kill(gpid, SIGKILL);
}

int main(int argc, char *argv[])
{
    pid_t pid;
    int status;
    if (argc < 2)
    {
        printf("Uso: %s <nombre_ejecutable>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == 0) { // Proceso hijo
        gpid = getpid();
        if (execvp(argv[1], &argv[1]) == -1) {
            perror("Error al cambiar el ejecutable del proceso hijo");
            exit(EXIT_FAILURE);
        }
    } else { // Proceso padre
        signal(SIGALRM, sigalarm_handler);
        alarm(5);

        if (wait(NULL) == -1) {
            perror("Error al esperar al proceso hijo");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) printf("El proceso hijo finalizó normalmente.\n");
        else if (WIFSIGNALED(status)) printf("El proceso hijo fue finalizado por una señal (%d).\n", WTERMSIG(status));
    }

    exit(0);
}
