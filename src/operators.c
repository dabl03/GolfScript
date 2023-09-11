#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./include/str.h"
#include "./include/stack.h"
#include "./include/global_vars.h"
/**
 * @todo        Hacer todas las operaciones suma deacuerdo al tipo de dato.  
 */
/**
 * @brief      Adds an integer.
 *
 * @param[in]  num      The number
 * @param[in]  type_n2  The type n 2
 * @param      num_2    The number 2
 *
 * @return     { description_of_the_return_value }
 */
int* add_int(int num,enum TYPE type_n2,void* num_2){
	struct type_value* out=malloc(sizeof(struct type_value*));
	enum TYPE out_t=NULL;
	switch(type_n2){ //[num_1 num_2]
		case INT:
			//Lo transformamos a entero largo si es necesario.
			if (num+*(int*)num_2>2.100.000.647){
				out->value=malloc(sizeof(mpz_t));
				mpz_init_set_si(*(mpz_t*)out->value,num);
				mpz_add_ui(*(mpz_t*)out->value,*(mpz_t*)out->value,*(int*)num_2);
				out_t=LONGINT;
			}else{
				out->value=malloc(sizeof(int));
				*(int*)out->value=num+*(int*)num_2;
			}
			break;
		case LONGINT:
			out->value=malloc(sizeof(mpz_t));
			mpz_init_set_si(*(mpz_t*)out->value,num);
			mpz_add_ui(*(mpz_t*)out->value,*(mpz_t*)out->value,*(int*)num_2);
			break;
		case FLOAT:
			*(double*)num_2->value+=*(double*)num_1->value;
			break;
		case STRING:
			tmp=to_string_value(INT,num_2->value);
			tmp_len=strlen(tmp)+strlen((char*)num_1->value)+1;
			num_1->value=realloc(num_1->value,tmp_len);
			strcat((char*)num_1->value,tmp);
			free(tmp);
			break;
		case CODES_BLOCKS:
			tmp=to_string_value(INT,num_2->value);
			tmp_len=strlen(tmp);
			num_1->value=realloc(num_1->value,tmp_len+strlen((char*)num_1->value)+4);
			tmp_len=strlen((char*)num_1->value);
			((char*)num_1->value)[tmp_len-2]='\0';
			sprintf((char*)num_1->value,"%s%s }",(char*)num_1->value,tmp);
			free(tmp);
			break;
		case ARRAY:
			tmp=malloc(sizeof(int));
			*(int*)tmp=*(int*)num_2->value;
			add_array((struct Array*)num_1->value,INT,tmp);
			break;
		default:
			perror("Caracteristica no disponible en la funcion add_operator\n");
			printf("Type num_2{%s}, type num_1{%s}",get_name_type(num_2->type),get_name_type(num_1->type));
			exit(-8);
	}
	out->type=(out_t!=NULL)?type_n2:out_t;
	return out;
}

#endif