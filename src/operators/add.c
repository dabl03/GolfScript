#ifndef ADD_C
#define ADD_C
#include <string.h>
#include <gmp.h>
//Agrego esto más para que el IDE sepa cuáles son los prototipos
#include "../header/str.h"
#include "../header/stack.h"
#include "../header/global_vars.h"
#include "../header/define.h"

#include "./header/add.h"
/**
 * @TODO:Buscar la manera de mejorar la sintaxis.
*/
struct type_value_err* add_int(int num,enum TYPE type_n2,void* num_2){
	static struct type_value_err out;
	int64_t tmp_i;
	void* tmp;
	out.type=type_n2;
	out.err=NORMAL;
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
				mpz_clear(*(mpz_t*)tmp);
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
				FREE__(out.value);
				//LONGFLOAT.
				out.type=LONGFLOAT;
				out.value=malloc(sizeof(mpf_t));
				mpf_init_set_d(*(mpf_t*)out.value,*(double*)num_2);

				tmp=alloca(sizeof(mpf_t));
				mpf_init_set_d(*(mpf_t*)tmp,(double)num);
				
				mpf_add(*(mpf_t*)out.value,*(mpf_t*)out.value,*(mpf_t*)tmp);
				mpf_clear(*(mpf_t*)tmp);
			}
			break;
		case LONGFLOAT:
			out.value=malloc(sizeof(mpf_t));
			mpf_init_set(*(mpf_t*)out.value,*(mpf_t*)num_2);

			tmp=alloca(sizeof(mpf_t));
			mpf_init_set_d(*(mpf_t*)tmp,(double)num);

			mpf_add(*(mpf_t*)out.value,*(mpf_t*)out.value,*(mpf_t*)tmp);
			mpf_clear(*(mpf_t*)tmp);
			break;
		case STRING:
			tmp=alloca(30);
			itoa(num,(char*)tmp, 10);

			out.value=malloc(strlen((char*)tmp)+strlen(num_2)+1);
			sprintf(out.value,"%s%s",(char*)tmp,(char*)num_2);
			break;
		case CODES_BLOCKS:
			tmp=alloca(30);
			itoa(num,(char*)tmp, 10);

			out.value=malloc(strlen((char*)tmp)+strlen(num_2)+2);
			sprintf(out.value,"%s %s",(char*)tmp,(char*)num_2);
			break;
		case STACK:
			tmp=alloca(sizeof(struct type_value));
			((struct type_value*)tmp)->type=INT;
			((struct type_value*)tmp)->value=malloc(sizeof(int));
			*(int*)((struct type_value*)tmp)->value=num;

			stack_setItem((struct Header_Stack*)num_2,tmp,0,true);
			break;
		default:
			perror("Error interno.");
			out.err=APP_UNKNOWN_DATA;
	}
	return &out;
}

char* add_codes_block(char* codes,enum TYPE t, void* value){
	char* out=NULL;
	char* tmp;
	switch (t){
		case STRING:
		  out=(char*)malloc(strlen((char*)value)+strlen(codes)+4);
			sprintf(out,"%s \"%s\"",codes,(char*)value);
			break;
		case CODES_BLOCKS:
			out=(char*)malloc(strlen((char*)value)+strlen(codes)+2);
			sprintf(out,"%s %s",codes,(char*)value);
			break;
		case STACK:
			tmp=to_string_value(STACK,value);

			out=(char*)malloc(strlen(tmp)+strlen(codes)+2);//<+2 por ' ' y '\0'
			sprintf(out,"%s %s",codes,tmp);

			free(tmp);
			break;
		default:
			tmp=to_string_value(t,value);

			out=(char*)malloc(strlen(tmp)+strlen(codes)+2);
			sprintf(out,"%s %s",codes,tmp);

			// Para tester.
			if (t==LONGFLOAT || t==LONGINT)
				FREE__(tmp);
			else
				free(tmp);
	}
	return out;
}

