#include <stdio.h>
#include <stdlib.h>
#include <signal.h> //nueva
#include <unistd.h>
#include <sys/wait.h>

/*programa que temporiza la ejecución de un proceso hijo */

void sigalarm_handler(int sig) {
    printf("Finalizacion por recepcion de señal\n");
    kill(getpid(), SIGKILL);
}

int main(int argc, char *argv[]) {
    pid_t pid;
    int status;

/*Comprueba si el numero de argumentos es valido (estrictamente 2)*/

    if (argc < 2) {
        printf("Uso: %s <nombre_ejecutable>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

/*Crea un proceso nuevo en cuyo hijo realiza el comando que le introducimos como argumento (ls, xeyes, cd...)*/

    pid = fork();
   
    if (pid == 0){// Proceso hijo
        if (execvp(argv[1], &argv[1]) == -1) {
            perror("Error al cambiar el ejecutable del proceso hijo");
            exit(EXIT_FAILURE);
        }
	}

/*

En el padre se establece una señal SIGALRM que hace lo de la funcion sigalarm_handler (SIGKILL al hijo) y luego con alarm se envia una señal 5 segundos despues
Finalmente se espera al proceso hijo con el wait de abajo y si el hijo termina nomrmalmente (antes de 5 segundos) se imprime el primer if y si no el segundo

*/
	else { // Proceso padre
        signal(SIGALRM, sigalarm_handler);
        // alarm(0.000000000000000001);
	    alarm(5);

        if (wait(&status) == -1) {
            perror("Error al esperar al proceso hijo");
            exit(EXIT_FAILURE);
        }

        if (WIFEXITED(status)) {
            printf("El proceso hijo finalizó normalmente.\n");
        } else if (WIFSIGNALED(status)) {
            printf("El proceso hijo fue finalizado por una señal (%d).\n", WTERMSIG(status));
        }
    }

    exit(0);
}
