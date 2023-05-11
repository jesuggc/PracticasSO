#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void copy(int fdo, int fdd) {
   size_t bytes = sizeof(char);
   char buffer;
  
   while (read(fdo, &buffer, bytes) != 0) {
       printf("entreeeee");
       write(fdd, &buffer, bytes);
   }
}

int main(int argc, char *argv[]) {
   int in_file, out_file;

   in_file = open(argv[1], O_RDONLY);
   out_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0660);

   if(in_file == -1 || out_file == -1) {
       printf("El archivo no se pudo abrir");
       exit(1);
   }

   copy(in_file, out_file);


   close(in_file);
   close(out_file);


   return 0;
}