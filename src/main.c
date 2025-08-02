#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <gmp.h>
#include "header/str.h"
#include "header/run.h"
#include "header/define.h"
#include "header/stack.h"
#include "header/global_vars.h"
const char* VERSION="V0";//0 porque todavia se esta en desarrollo.
const char* AUTHOR="  Interprete: Daniel Briceño.\n  Sintaxis: Darren Smith.";
const char* LICENSE_URL="https://raw.githubusercontent.com/dabl03/GolfScript/main/licence";
const char* LICENSE="Copyright (c) 2022 Daniel Alexander";
char* err_msg=0;
int CLIMIT_INT=MAX_INT_DIG;
int CLIMIT_FLOAT=DBL_DIG-1;
int quit=0;
/** Interpreta golfscript desde la consola
 * 
 * @param stack Pila donde se almacenara todos los datos. 
 * @param vars Pila que contendrá las variables.
 * @return int Error codes.
*/
int interprete(struct Header_Stack* stack,struct Header_Stack* vars);
/** Trata los flags por consola.
 * 
 * @param opciones Pila de string que representa los flags.
*/
void config_all(struct Header_Stack* opciones);
/** Pide al usuario una cadena con terminando si hay salto de linea o fin del buffer.
 * | Si consigue una comilla doble o simple, seguirá pidiendo hasta su termino.
 * @param  type_string El tipo de cadena.
 * @return             Cadena dinamica, tiene que liberar.
*/
char* get_input_str(char type_string);
/** Pide por consola hasta llegar a "cEnd".
 * | Si hay anidamiento seguirá pidiendo hasta que termine
 *
 * @param[in]  cInit,cEnd  Inicio y final del bloque
 * @param[in]  out_nesting Si hay anidamiento out_nesting se mostrará. Recomiendo "...", "--" o ">>"
 * @param[in] base_sub     Identación base.
 * @return La entrada del usuario. Recordar liberar
 */
char* input_block(const char cInit,const char cEnd,const char* out_nesting,const U_INT base_sub);
//Por si se quiere testear.
#ifdef __MAIN__
	#define main main_c
