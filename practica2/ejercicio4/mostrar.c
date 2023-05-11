#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
	int opt;
	int n = 0;
	int e_flag = 0;
	char *file_name;

	while ((opt = getopt(argc, argv, "n:e")) != -1) {
    	switch (opt) {
        	case 'n':
            	n = atoi(optarg);
            	break;
        	case 'e':
            	e_flag = 1;
            	break;
        	default:
            	fprintf(stderr, "Uso: %s [-n N] [-e] <archivo>\n", argv[0]);
            	exit(EXIT_FAILURE);
    	}
	}

	if (optind >= argc) {
    	fprintf(stderr, "Se requiere un argumento de archivo\n");
    	exit(EXIT_FAILURE);
	}

	file_name = argv[optind];

	int file_descriptor = open(file_name, O_RDONLY);
	if (file_descriptor < 0) {
    	perror("Error al abrir el archivo");
    	exit(EXIT_FAILURE);
	}

	off_t file_size = lseek(file_descriptor, 0, SEEK_END);
	off_t start_position = e_flag ? file_size - n : n;

	if (lseek(file_descriptor, start_position, SEEK_SET) < 0) {
    	perror("Error al cambiar la posición del marcador");
    	exit(EXIT_FAILURE);
	}

	char buf;
	ssize_t bytes_read;
	while ((bytes_read = read(file_descriptor, &buf, 1)) > 0) {
    	write(1, &buf, 1);
	}

	if (bytes_read < 0) {
    	perror("Error al leer el archivo");
    	exit(EXIT_FAILURE);
	}

	close(file_descriptor);
	return 0;
}