struct type_value_err* add_str(char* str,enum TYPE t, void* value,bool is_right){
	static struct type_value_err out;
	struct String tmp_str={0,0,NULL};
	struct type_value* now=NULL;
	void* tmp=alloca(sizeof(int));
	out.type=STRING;
	switch(t){
		case CODES_BLOCKS:
		case STRING:
			*(int*)tmp=strlen(str)+strlen((char*)value)+1;
			out.value=malloc(*(int*)tmp);
			(is_right)?
				sprintf(out.value,"%s%s",str,(char*)value)
			:
				sprintf(out.value,"%s%s",(char*)value,str);
			break;
		case STACK:
			//Ponemos todo el array en una cadena.
			tmp=(void*)((struct Header_Stack*)value)->stack;
			while (tmp){
				now=&((struct Stack_*)tmp)->item;
				switch(now->type){
					case INT:
					case FLOAT:
						tmp=alloca(70);
						sprintf((char*)tmp,"\\x%.2x",*(int*)now->value);
						str_add_str(&tmp_str,(char*)tmp);
						break;
					default:
						tmp=(void*)to_string_value(now->type,now->value);
						str_add_str(&tmp_str,(char*)tmp);
						// Para tester.
						if (now->type==LONGFLOAT || now->type==LONGINT)
							// La API de gmplib (tmp=string) reselva memoria sin usar la macro malloc en la depuracion,
							// Asi que liberamos directamente para no generar falsas advertensia.
							FREE__(tmp);
						else
							free(tmp);
				}
				tmp=(void*)((struct Stack_*)tmp)->next;
				if (tmp==((struct Header_Stack*)value)->stack)
					break;
			}
			if (!tmp_str.count)
				break;
			//Hacemos el valor a retornar.
			*(int*)tmp=strlen(str)+tmp_str.count+1;
			out.value=malloc(*(int*)tmp);
			(is_right)?
				sprintf(out.value,"%s%s",str,tmp_str.str)
			:
				sprintf(out.value,"%s%s",tmp_str.str,str);

			free(tmp_str.str);
			break;
		default:
			tmp=(void*)to_string_value(t,value);

			out.value=malloc(strlen(str)+strlen((char*)tmp)+1);
			(is_right)?
				sprintf(out.value,"%s%s",str,(char*)tmp)
			:
				sprintf(out.value,"%s%s",(char*)tmp,str);
			// Para tester.
			if (t==LONGFLOAT || t==LONGINT)
				FREE__(tmp);
			else
				free(tmp);
	}
	return &out;
}

struct type_value_err* add_longint(mpz_t* long_int,enum TYPE t, void* value){
	static struct type_value_err out;
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
			  mpz_add(*(mpz_t*)out.value,*long_int,*(mpz_t*)tmp);
				mpz_clear(*(mpz_t*)tmp);
				break;
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
			mpf_clear(*(mpf_t*)tmp);
			break;
		case LONGFLOAT:break;/** 
		* @todo: Hacer...
		*/
		case STRING:
			tmp=(void*)mpz_get_str(NULL,10, *long_int);

			out.value=malloc(strlen((char*)value)+strlen((char*)tmp)+2);
			sprintf((char*)out.value,"%s%s",(char*)tmp,(char*)value);
			FREE__(tmp);
			break;
		case CODES_BLOCKS:
			tmp=(void*)mpz_get_str(NULL,10, *long_int);

			out.value=malloc(strlen((char*)value)+strlen((char*)tmp)+2);
			sprintf((char*)out.value,"%s %s",(char*)tmp,(char*)value);

			FREE__(tmp);
			break;
		case STACK:///@todo: Modificar el array original para no crear uno nuevo.
			copy_n=(mpz_t*)malloc(sizeof(mpz_t));
			mpz_init_set(*copy_n,*long_int);

			tmp=alloca(sizeof(struct type_value));
			((struct type_value*)tmp)->type=INT;
			((struct type_value*)tmp)->value=copy_n;
			
			stack_setItem((struct Header_Stack*)value,tmp,0,true);
			out.type=NONE;
			break;
		default:
			perror("Caracteristica no disponible en la funcion add_longint\n");
			printf("Type num_2{ %s }",get_name_type(t));
			exit(-8);
	}
	return &out;
}
struct type_value_err* add_longfloat(void);
	/**
	 * @todo ...
	 * */
struct type_value_err* add_float(void);
	/**
	 * @todo ...
	 * */

struct type_value_err* opr_add_stack(struct Header_Stack* h_stack,enum TYPE t, void* value){
	static struct type_value_err out;
	void* tmp=NULL;
	out.type=NONE;
	out.value=NULL;
	switch (t){
		case STRING:
			out.type=STRING;
			out.value=add_str((char*)value,STACK,(void*)h_stack,false)->value;
			break;
		case CODES_BLOCKS:
			out.type=CODES_BLOCKS;
			//Ponemos todo el array en una cadena.
			tmp=to_string_value(STACK,h_stack);

			out.value=malloc(strlen((char*)tmp)+strlen((char*)value)+5);
			sprintf((char*)out.value,"%s %s",(char*)tmp,(char*)value);

			FREE__(tmp);
			break;
		case STACK:
			tmp=(void*)h_stack->stack->next;
			// Next
			h_stack->stack->next=(((struct Header_Stack*)value)->stack)->next;
			(((struct Header_Stack*)value)->stack)->next=(struct Stack_*)tmp;
			// Previous
			h_stack->stack->next->previous=h_stack->stack;
			((struct Stack_*)tmp)->previous=((struct Header_Stack*)value)->stack;
			// Now stack is value->stack 
			h_stack->stack=((struct Header_Stack*)value)->stack;
			((struct Header_Stack*)value)->stack=NULL;
			break;
		default:
			add_stack(h_stack,t,value);
	}
	return &out;
}
//Para restar long int debes ver si ek resultado es menor a INT_MAX
#endif