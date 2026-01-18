#ifndef ADD_C
#define ADD_C
#include <string.h>
#include <gmp.h>
//Agrego esto más para que el IDE sepa cuáles son los prototipos
#include "../header/str.h"
#include "../header/stack.h"
#include "../header/global_vars.h"
#include "../header/define.h"
#include "../header/operators.h"

#include "./header/add.h"

// LONGFLOAT


// Basic type

// Antiguo.
struct type_value_err* add_int(int num, enum TYPE type_n2, void* num_2, bool is_right){
	static struct type_value_err out;
	int64_t tmp_i;
	void* tmp;
	out.type=type_n2;
	out.err=NORMAL;
	switch(type_n2){
		case CODES_BLOCKS:
			tmp=alloca(30);
			itoa(num,(char*)tmp, 10);
			tmp_i=strlen((char*)tmp)+strlen(num_2);
			
			out.value=malloc(tmp_i+((type_n2==STRING)?
				1: // String
				2 // Code block. For ' '
			));
			sprintf_with_invert(out.value,
				// String or CodeBlock.
				(type_n2==STRING)?"%s%s" : "%s %s",
				is_right,
				(char*)tmp,(char*)num_2
			);
			break;
		case STACK:
			tmp=alloca(sizeof(struct type_value));
			((struct type_value*)tmp)->type=INT;
			((struct type_value*)tmp)->value=malloc(sizeof(int));
			*(int*)((struct type_value*)tmp)->value=num;
			// @TODO: Ver si es necesario una macro
			if (is_right)
				stack_setItem((struct Header_Stack*)num_2,tmp,0,true);
			else
				add_stack(
					(struct Header_Stack*)num_2,
					INT,
					num_2
				);
			break;
		default:
			perror("Error interno.");
			out.err=APP_UNKNOWN_DATA;
	}
	return &out;
}

char* add_codes_block(char* codes,enum TYPE t, void* value, bool is_right){
	char* out=NULL;
	char* tmp;
	int count;
	switch (t){
		case STRING:
		case CODES_BLOCKS:
			count=strlen((char*)value)+strlen(codes);
			out=(char*)malloc(count+(t==STRING)?4:2);
			sprintf_with_invert(out,
				// String 4 or CodeBlock 2.
				(t==STRING)?"%s \"%s\"" : "%s %s",
				is_right,
				codes, (char*)value
			);
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
		case STACK:
			//Ponemos todo el arraincludey en una cadena.
			tmp=(void*)((struct Header_Stack*)value)->stack;
			while (tmp){
				now=&((struct Stack_*)tmp)->item;
				switch(now->type){
					case INT:
					case FLOAT:
						/* In GolfScript, numbers inside
						the stack are converted to characters
						The conversion is not done*/
						tmp=alloca(40);
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
			if (!tmp_str.count){
				if (tmp_str.str!=NULL){
					printf("An error has occurred in the function \"add_str\""
						"where \"tmp_str.str\" which should not have a value does.");
					exit(-1);/// TODO: Hacer codigos de errores personalizados, este seria MemoryLeap.
				}
				break;
			}
			//Hacemos el valor a retornar.
			*(int*)tmp=strlen(str)+tmp_str.count+1;
			out.value=malloc(*(int*)tmp);
			sprintf_with_invert(out.value, "%s%s", is_right, str, tmp_str.str);

			free(tmp_str.str);
			break;
		default:
			tmp=(void*)to_string_value(t,value);

			out.value=malloc(strlen(str)+strlen((char*)tmp)+1);
			sprintf_with_invert(out.value, "%s%s", is_right, str, (char*)tmp);
			// Para testear.
			#if defined(DEBUG) || defined(TEST_)
				if (t==LONGFLOAT || t==LONGINT)
					FREE__(tmp);
				else
			#endif
				//else:
					free(tmp);
	}
	return &out;
}

struct type_value_err* add_longint(mpz_t* long_int,enum TYPE t, void* value){
	static struct type_value_err out;
	mpz_t* copy_n=NULL;
	void* tmp;
	out.type=t;
	/*		break;
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
	}*/
	return &out;
}

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