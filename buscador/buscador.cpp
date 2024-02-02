//BUSCADOR DE CADENAS DE CARACTERES (GNU)
/*

Autor: Daniel Alejandro Padilla Rodriguez

Fecha de comenzado: 4 de junio de 2020
Fecha de terminado: 8 de junio de 2020

Este programa buscar una cadena de caracteres dentro de uno o 
varios ficheros encontrados en un directorio, en donde puede haber 
tambien varios directorios con mas ficheros. Busca la cadena en 
todos los archivos.

Especificaciones:
-)Buscará de entre todos los directorios que pueda tener un mismo directorio.
-)Leera todos los archivos de un directorio
-)Buscara la cadena de caracteres ingreso por el usuario
-)Si no se encuentra en un fichero, pasará al siguiente
-)De no encontrarse la palabra en los archivos de un directorio, 
pasara al siguiente
-)Se mostrara los datos en pantalla, conforma se vayan 
encontrando,en el siguiente modo

Numero de veces encontrado    Direccion del fichero
     en el fichero

-)El programa debe ejecutarse en la linea de comando, de la 
siguiente forma:

nombre_programa directorio palabra

programacion de funciones en el siguiente orden:
Directorios
Fichero
Interfaz

*/

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dirent.h>
//#include "timer.h"

#define LONG_MAX 0x7FFFFFFFFFFFFFFFL

int Error()
{
		
		printf("\x1B[2J \x1B[0;0H");
		
    printf("Memoria insuficiente. Se cierra el programa.");
    
    getchar();
    
    return 1;
}

//NUMERO DE CARACTERES EN UN FICHERO
unsigned long num_car_fich(FILE *A)
{
		
		unsigned long n = 0;
		
		rewind(A);
		
		while(!feof(A))
		{
				
			fgetc(A);
				
				n++;
				
		}
		
		return n;
		
}

unsigned int comp_cad(char *cadena, const char *pal)
{
		
		unsigned int ocu = 0;
		char *ext = NULL;
		unsigned log = 0; //Longitud de palabra
		int i;
		
		log = strlen(pal);
		
		//ASIGNACION DE MEMORIA A LA CADENA QUE EXTRAERA LOS CARACTERES PARA COMPARAR
		 if((ext = (char *) malloc ((log + 1) * sizeof(char))) == NULL) Error();
		
		*ext = cadena[0];
		
		for( i = 0; cadena[i] != '\0'; i++)
		{
				
				//LA CADENA ext EMPIEZA CARACTER POR CARACTER, Y CON EL, EMPIEZA LA COMPARACION
				strncpy(ext, cadena + i, log);
				
				ext[log] = '\0';
				
				if(strcmp(ext, pal) == 0) ocu++;
				
		}
		
		//LIBERAR MEMORIA
		free(ext);
		
		return ocu;
		
}

void lectura_fich(const char *d_arch, const char *pal)
{
		
		FILE *arch;
		char *cad = NULL;
		int c = 0;                  //CARACTER
		int i = 0;
		unsigned long num_car = 0; //NUMERO DE CARACTERES EN EL FICHERO
		unsigned int num_ocu = 0;  //NUMERO DE O URRENCIAS EN LAS QUE APARECE LA CADENA EN EL TEXTO
		
		if((arch = fopen(d_arch, "r+")) != NULL)
		{
				
				 //NUMERO DE CARACTERES EN EL FICHERO
				num_car = num_car_fich(arch);
				
				//ASIGNAR MEMORIA
				if((cad = (char *) malloc (num_car)) == NULL) Error();
				
				//SE COPIA EL TEXTO DEL FICHERO EN LA CADENA
				rewind(arch);
				
				while(!feof(arch))
				{
						
						c = (char)fgetc(arch);
						
						if(c == EOF || c == 0xff) continue;
						
						(c == '\n') ? cad[i++] = ' ' : cad[i++] = (char)c;
						
				}
				
				//COMPARACION DE CADENAS
				num_ocu = comp_cad(cad, pal);

				 //SE MUESTRA LA INFORMACION DE LAS OCURRENCIAS ENCONTRADAS
				 if(num_ocu)
						printf("\t%u \t\t %s\n", num_ocu, d_arch);
				
				//LIBERAR MEMORIA
				free(cad);
				
				fclose(arch);
				
		}
		
		return;
		
}

void busqueda_dir(char *d_dir, const char *dir_actual, const char *pal)
{
		
		DIR *dir;
		struct dirent *ent;
		char direct[256];
		
		strcat(d_dir, dir_actual);
		
		//LECTURA DEL DIRECTORIO
		if((dir = opendir(d_dir)) != NULL)
		{
				
				#if defined(_INC__MINGW_H)
				
				strcat(d_dir, "\\");
				
				#else 
				
				strcat(d_dir, "/");
				
				#endif
				
				
				while((ent = readdir(dir)) != NULL)
				{
						
						if((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0)) continue;
						
						 //INCORPORAR LOS NOMBRE A LA DIRECCION DEL DIRECTIORIO
						strcpy(direct, d_dir);
						
						//SE BUSCA FICHEROS EN LOS DIRECTORIOS DENTRO DE ESTE
						busqueda_dir(direct, ent->d_name, pal);
						
						//BUSQUEDA DE LA CADENA EN EL FICHERO
						lectura_fich(direct, pal);
						
						//if(comp_cad(ent->d_name, pal))
						   //  puts(direct);

						//EVITA ERRORES DE ESCRITURA
						strcpy(direct, d_dir);
						
						
				}
				
				closedir(dir);
				
				return;
				
		}
		
}

int main(int argc, char *argv[])
{
		
		char direct[] = {"/storage/emulated/0/Archivo"};
		
		//T_INICIAL
		
		/*
		if(argc > 3)
		{
				
				printf("sintaxis: nombre_programa directorio palabra");
				
				return -1;
				
		}*/
		
		printf("\x1B[2J \x1B[0;0H");
		
		//printf("Cadena: "); puts(argv[2]);
		
		printf("Numero de ocurrencias  \t\t   direccion\n");
		
		busqueda_dir(direct, "", "Ani"); // busqueda_dir(argv[1], "", argv[2]);
		
		printf("\n");
		
		//T_FINAL
		
		return 0;
		
}
