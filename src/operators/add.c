#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
//Agrego esto más para que el IDE sepa cuáles son los prototipos
#include "../header/str.h"
#include "../header/stack.h"
#include "../header/global_vars.h"
#include "../header/define.h"

#include "./header/add.h"

/**
 * @brief      Adds an integer.
 *
 * @param[in] num      The number
 * @param[in] type_n2  The type n 2
 * @param[] num_2    The number 2
 * @param[] is_right    Solo para array, blockes codes, y cadena.
 ** Determina en que posicion se le agrega el entero(principio o final)
 ** True para primero se coloca el entero y despues la cadena.
 *
 * @return tatic struct type_valur* -  El resultado, no se requiere liberar la estructura.
 */
struct type_value* add_int(int num,enum TYPE type_n2,void* num_2,bool is_right){
	static struct type_value out;
	int* copy_n=NULL;
	int64_t tmp_i;
	void* tmp;
	out.type=type_n2;
	switch(type_n2){
		case INT:
			tmp_i=num+*(int*)num_2;
			//Primero verificamos si hubo un desbordamiento.
			if (tmp_i<INT_MIN || tmp_i>INT_MAX){
				out.value=malloc(sizeof(mpz_t));
				mpz_init_set_si(*(mpz_t*)out.value,num);

				tmp=alloca(sizeof(mpz_t));
				mpz_init_set_si(*(mpz_t*)tmp,*(int*)num_2);

				mpz_add(*(mpz_t*)out.value,*(mpz_t*)out.value,*(mpz_t*)tmp);
				out.type=LONGINT;
				break;
			}
			out.value=malloc(sizeof(int));
			*(int*)out.value=num+*(int*)num_2;
			break;
		case LONGINT:
			out.value=malloc(sizeof(mpz_t));
			mpz_init_set_si(*(mpz_t*)out.value,num);
			
			mpz_add(*(mpz_t*)out.value,*(mpz_t*)out.value,*(mpz_t*)num_2);
			break;
		case FLOAT:
			out.value=malloc(sizeof(double));
			*(double*)out.value=*(double*)num_2+num;

			//Si se desborda el double
			if (isinf(*(double*)out.value)){
				free(out.value);
				//LONGFLOAT.
				out.type=LONGFLOAT;
				out.value=malloc(sizeof(mpf_t));
				mpf_init_set_d(*(mpf_t*)out.value,*(double*)num_2);

				tmp=alloca(sizeof(mpf_t));
				mpf_init_set_d(*(mpf_t*)tmp,(double)num);
				
				mpf_add(*(mpf_t*)out.value,*(mpf_t*)out.value,*(mpf_t*)tmp);
			}
			break;
		case LONGFLOAT:
			out.value=malloc(sizeof(mpf_t));
			mpf_init_set(*(mpf_t*)out.value,*(mpf_t*)num_2);

			tmp=alloca(sizeof(mpf_t));
			mpf_init_set_d(*(mpf_t*)tmp,(double)num);

			mpf_add(*(mpf_t*)out.value,*(mpf_t*)out.value,*(mpf_t*)tmp);
			break;
		case STRING:
			tmp=alloca(30);
			itoa(num,(char*)tmp, 10);

			out.value=malloc(strlen((char*)tmp)+strlen(num_2)+1);
			(is_right)?
				sprintf(out.value,"%s%s",(char*)tmp,(char*)num_2)
			:
				sprintf(out.value,"%s%s",(char*)num_2,(char*)tmp)
			;
			break;
		case CODES_BLOCKS:
			out.value=add_codes_block((char*)num_2,INT,(void*)&num,false);
			break;
		case ARRAY:
			out.value=copy_array(num_2);
			copy_n=(int*)malloc(sizeof(int));
			*copy_n=num;
			array_set_item(out.value,true,(is_right)?0:-1,INT,copy_n);
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
 * @param codes    Bloque de codigo
 * @param t        Tipo de objeto agregar.
 * @param value    valor
 * @param is_right Si se agregara al inicio o final
 * @return Char*  - El resultado, recordar liberar.
 */
char* add_codes_block(char* codes,enum TYPE t, void* value,bool is_right){
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
 * @brief      Concatena str con tipo de dato.
 *
 * @param str          The string
 * @param[in] t        parameter_description
 * @param value        The value
 * @param[in] is_right Indicates if right
 *
 * @return static struct type_value* -  El resultado, no se requiere liberar la estructura.
 */
struct type_value* add_str(char* str,enum TYPE t, void* value,bool is_right){
	static struct type_value out;
	U_INT tmp_len=0;
	out.type=STRING;
	if (t==CODES_BLOCKS){
		tmp_len=strlen(str)+strlen((char*)value)+1;
		out.value=malloc(tmp_len);

		(is_right)?
			sprintf(out.value,"%s%s",str,(char*)value)
		:
			sprintf(out.value,"%s%s",(char*)value,str);
	}else if(t==ARRAY){//Revisar.
		struct String tmp_str={0,0,NULL};
		for (U_INT i=0;i<((struct Array*)value)->i;i++){
			char* tmp_str_for=to_string_value(((struct Array*)value)->value[i].type,((struct Array*)value)->value[i].value);
			str_add_str(&tmp_str,tmp_str_for);
			free(tmp_str_for);
		}
		tmp_len=strlen(str)+tmp_str.count+1;
		out.value=malloc(tmp_len);
		if (is_right)
			sprintf(out.value,"%s%s",str,tmp_str.str);
		else
			sprintf(out.value,"%s%s",tmp_str.str,str);
		free(tmp_str.str);
	}else{
		char* tmp_str=to_string_value(t,value);
		out.value=malloc(strlen(str)+strlen(tmp_str)+1);
		if (is_right)
			sprintf(out.value,"%s%s",str,tmp_str);
		else
			sprintf(out.value,"%s%s",tmp_str,str);
		free(tmp_str);
	}
	return &out;
}
/***
 * @brief Sumas un LONGINT con un dato.
 * 
 * @param long_int - El LONGINT
 * @param t - El tipo del segundo dato
 * @param value - El segundo dato
 * @param is_right - Si el LONGINT es el primer sumando o no.
 * @return static struct type_value - El resultado, no se requiere liberar la estructura. 
*/
struct type_value* add_longint(mpz_t* long_int,enum TYPE t, void* value,bool is_right){
	static struct type_value out;
	mpz_t* copy_n=NULL;
	void* tmp;
	out.type=t;
	switch(t){
		case INT:
		case LONGINT:
			out.type=LONGINT;
			
			out.value=malloc(sizeof(mpz_t));
			mpz_init(*(mpz_t*)out.value);

			tmp=value;
			if (t==INT){
				tmp=alloca(sizeof(mpz_t));
				mpz_init_set_si(*(mpz_t*)tmp,*(int*)value);
			}
			mpz_add(*(mpz_t*)out.value,*long_int,*(mpz_t*)tmp);
			break;
		case FLOAT:
			//Debemos combertir primero a LONGDOUBLE los dos operando para realizar la suma.
			out.type=LONGFLOAT;
			
			out.value=malloc(sizeof(mpf_t));
			mpf_init(*(mpf_t*)out.value);
			mpf_set_z(*(mpf_t*)out.value,*long_int);

			tmp=alloca(sizeof(mpf_t));// Segundo operando.
			mpf_init(*(mpf_t*)tmp);
			mpf_set_d(*(mpf_t*)tmp, *(double*)value);

			mpf_add(*(mpf_t*)out.value,*(mpf_t*)out.value,*(mpf_t*)tmp);
			break;
		case STRING:
			tmp=(void*)mpz_get_str(NULL,10, *long_int);
			out.value=malloc(strlen((char*)value)+strlen((char*)tmp)+2);

			if (is_right)
				sprintf((char*)out.value,"%s%s",(char*)tmp,(char*)value);
			else
				sprintf((char*)out.value,"%s%s",(char*)value,(char*)tmp);
			free(tmp);
			break;
		case CODES_BLOCKS:
			tmp=(void*)mpz_get_str(NULL,10, *long_int);
			out.value=malloc(strlen((char*)value)+strlen((char*)tmp)+2);
			if (is_right)
				sprintf((char*)out.value,"%s %s",(char*)tmp,(char*)value);
			else
				sprintf((char*)out.value,"%s %s",(char*)value,(char*)tmp);
			free(tmp);
			break;
		case ARRAY:
			out.value=copy_array(value);
			copy_n=(mpz_t*)malloc(sizeof(mpz_t));
			mpz_init_set(*copy_n,*long_int);
			array_set_item(out.value,true,(is_right)?0:-1,LONGINT,copy_n);
			break;
		default:
			perror("Caracteristica no disponible en la funcion add_longint\n");
			printf("Type num_2{ %s }",get_name_type(t));
			exit(-8);
	}
	return &out;
}
//Para restar long int debes ver si ek resultado es menor a INT_MAX
#endif