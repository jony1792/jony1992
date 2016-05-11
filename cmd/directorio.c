#include "../smallsh.h"

void listar(char *directorioActual, int n, char *string){
	/* Variables */
	 DIR *dirp;
	 struct dirent *direntp;

	/* Abrimos el directorio */
	 dirp = opendir(directorioActual);
	 if (dirp == NULL){
		printf("Error: No se puede abrir el directorio\n");
	 	exit(2);
	 }

	/* Leemos las entradas del directorio */
	 while ((direntp = readdir(dirp)) != NULL) {
		/* Si la entrada es un Directorio --> recursividad*/
  		if (direntp->d_type==DT_DIR){
			if(strcmp(direntp->d_name, ".") != 0 && strcmp(direntp->d_name, "..") != 0){
				char * dir = (char *) malloc(2 + strlen(directorioActual)+ strlen(direntp->d_name) );
				strcpy(dir, directorioActual);
				strcat(dir, "/");		
				strcat(dir, direntp->d_name);		
				listar(dir, n, string);
			}
		}
		/*Si la entrada es un Fichero --> buscar las coincidencias de string en el contenido del Fichero*/
 		else{
			char * file = (char *) malloc(2 + strlen(directorioActual)+ strlen(direntp->d_name));
			strcpy(file, directorioActual);
			strcat(file, "/");		
			strcat(file, direntp->d_name);	

			unsigned coincidencias = cuentaStringArchivo(file, n, string);
			if(coincidencias > 0){
				if(n == 1){
					printf("%s\n", file);
				}
				else{
					printf("%d-%s\n", coincidencias, file);
				}
			}	
		}
	 }
	 
	 /* Cerramos el directorio */
	 closedir(dirp);
}

unsigned cuentaStringArchivo(char *nombreArchivo, int n, char *string){
	FILE *Fd;
	Fd=fopen(nombreArchivo,"r");
	int contador = 0;

	if (Fd==NULL)
         	printf("Error abriendo el fichero");

    	char cadena[200];

	while (fgets(cadena, 200, Fd) != NULL){
		int i = 0;
		for (i; i<strlen(cadena) && cadena[i] != '\0';) {
			int j = 0;
			int encontrado = 1;
			for(j; j < strlen(string); j++){
				if(string[j] == cadena[i]){
					if(j == strlen(string)-1){
						encontrado = 0;
					}
					else{
						i++;
					}
				}
				else{
					i++;
					break;
				}
			}
			if (encontrado == 0){
				contador++;
				encontrado = 1;
			}
		}
	} 
	return contador;
}
