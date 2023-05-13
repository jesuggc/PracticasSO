#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//ln -s elMioLink laMiaCopia

void copy(int fdo, int fdd) {
	size_t bytes = sizeof(char);
	char buffer;

	while (read(fdo, &buffer, bytes) != 0) {
    	write(fdd, &buffer, bytes);
	}
}
void copiaSimbolica(char* in, char* out,struct stat st){
	int link_size = st.st_size + 1;
    	char *link_target = malloc(link_size);

    	if (readlink(in, link_target, link_size) == -1) {
        	perror("readlink");
        	free(link_target);
        	exit(1);
    	}

    	link_target[link_size - 1] = '\0';

    	if (symlink(link_target, out) == -1) {
        	perror("symlink");
        	free(link_target);
        	exit(1);
    	}

    	free(link_target);
}
void copiaRegular(char* in, char* out) {
	int in_file = open(in, O_RDONLY);
	int out_file = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0660);

	if (in_file == -1 || out_file == -1) {
		printf("El archivo no se pudo abrir\n");
		exit(1);
	}

	copy(in_file, out_file);

	close(in_file);
	close(out_file);
}

int main(int argc, char *argv[]) {
	int in_file, out_file;
	struct stat st;

	if (lstat(argv[1], &st) == -1) {
    	perror("lstat");
    	exit(1);
	}

	if (S_ISREG(st.st_mode)) copiaRegular(argv[1],argv[2]);	
	else if (S_ISLNK(st.st_mode)) copiaSimbolica(argv[1],argv[2], st);
	else printf("El fichero origen no es un fichero regular ni un enlace simbólico\n");

	return 0;
}
