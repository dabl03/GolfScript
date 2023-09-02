#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <gmp.h>
#include "include/str.h"
#include "include/run.h"
#include "include/define.h"
#include "include/stack.h"
#include "include/global_vars.h"
/**
 * @todo Usar la biblioteca gmp para los enteros grandes.
 * Tengo la documentación descargada en ingles.
 * @todo: Arreglar todos los warning.
 * Quitarle los saltos de lineas a los bloques de codigo.
 * 
 * Agregar una forma de acceder a los elementos del array.
 * Estudiar el archivo example.gs
 * 
 * Investigar esta conversación: https://chat.stackexchange.com/transcript/message/62670441#62670441
 */
const char* VERSION="V0";//0 porque todavia se esta en desarrollo.
const char* AUTHOR="    Interprete: Daniel Briceño.\n    Sintaxis: Darren Smith.";
const char* LICENCE="https://raw.githubusercontent.com/dabl03/GolfScript/main/licence";
char* err_msg=0;
int CLIMIT_INT=MAX_INT_DIG;
int CLIMIT_FLOAT=DBL_DIG-1;
int quit=0;
int interprete(struct Array* stack,struct Array* vars);
void config_all(struct Array* opciones);
int main(int argc, char *argv[]){
	struct Array stack={0,0,NULL},//Nuestra pila.
	vars={0,0,NULL};//Nuestra variables
	init_gvars(&vars);

	if (argc > 1){//Vemos que parametros se paso.
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
			delete_array(&lineas);
			delete_array(&stack);
			delete_array(&vars);
		}
	}else{//Interpretamos.
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
	char type_block=0;//0 no hay bloque, 1 codes block, 2 array.
	char c_linea[BUFFER];
	struct Array lineas={0,0,NULL};
	U_INT i=0;
	printf("Golfscript Interactive Mode%s",ENDL);
	
	while (!quit)
	{
		if (sub){// Esta anidando algo.
			char *space = (char *)malloc(sizeof(char) * (sub + 1));
			i = 0;
			for (; i < sub; i++)
				space[i] = ' ';
			space[i] = '\0';
			printf(space);
			free(space);
		}
		printf("> ");
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
					}else if(c=='\n'){//Enseñamos que todavia esta en una cadena.
						//c_linea[i]='\n';//Despues se escapa.
						printf("... ");
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
			}else if (c == '{') // Vemos si hay anidamiento. TODO: Usalo: input_block
				sub += 1;
			else if (c == '}')
			{ // Fin del anidamiento.
				if (sub == 0)
				{ // Espera no hubo nada que desanidar ;(
					printf("Advertencia: no hay suficiente anidamiento.");
					c='\0';
					break;
				}
				sub -= 1;
			}else if(c=='['){///@TODO: Usarlo input_block.
				sub += 1;
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
			//Mostramos la variable n.
    		struct Var* this_var=(struct Var*)vars->value[search_var("n",vars)].value;
			char* extend=to_string_value(this_var->type,this_var->value);
			char* output = printf_stack(stack);//Obtenemos la pila.
			if (extend==NULL)
				printf("[ %s]",output);
			else{
				printf("[ %s]%s",output,extend);
				free(extend);
			}
			free(output);
		}
	}
	return 0;
}
/**
 * Aqui nos encargaremos de pedir la entrada al usuario hasta que llegue al fin del bloque
 * Ojo tomamos encuenta el anidamiento.
 * @param  init        El inicio del bloque.
 * @param  end         El final del bloque.
 * @param  out_nesting Lo que se enseña despues del identado. Puede ser "..." "--" ">>"
 * @return             La entrada del usuario.
 * @TODO:{IF_INIT_STRING hacer una funcion que pida la entrada hasta que termine la cadena.}
 */
char* input_block(char init,char end,char* out_nesting){
	U_INT sub=1,max_len=20, p_chr=0;
	char c,  *out=(char*)malloc(20), is_nline=TRUE;//c this input, is_nline(is new line?)
	while(sub!=0){
		if (is_nline){
			for (U_INT i=0;i<sub;i++){//Identamos.
				printf("  ");
			}
			printf(out_nesting);
			is_nline=FALSE;
		}
		c=getchar();
		if (c==init)
			sub++;
		else if (c==end)
			sub--;
		else if (IF_INIT_STRING(c)){

		}else if (IF_INIT_COMENT(c)){
			while(c=getchar()!='\n' AND c!='\0');
		}
		if (p_chr+3==max_len){
			out=(char*)realloc(out,max_len+=20);
		}
		out[p_chr++]=c;
	}
	return NULL;
}
