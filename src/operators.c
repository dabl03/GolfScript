#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./include/operators.h"
/**
 * @todo        Hacer todas las operaciones suma deacuerdo al tipo de dato.  
 */
/**
 * @brief      Adds an integer.
 *
 * @param[in]  num      The number
 * @param[in]  type_n2  The type n 2
 * @param      num_2    The number 2
 * @param[] is_right    Solo para array, blockes codes, y cadena.
 ** Determina en que posicion se le agrega el entero(principio o final)
 ** True para primero se coloca el entero y despues la cadena.
 *
 * @return     struct type_valur* . Nuevo objeto. No es necesario liberar
 */
struct type_value* add_int(int num,enum TYPE type_n2,void* num_2,char is_right){
	static struct type_value out;
	int* copy_n=NULL;
	out.type=type_n2;
	switch(type_n2){ //[num_1 num_2]
		case INT:
			//Lo transformamos a entero largo si es necesario.
			if (num+*(int*)num_2>2100000647){
				out.value=malloc(sizeof(mpz_t));
				mpz_init_set_si(*(mpz_t*)out.value,num);
				mpz_add_ui(*(mpz_t*)out.value,*(mpz_t*)out.value,*(int*)num_2);
				out.type=LONGINT;
			}else{
				out.value=malloc(sizeof(int));
				*(int*)out.value=num+*(int*)num_2;
				out.type=INT;
			}
			break;
		case LONGINT:
			out.value=malloc(sizeof(mpz_t));
			mpz_init_set_si(*(mpz_t*)out.value,num);
			mpz_add_ui(*(mpz_t*)out.value,*(mpz_t*)out.value,*(int*)num_2);
			break;
		case FLOAT:
			out.value=malloc(sizeof(double));
			*(double*)out.value=*(double*)num_2+num;
			break;
		case STRING:
			out.value=malloc(strlen((char*)num_2)+20);
			if (is_right){
				sprintf((char*)out.value,"%d%s",num,(char*)num_2);
			}else{
				sprintf((char*)out.value,"%s%d",(char*)num_2,num);
			}
			out.value=realloc(out.value,strlen(out.value)+1);
			break;
		case CODES_BLOCKS:
			out.value=malloc(strlen((char*)num_2)+20);
			if (is_right){
				sprintf((char*)out.value,"%d %s",num,(char*)num_2);
			}else{
				sprintf((char*)out.value,"%s %d",(char*)num_2,num);
			}
			out.value=realloc(out.value,strlen(out.value)+1);
			break;
		case ARRAY:
			out.value=copy_array(num_2);
			copy_n=(int*)malloc(sizeof(int));
			*copy_n=num;
			array_set_item(out.value,TRUE,(is_right)?0:-1,INT,copy_n);
			break;
		default:
			perror("Caracteristica no disponible en la funcion add_int\n");
			printf("Type num_2{%s}",get_name_type(type_n2));
			exit(-8);
	}
	return &out;
}
/**
 * Agrega algo al bloque de codigo
 * @param  codes    Bloque de codigo
 * @param  t        Tipo de objeto agregar.
 * @param  value    valor
 * @param  is_right Si se agregara al inicio o final
 * @return          Char* . Recordar liberar.
 */
char* add_codes_block(char* codes,enum TYPE t, void* value,char is_right){
	char* out=NULL;
    char* tmp=to_string_value(t,value);
    if (is_right){
	    switch (t){
	    	case STRING:
		   		out=(char*)malloc(strlen(tmp)+strlen(codes)+4);
				sprintf(out,"%s \"%s\"",codes,tmp);
				break;
			case ARRAY:
				out=(char*)malloc(strlen(tmp)+strlen(codes)+5);
				sprintf(out,"%s [ %s]",codes,tmp);
				break;
			default:
				out=(char*)malloc(strlen(tmp)+strlen(codes)+1);
				sprintf(out,"%s %s",codes,tmp);
		}
	}else{
		switch (t){
	    	case STRING:
		   		out=(char*)malloc(strlen(tmp)+strlen(codes)+4);
				sprintf(out,"\"%s\" %s",tmp,codes);
				break;
			case ARRAY:
				out=(char*)malloc(strlen(tmp)+strlen(codes)+5);
				sprintf(out,"[ %s] %s",tmp,codes);
				break;
			default:
				out=(char*)malloc(strlen(tmp)+strlen(codes)+1);
				sprintf(out,"%s %s",tmp,codes);
		}
	}
    free(tmp);
    return out;
}
/**
 * @todo       : Recuerda si es un bloque de codigo la cadena se agrega dentro de este, si es un array recuerda hacer:
 Si el numero es mayor a FF entonces dejarlo en solo numero o si quieres hacer \xFF\FF\FF hasta poner todo el numero en la cadena.
 Recuerda que los bloques de codigo dentro de los array se agregan tal cual estan.
 Y la cadenas igual pero sin ""
 * @brief      Adds a longint.
 *
 * @param      str       The string
 * @param[in]  t         { parameter_description }
 * @param      value     The value
 * @param[in]  is_right  Indicates if right
 *
 * @return     { description_of_the_return_value }
 */
struct type_value* add_longint(char* str,enum TYPE t, void* value,char is_right){
	static struct type_value out;
	if (t==CODES_BLOCKS){

	}else if(t==ARRAY){

	}else{

	}
	return &out;
}
struct type_value* add_longint(mpz_t* long_int,enum TYPE t, void* value,char is_right){
	return NULL;
}
#endif