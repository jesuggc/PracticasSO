#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Copiar contenidos de entrada.txt a salida.txt
//Profe: Se pide hacer la copia en bloques de 512b, no byte a byte. Pero sería igual cambiando poca cosa. CAMBIADO

void copy(int fdo, int fdd) { // lee bytes de un archivo de origen y los escribe en un archivo de destino,
   //size_t bytes = sizeof(char);//de cuanto en cuanto se va recorriendo los ficheros
   size_t bytes = 512;
   char buffer[bytes];// en el que se almacenarán los datos leídos
   size_t cantidadLeida=bytes; //Si llega a un bloque del final y no lee justo 512 bytes podria dar error al escribir por eso escribimos la cantidad leida
   while ((cantidadLeida=read(fdo, &buffer, bytes)) !=0) { // devuelve el número de bytes leídos, si es 0 == EOF
      write(fdd, &buffer, cantidadLeida); //Escribe lo de buffer en fdd 
   }
}

int main(int argc, char *argv[]) {
   int in_file, out_file;

   in_file = open(argv[1], O_RDONLY);//El orgien sera primer parametro, solo querremos leerlo.
   out_file = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0660); //abrir el archivo en modo escritura (O_WRONLY), crear un archivo nuevo si no existe (O_CREAT) y truncar el archivo a cero bytes de longitud si ya existe (O_TRUNC).
   //0660 es un permiso que añadio profe de lab porque no se abria.  permisos de lectura y escritura para el propietario del archivo (6), permisos de lectura para los usuarios del mismo grupo (6) y ningún permiso para otros usuarios (0).
   if(in_file == -1 || out_file == -1) { //Si open da error devuelve -1, sino un numero entero.
      printf("El archivo no se pudo abrir");
    // Importante cerrar los ficheros 
      close(in_file);
      close(out_file);
      exit(1);
   }

   copy(in_file, out_file);//Son int porque open devuelve un int que sirve como identificador 

   close(in_file);
   close(out_file);

   return 0;
}