#endif
//Use macro para que el ide no vea error.
int main(int arc, char** argv){
	struct Header_Stack stack={NULL,NULL},//Nuestra pila.
	vars={NULL,NULL};//Nuestra variables
	int i_return_code=0;
	init_gvars(&vars);
	if (argc > 1){//Vemos que parametros se paso.
		struct Header_Stack params={NULL,NULL}, path_files={NULL,NULL};

		for (int i = 1; i < argc; i++)
		{
			char* opcion =(char*)malloc(sizeof(char)*(strlen(argv[i])+1));
			strcpy(opcion,argv[i]);
			if (opcion[0]=='-')
			{
				add_stack(&params,STRING,opcion);
				continue;
			}
			add_stack(&path_files,STRING,opcion);
		}
		if (params.i){
			config_all(&params);
			delete_stack(&params);
		}
		if (!path_files.i){
			i_return_code=interprete(&stack,&vars);
		}else{
			FILE* file=fopen((char*)path_files.value[0].value,"r");
			struct Array lineas={1,0,malloc(sizeof(struct type_value))};
			long int size_file=0;
			char* str;
			//No existe.
			if (file==NULL){
				printf("El archivo \"%s\" no existe, revise la ruta.%s",(char*)path_files.value[0].value,ENDL);
				delete_stack(&path_files);
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
			add_stack(&lineas,STRING,str);
			i_return_code=run(&lineas,&stack,&vars);
			//Liberamos todo.
			delete_stack(&lineas);
			delete_stack(&path_files);
		}
	}else{//Interpretamos.
		i_return_code=interprete(&stack,&vars);
	}
	delete_stack(&stack);
	delete_stack(&vars);
	#ifdef DEBUG
		puts("\n¿Hay fuga de memoria?:");
  	viewStack();
	#endif
	return i_return_code;
}
void config_all(struct Header_Stack* opciones){
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
int interprete(struct Header_Stack* stack,struct Header_Stack* vars){
	int sub = 0;	//para cambiar de >> a .. cuando hay una condición.
	char *tmp_out=NULL;//Puntero de uso temporal.
	U_INT tmp_len=0;
	struct Header_Stack lineas={NULL,NULL};
	struct String c_linea={0,0,NULL};
	char c='\0';
	printf("Golfscript Interactive Mode%s",ENDL);
	while (!quit){
		if (sub)
			for (unsigned int i=0; i < sub; i++)
				putchar(' ');
		printf("> ");
		while(true){
			c=getchar();
			cadd_leftover(&c_linea,c);
			if (IF_ENDL(c))//Terminamos de pedir por teclado, o el usuario precionó la tecla ctrl+c
				break;
			if (IF_INIT_STRING(c)){
				//" and '
				tmp_out=get_input_str(c);
				tmp_len=strlen(tmp_out);
				// If breakline.
				char is_new_line_tmp=tmp_out[tmp_len-1]=='\n';
				c_linea.str[--c_linea.count]='\0';
				str_add_str_end(&c_linea,tmp_out,tmp_len);
				free(tmp_out);
				if (is_new_line_tmp)
					break;
				continue;
			}else if(IF_INIT_COMENT(c)){//Innoramos todo despues del comentario.
				while ((c=getchar())!='\n' && c!='\0');
				break;
			}else if (c == '{'){ // Vemos si hay anidamiento. TODO: Usalo: input_block
				tmp_out=input_block('{','}',">> ",sub);
				c_linea.str[--c_linea.count]='\0';
				str_add_str(&c_linea,tmp_out);
				free(tmp_out);
				if (c_linea.str[c_linea.count-1]=='\n')
					break;
				continue;
			}else if(c=='['){///@TODO: Usarlo input_block.
				tmp_out=input_block('[',']',"-- ",sub);
				c_linea.str[--c_linea.count]='\0';
				str_add_str(&c_linea,tmp_out);
				free(tmp_out);
				if (c_linea.str[c_linea.count-1]=='\n')
					break;
				continue;
			}
		}
		c_linea.str[c_linea.count]='\0';
		add_stack(&lineas,STRING,c_linea.str);
		INIT_STRING(c_linea,80);

		if (sub == 0){ // Podemos interpretar linea a linea.
			run(&lineas, stack, vars);
			delete_stack(&lineas);
			//Mostramos la variable n.
			struct Var* this_var=(struct Var*)vars->value[search_var("n",vars)].value;
			char* extend=to_string_value(this_var->type,this_var->value);
			char* output = printf_stack(stack);//Obtenemos la pila.
			printf("[ %s]%s",output,extend);
			free(extend);
			free(output);
		}
	}
	free(c_linea.str);
	return 0;
}
char* get_input_str(char type_string){
	struct String str_={20,1,(char*)malloc(20)};
	unsigned char input=0;
	bool is_scape=false;//Para saber si escapamos la cadena.
	str_.str[0]=type_string;
	while(true){
		input=getchar();
		cadd_leftover(&str_,input);
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
	cadd_leftover(&str_,'\0');
	return (char*)realloc(str_.str,str_.count);
}
char* input_block(const char cInit,const char cEnd,const char* out_nesting,const U_INT base_sub){
	U_INT sub=1;
	struct String out={20,1,(char*)malloc(20)};
	char c;//c this input
	bool is_nline=false;//is_nline(is new line?), es para no tomar el '\0' antes de que el usuario si quiera escriba(He conseguido este error).
	out.str[0]=cInit;
	while(true){
		c=getchar();

		// Verificamos el anidamiento.
		if (c==cInit)
			sub++;
		else if (c==cEnd){
			sub--;
			cadd_leftover(&out,c);
			break;
		// Get string of console.
		}else if (IF_INIT_STRING(c)){
			char* str_=get_input_str(c);
			U_INT len_=strlen(str_);

			//Hubo un salto de linea a escribir la cadena.
			if (str_[len_-1]=='\n'){
				is_nline=true;
			}

			str_add_str_end(&out,str_,len_);
			free(str_);
			continue;
		}else if (IF_INIT_COMENT(c)){
			while((c=getchar()!='\n') AND c!='\0');
			is_nline=true;
			continue;
		}else if(IF_ENDL(c))
			is_nline=true;
		cadd_leftover(&out,c);

		// Nueva linea. ¡Identamos!.
		if (is_nline){
			for (U_INT i=0;i<base_sub+sub;i++){//Identamos.
				printf("  ");
			}
			printf(out_nesting);
			is_nline=false;
			continue;
		}
	}
	cadd_leftover(&out,'\0');
	return (char*)realloc(out.str,out.count);
}
