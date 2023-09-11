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
 * @todo: Arreglar todos los warning.
 * 
 * Agregar una forma de acceder a los elementos del array.
 * Estudiar el archivo example.gs
 * TODO agregar una opcion para que se importe archivos.
 * Investigar esta conversación: https://chat.stackexchange.com/transcript/message/62670441#62670441
 * @Termine el interprete, solo falta modificar la ejecucion y los operadores para terminar"
*/
const char* VERSION="V0";//0 porque todavia se esta en desarrollo.
const char* AUTHOR="  Interprete: Daniel Briceño.\n  Sintaxis: Darren Smith.";
const char* LICENSE_URL="https://raw.githubusercontent.com/dabl03/GolfScript/main/licence";
const char* LICENSE="Copyright (c) 2022 Daniel Alexander";
char* err_msg=0;
int CLIMIT_INT=MAX_INT_DIG;
int CLIMIT_FLOAT=DBL_DIG-1;
int quit=0;
int interprete(struct Array* stack,struct Array* vars);
void config_all(struct Array* opciones);
char* get_input_str(char type_string);
char* input_block(char init,char end,char* out_nesting,U_INT base_sub);
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
            struct Array lineas={1,0,malloc(sizeof(struct type_value))};
            long int size_file=0;
            char* str;
			//No existe.
			if (file==NULL){
				printf("El archivo \"%s\" no existe, revise la ruta.%s",(char*)path_files.value[0].value,ENDL);
				delete_array(&path_files);
				exit(EXIT_FAILURE);
			}
            //Movemos al final del archivo para ver donde termina su contenido.
            fseek(file, 0L, SEEK_END);
            size_file=ftell(file);
			fseek(file, 0L, SEEK_SET);//Regreso al inicio.
			
			str=(char*)malloc(size_file+2);
			//Leemos todo de una.
			fread(str,sizeof(char),size_file,file);
			str[size_file]='\0';//Seguridad.
			fclose(file);
			//Ejecutamos:
			add_array(&lineas,STRING,str);
			run(&lineas,&stack,&vars);
			//Liberamos todo.
			delete_array(&lineas);
			delete_array(&stack);
			delete_array(&vars);
			delete_array(&path_files);
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
	char *tmp_out=NULL;//Puntero de uso temporal.
	U_INT tmp_len=0;
	struct Array lineas={0,0,NULL};
	struct String c_linea={20,0,(char*)malloc(20)};
	printf("Golfscript Interactive Mode%s",ENDL);
	while (!quit)
	{
		if (sub){// Esta anidando algo.
			char *space = (char *)malloc(sizeof(char) * (sub + 1));
			U_INT i=0;
			for (; i < sub; i++)
				space[i] = ' ';
			space[i] = '\0';
			printf(space);
			free(space);
		}
		printf("> ");
		while(TRUE){
			char c=getchar();
			cadd_add_leftover(&c_linea,c);
			if (IF_ENDL(c))//Terminamos de pedir por teclado, o el usuario precionó la tecla ctrl+c
				break;
			if (IF_INIT_STRING(c)){//Si es el comienzo de una cadena entonces pedimos hasta que el usuario ya no quiera mas string.
				tmp_out=get_input_str(c);
				tmp_len=strlen(tmp_out);
				char is_new_line_tmp=tmp_out[tmp_len-1]=='\n';
				c_linea.str[--c_linea.count]='\0';
				str_add_str_init_end(&c_linea,tmp_out,0,tmp_len);
				free(tmp_out);
				if (is_new_line_tmp) break;
				continue;
			}else if(IF_INIT_COMENT(c)){//Innoramos todo despues del comentario.
				while ((c=getchar())!='\n' && c!='\0');
				break;
			}else if (c == '{'){ // Vemos si hay anidamiento. TODO: Usalo: input_block
				tmp_out=input_block('{','}',">> ",sub);
				c_linea.str[--c_linea.count]='\0';
				str_add_str_init_end(&c_linea,tmp_out,0,0);
				free(tmp_out);
				if (c_linea.str[c_linea.count-1]=='\n')break;
				continue;
			}else if(c=='['){///@TODO: Usarlo input_block.
				tmp_out=input_block('[',']',"-- ",sub);
				c_linea.str[--c_linea.count]='\0';
				str_add_str_init_end(&c_linea,tmp_out,0,0);
				free(tmp_out);
				if (c_linea.str[c_linea.count-1]=='\n')break;
				continue;
			}
		}
		c_linea.str[c_linea.count]='\0';
		add_array(&lineas,STRING,(char*)realloc(c_linea.str,c_linea.count+1));
		INIT_STRING(c_linea,20);
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
 * Pide al usuario una cadena con final terminando si hay salto de linea o fin del buffer.
 * @param  type_string El tipo de cadena("dobles", 'simple').
 * @return             Cadena dinamica, tiene que liberar.
 */
char* get_input_str(char type_string){
	struct String str_={20,1,(char*)malloc(20)};
	unsigned char input=0,
	is_scape=FALSE;//Para saber si escapamos la cadena.
	str_.str[0]=type_string;
	while(TRUE){
		input=getchar();
		cadd_add_leftover(&str_,input);
		if (str_.str[str_.count-1]==type_string AND !is_scape){//Si es termino y no es un escape.
			str_.str[str_.count-1]=type_string;
			break;
		}else if (input=='\n' || input=='\0'){
			str_.str[str_.count-1]=type_string;
			str_.str[str_.count++]='\n';
			break;
		}
		is_scape=(str_.str[str_.count-1]=='\\' && !is_scape);
	}
	cadd_add_leftover(&str_,'\0');
	return (char*)realloc(str_.str,str_.count);
}
/**
 * Aqui nos encargaremos de pedir la entrada al usuario hasta que llegue al fin del bloque
 * Ojo tomamos encuenta el anidamiento.
 * @param  init        El inicio del bloque.
 * @param  end         El final del bloque.
 * @param  out_nesting Lo que se enseña despues del identado. Puede ser "..." "--" ">>"
 * @param base_sub { Es para no perder la identación base }
 * @return             La entrada del usuario. Recordar liberar
 */
char* input_block(char init,char end,char* out_nesting,U_INT base_sub){
	U_INT sub=1;
	struct String out={20,1,(char*)malloc(20)};
	char c, is_nline=FALSE;//c this input, is_nline(is new line?), es para no tomar el '\0' antes de que el usuario si quiera escriba(He conseguido este error).
	out.str[0]=init;
	while(TRUE){
		c=getchar();
		if (c==init)
			sub++;
		else if (c==end){
			sub--;
			cadd_add_leftover(&out,c);
			break;
		}else if (IF_INIT_STRING(c)){//Obtenemos la cadena.
			char* str_=get_input_str(c);
			U_INT len_=strlen(str_);
			if (str_[len_-1]=='\n'){
				is_nline=TRUE;
			}
			str_add_str_init_end(&out,str_,0,len_);
			free(str_);
			continue;
		}else if (IF_INIT_COMENT(c)){//Ignoramos los comentarios.
			while((c=getchar()!='\n') AND c!='\0');
			is_nline=TRUE;
			continue;
		}else if(IF_ENDL(c))
			is_nline=TRUE;
		cadd_add_leftover(&out,c);
		if (is_nline){//Tiene que ser el ultimo por si el usuario solo usa una linea para el bloque.
			//Tambien se soluciona otro error al dejar esto de ultimo(Un char '\0' hechando
			//broma se obtiene en el primer prompt).
			for (U_INT i=0;i<base_sub+sub;i++){//Identamos.
				printf("  ");
			}
			printf(out_nesting);
			is_nline=FALSE;
			continue;
		}
	}
	cadd_add_leftover(&out,'\0');
	return (char*)realloc(out.str,out.count);
}
