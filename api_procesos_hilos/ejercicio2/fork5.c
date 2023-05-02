#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* programa que crea N hijos siguiendo un grado de dependencias. Usa las
 * versiones de execl y execlp */


// int main(int argc, char **argv)
// {
// 	pid_t P1, P2, P3, P4, P5, P6, P7, P8;
// 	P1 = fork();
// 	if(P1 == 0){
// 		printf("Soy el P1");
// 		P2 = fork();
// 		if(P2 == 0) {

// 		}
// 		else{

// 		}
// 		P5 = fork();
// 		P7 = fork();
// 	}
// 	else { //MAIN
// 		wait(NULL);
// 		printf("Soy el padre");
// 	}
// 	return 0;
// }
int main(int argc, char **argv)
{
	pid_t P1, P2, P3, P4, P5, P6, P7, P8;
	P1 = fork();
	if(P1 == 0){
		printf("Soy P1 el fork devuelve: %d y el pid es: %d\n", P1, getpid());
		P2 = fork();
		if(P2 == 0) { //P2
			P3 = fork();
			if(P3 == fork()) { //P3
				printf("Soy P3");
			}
			else {
				P4 = fork();
				if (P4 == 0){
					printf("Soy P4");
				}
				else { // P3
					printf("Sigo siendo P3");
				}
			}
		}
		else { //P1
			P5 = fork();
			if(P5 == 0){ //P5
				printf("Soy P5");
			}
			else { //P1

			}
		}
		
	}
	else { //MAIN
		wait(NULL);
		printf("Soy el padre, el fork devuelve: %d y el pid es: %d\n", P1, getpid());
	}
	return 0;
}
