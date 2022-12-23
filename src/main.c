#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/str.h"
#include "include/run.h"
#include "include\define.h"
#define ARCHIVO_CON_VARIABLES_DEL_PREDECTERMINADAS "./include/vars.gs"
const char* VERSION="V0";//0 porque todavia se esta en desarrollo.
const char* AUTHOR="    Interprete: Daniel Briceño.\n    Sintaxis: Darren Smith.\nBigInt:    Daniel Clark";
const char* LICENCE="https://raw.githubusercontent.com/dabl03/GolfScript/main/licence";
char* err_msg;
int interprete(struct Array* stack,struct Array* vars);
void config_all(struct Array* opciones);
int main(int argc, char *argv[]){
	struct Array stack={0,0,NULL},
	vars={0,0,NULL};
	if (argc > 1)
	{
		struct Array params={0,0,NULL}, path_files={0,0,NULL};

		for (int i = 1; i < argc; i++)
		{
			char* opcion =(char*)malloc(sizeof(char)*(strlen(argv[i])+1));
			strcpy(opcion,argv[i]);
			if (opcion[0]=='-')
			{
				add_array(&params,STRING,opcion);
				continue;
			}
			add_array(&path_files,STRING,opcion);
		}
        if (params.i){
            config_all(&params);
			delete_array(&params);
        }
		if (!path_files.i){
            return interprete(&stack,&vars);
        }else{
			FILE* file=fopen((char*)path_files.value[0].value,"r");
            struct Array lineas={0,0,NULL};
			struct String str={0,0,NULL};
			init_str(&str,BUFFER);
			if (file==NULL)
			{
				printf("El archivo no existe, revise la ruta.%s",ENDL);
				delete_array(&path_files);
				free(str.str);
				exit(EXIT_FAILURE);
			}

			while (fgets(str.str, BUFFER, file)){
				char* linea=(char*)malloc(sizeof(char*)*(strlen(str.str)+1));
				strcpy(linea,str.str);
				add_array(&lineas,STRING,linea);
			}
			fclose(file);
			delete_array(&path_files);
			free(str.str);
			run(&lineas,&stack,&vars);
		}
	}else{
		return interprete(&stack,&vars);
	}
	return 0;
}
void config_all(struct Array* opciones){
    for(unsigned int i=0;i<opciones->i;i++){
        /****
         * @todo Hacer opciones de configuracion para configurarlas aqui.
         * Me gustaria una que esamble y compile el archivo para que sea mas facil analizarlo.
         * Otra -o
         * --help
         * --v o --version
         * --license
        */
    }
}
int interprete(struct Array* stack,struct Array* vars){
	int sub = 0;	//para cambiar de >> a .. cuando hay una condición.
	char c_linea[BUFFER];
	struct Array lineas={0,0,NULL};
	unsigned int i=0;
	printf("Golfscript Interactive Mode%s",ENDL);
	
	while (1)
	{
		if (sub) // Esta anidando algo.
		{
			char *space = (char *)malloc(sizeof(char) * (sub + 1));
			i = 0;
			for (; i < sub; i++)
				space[i] = ' ';
			space[i] = '\0';
			printf(space);
			free(space);
		}
		putchar('>');
		for(i=0;i<BUFFER;i++){
			char c=getchar();
			if (c=='\n' || c=='\0'){//Terminamos de pedir por teclado, o el usuario precionó la tecla ctrl+c
				c_linea[i++]=c;
				c_linea[i]='\0';
				break;
			}
			c_linea[i]=c;
			if (IF_INIT_STRING(c)){//Si es el comienzo de una cadena entonces pedimos hasta que el usuario ya no quiera mas string.
				/**
				 * @todo Colocar un resaltado a esto.
				 * 
				 */
				unsigned short is_scape=0;
				char type=c;
				i++;
				for(;i<BUFFER;i++){
					c=getchar();
					c_linea[i]=c;
					if (c==type){
						if (!is_scape)
							break;//Ya salimos de la subcadenas.
					}else if(c=='\n'){//Transformamos los saltos de lineas en su analogo \\n
						c_linea[i++]='\\';
						c_linea[i]='n';//Vamos por el siguiente caracter:)
						printf("... ");//Enseñamos que todavia esta en una cadena.
						continue;
					}else if(c=='\0'){
						i=BUFFER;
						break;
					}
					is_scape=(c=='\\' && !is_scape);
				}
				continue;
			}else if(IF_INIT_COMENT(c)){//Innoramos todo despues del comentario.
				while ((c=getchar())!='\n' && c!='\0');
				break;
			}else if (c == '{') // Vemos si hay anidamiento.
				sub += 1;
			else if (c == '}')
			{ // Fin del anidamiento.
				if (sub == 0)
				{ // Espera no hubo nada que desanidar ;(
					printf("Advertencia no hay suficiente anidamiento.");
					c='\0';
					break;
				}
				sub -= 1;
			}
		}
		c_linea[i]='\0';
		char* l=(char*)malloc(sizeof(char)*(i+1));
		strcpy(l,c_linea);
		add_array(&lineas,STRING,l);
		if (sub == 0)
		{ // Podemos interpretar linea a linea.
			run(&lineas, stack, vars);
			delete_array(&lineas);
			char* output = printf_stack(stack);
			printf("[ %s]%s",output,ENDL);
			free(output);
		}
	}
	return 0;
}