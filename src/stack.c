#ifndef STACK_C
#define STACK_C 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./header/str.h"
#include "./header/run.h"
#include "./header/stack.h"
#include "./header/define.h"

bool add_stack(struct Header_Stack* stc_out,const enum TYPE typ_data, void* value){
	struct Stack_* new_stack=(struct Stack_*)malloc(sizeof(struct Stack_));
	if (new_stack==NULL)
		return true;
	//Enlazamos la cima con el suelo.
	// Para previous usar next como final, 
	// si usas previous terminaras en un bucle infinito.
	// do{}while(p_stack->next==NULL);
	if (stc_out->stack!=NULL){
		new_stack->previous=(stc_out->stack->next==NULL)?
			stc_out->stack
		:
			stc_out->stack->previous
		;
		stc_out->stack->previous=new_stack;
	}else{
		new_stack->previous=NULL;
	}
	// Importante.
	new_stack->item.type=typ_data;
	new_stack->item.value=value;
	new_stack->next=stc_out->stack;
	stc_out->stack=new_stack;
	return false;
}
struct type_value* pop_stack(struct Header_Stack* stc_){
	if (stc_->stack==NULL)
		return NULL;
	struct type_value* typ_out=(struct type_value*)malloc(sizeof(struct type_value));
	struct Stack_* stack_temp=stc_->stack;
	stc_->stack=stack_temp->next;
	typ_out->type=stack_temp->item.type;
	typ_out->value=stack_temp->item.value;
	free(stack_temp);
	return typ_out;
}
void delete_item(const enum TYPE t_typValue, void* v_data){
	switch(t_typValue){
		case VAR:
			delete_var((struct Var*)v_data);
			free(v_data);
			break;
		case FUNCTION:break;// ¡No liberar!
		case STACK:
			delete_stack((struct Header_Stack*)v_data);
			free(v_data);
		break;
		case LONGINT:
			mpz_clear(*(mpz_t*)v_data);
			free(v_data);
			break;
		case LONGFLOAT:
			mpf_clear(*(mpf_t*)v_data);
			free(v_data);
			break;
		case INT:
		case CODES_BLOCKS:
		case STRING:
		case FLOAT:
			free(v_data);
			break;
		default:
			perror("Error: Se intenta liberar un tipo de dato no tratado. \nFunción \"delete_item\" ");
			printf("Type: %s",get_name_type(t_typValue));
			exit(-130);
	}
}
void delete_stack(struct Header_Stack* stc_data){
	if (stc_data->stack==NULL)
		return;
	struct Stack_* stc_now=stc_data->stack;
	struct Stack_* stc_for_free=NULL;
	while (stc_now!=NULL){
		delete_item(stc_now->item.type,stc_now->item.value);
		stc_for_free=stc_now;
		stc_now=stc_now->next;
		free(stc_for_free);
	}
	stc_data->stack=NULL;
	//stc_data->father=NULL;//No se si liberar al padre tambien.
}
struct Header_Stack* copy_stack(const struct Header_Stack* stc_io){
	struct Header_Stack* stc_out=(struct Header_Stack*)malloc(sizeof(struct Header_Stack));
	struct Stack_* stack_now=stc_io->stack;
	void* v_generic;// Dinamic vars.
	
	stc_out->stack=NULL;
	stc_out->father=stc_io->father;// No se si copiamos los mismos padres.
	
	while (stack_now!=NULL){
		switch(stack_now->item.type){
			case VAR:// No se si agregarlo.
				perror("No se añadio la caracteristica.\n"
				  "En funcion copy_array->caso VAR.\n"
			  );
				exit(-4);
				break;
			case STACK:
				v_generic=copy_stack((struct Header_Stack*)stack_now->item.value);
				break;
			case INT:
				v_generic=malloc(sizeof(int));
				*(int*)v_generic=*(int*)stack_now->item.value;
				break;
			case FLOAT:
				v_generic=malloc(sizeof(double));
				*(double*)v_generic=*(double*)stack_now->item.value;
				break;
			case LONGINT:
				v_generic=malloc(sizeof(mpz_t));
				mpz_init(*(mpz_t*)v_generic);
				mpz_set(*(mpz_t*)v_generic,*(mpz_t*)stack_now->item.value);
				break;
			case LONGFLOAT:
				v_generic=malloc(sizeof(mpf_t));
				mpf_init_set(*(mpf_t*)v_generic,*(mpf_t*)stack_now->item.value);
				break;
			case CODES_BLOCKS:
			case STRING:
				v_generic=malloc(SIZE_CHAR(
					strlen((char*)stack_now->item.value)+1
				));
				strcpy((char*)v_generic,(char*)stack_now->item.value);
				break;
			default:
				perror("Error interno de la app, en la funcion copy_array->default.\n");
				printf("arr_allData->value[i].type= %s\n",get_name_type(stack_now->item.value) );
				exit(-2);
		}
		add_stack(stc_out,stack_now->item.type,v_generic);
		stack_now=stack_now->next;
	}
	return stc_out;
}
void set_item(
	struct Header_Stack* header,
	struct Stack_* next,
	struct Stack_* previous,
	struct Stack_* new_item,
	bool is_append
) {
	if (is_append){
		// Alteramos el nuevo elemento.
		new_item->next=previous;
		new_item->previous=previous->previous;
		// Alteramos los vecinos.
		previous->previous=new_item;
		if (new_item->previous->next!=NULL)
			new_item->previous->next=new_item;
		else
			header->stack=previous;
	}else{

	}
}
bool stack_setItem(
	struct Header_Stack* stc_out,
	const bool b_isAppend,
	const int i_indexSet,
	const enum TYPE typ_data,
	void* v_data
){
	struct Stack_* new_item=(struct Stack_*)malloc(sizeof(struct Stack_));
	struct Stack_* next=stc_out->stack;
	struct Stack_* previous_stack=NULL;
	U_INT i=i_indexSet;
	new_item->item.type=typ_data;
	new_item->item.value=v_data;
	if (i_indexSet>=0){
		if (i==0){
			//La cima.
			if (b_isAppend){
				add_stack(stc_out,typ_data,v_data);
			}else{
				delete_item(stc_out->stack->item.type,stc_out->stack->item.value);
				stc_out->stack->item.type=typ_data;
				stc_out->stack->item.value=v_data;
			}
			free(new_item);
			return false;
		}
		for (;next!=NULL;i--){
			if (i==0){
				if (b_isAppend){

					new_item->next=next;
				}else{
					new_item->next=next->next;
					new_item->previous=next-previous_stack;
					delete_item(next->item.type,next->item.value);
					free(next);
				}
				return false;
			}
			previous_stack=next;
			next=next->next;
		}
	}else{
		// modificar el final-i_indexSet.
		previous_stack=stc_out->stack;
		do{
			i++;
			next=previous_stack;
			previous_stack=previous_stack->previous;
			if (i==0){//LLegamos al destino
				if (b_isAppend){
					// Modificamos el actual.
					new_item->next=previous_stack;
					new_item->previous=previous_stack->previous;
					// Modificamos los vecinos.
					previous_stack->previous=new_item;
					if (new_item->previous->next!=NULL){
						new_item->previous->next=new_item;
					}else{
						//Alteramos stc_out
						stc_out->stack=new_item;
					}
				}else{
					delete_item(previous_stack->item.type,previous_stack->item.value);
					// Modificamos el actual.
					new_item->previous=previous_stack->previous;
					new_item->next=previous_stack->next;
					// Modificamos los vecinos:
					new_item->next->previous=new_item;
					if (new_item->previous->next!=NULL)//Si no es el suelo:
						new_item->previous->next=new_item;
					else{
						//Alteramos stc_out
						stc_out->stack=new_item;
					}
					free(previous_stack);
				}
				return false;
			}
		}while(next->next!=NULL);
	}
	return true;
}
void setValue_tv(struct Var* vr_now,const char* s_name,struct type_value* tv_setVar){
	char* str;
	// Var no defined
	if (s_name==NULL){
		delete_item(vr_now->type,vr_now->value);
	}else{
		unsigned int len=strlen(s_name),i=0;
		vr_now->name=(char*)malloc(SIZE_CHAR(len+1));
		vr_now->i_name=0;
		if (vr_now->name==NULL){
			PRINTF_MEMORY_ERROR(s_name);
			return;
		}
		for (;i<len;i++){
			vr_now->i_name+=s_name[i];
			vr_now->name[i]=s_name[i];
		}
		vr_now->name[i]='\0';
	}

	vr_now->type = tv_setVar->type;
	switch(vr_now->type){
		case INT:
			vr_now->value=malloc(sizeof(int));
			*(int*)vr_now->value=*(int*)tv_setVar->value;
			break;
		case FLOAT:
			vr_now->value=malloc(sizeof(double));
			*(double*)vr_now->value=*(double*)tv_setVar->value;
			break;
		case LONGINT:
			vr_now->value=malloc(sizeof(mpz_t));
			mpz_init(*(mpz_t*)vr_now->value);//Importante.
			mpz_set(*(mpz_t*)vr_now->value,*(mpz_t*)tv_setVar->value);
			break;
		case LONGFLOAT:
			vr_now->value=malloc(sizeof(mpf_t));
			mpf_init_set(*(mpf_t*)vr_now->value,*(mpf_t*)tv_setVar->value);
			break;
		case ARRAY:
			vr_now->value=copy_array((struct Array*)tv_setVar->value);
			break;
		case STRING:
		case CODES_BLOCKS:
			str=(char*)malloc(sizeof(char)*(strlen((char*)tv_setVar->value)+1));
			strcpy(str,(char*)tv_setVar->value);
			vr_now->value=str;
			break;
		case FUNCTION:
			vr_now->value=tv_setVar->value;
			break;
		default:
			vr_now->value=tv_setVar->value;
			// Si no está tratado, no se copiará.
			#ifdef TEST
				printf(
					"Advertensia: No se puede copiar dato.\n  Function=setValue_tv\n  tipo=%s.\n",
					vr_now->type
				);
			#endif
	}
}
void delete_var(struct Var* vr_var){
	delete_item(vr_var->type,vr_var->value);
	free(vr_var->name);
	vr_var->value=NULL;
}
void process_data(struct Array* arr_allData,struct Array* arr_allVars,struct Var* vr_data){
	void* v_generic;
	switch (vr_data->type){
	case INT:
		v_generic=malloc(sizeof(int));
		*(int*)v_generic=*(int*)vr_data->value;
		break;
	case FLOAT:
		v_generic=malloc(sizeof(double));
		*(double*)v_generic=*(double*)vr_data->value;
		break;
	case FUNCTION: //Usamos funciones dentro del mismo programa para esta.
		((unsigned short (*)(struct Array* arr_allData,struct Array* vars,char* extend))vr_data->value)(arr_allData,arr_allVars,"");
		return;
	case CODES_BLOCKS://Interpretamos el bloque de código.
		v_generic=malloc(sizeof(struct Array));
		((struct Array*)v_generic)->i=0;
		((struct Array*)v_generic)->max=0;
		((struct Array*)v_generic)->value=NULL;
		add_array((struct Array*)v_generic,STRING,vr_data->value);
		run((struct Array*)v_generic,arr_allData,arr_allVars);
		free(((struct Array*)v_generic)->value);
		free(v_generic);
		return;
	case STRING:
		v_generic=malloc(sizeof(char)*(strlen((char*)vr_data->value)+1));
		strcpy((char*)v_generic,(char*)vr_data->value);
		break;
	case ARRAY:
		v_generic=copy_array((struct Array*)vr_data->value);
		break;
	case LONGINT:
		v_generic=malloc(sizeof(mpz_t));
		mpz_init(*(mpz_t*)v_generic);
		mpz_set(*(mpz_t*)v_generic, *(mpz_t*)vr_data->value);
		break;
	case LONGFLOAT:
		v_generic=malloc(sizeof(mpf_t));
		mpf_init_set(*(mpf_t*)v_generic, *(mpf_t*)vr_data->value);
		break;
	default:
		printf("Error tipo %s no tratado en la función setValue_tv\n",get_name_type(vr_data->type) );
		exit(-3);
	}
	// Agregamos el dato procesado
	add_array(arr_allData,vr_data->type,v_generic);
}
char* printf_stack(struct Array* arr_allData){
	char* s_out=(char*)malloc(sizeof(char)*2);// Iniciamos para despues agregarle.
	char* s_generic;//Uso general.
	*s_out='\0';
	char* s_format;
	unsigned int len=1;
	unsigned int len_str_return;
	mp_exp_t mp_exponent;
	for(unsigned int i=0;i<arr_allData->i;i++){
		switch(arr_allData->value[i].type){
			case INT:
				// Creamos una cadena temporal y guardamos el antero
				s_generic=(char*)alloca(50);
				sprintf(s_generic,"%d ",*(int*)arr_allData->value[i].value);

				len=append_strcpy(&s_out,len,s_generic);
				break;
			case FLOAT:
				// Creamos una cadena temporal y guardamos el flotante.
				s_generic=(char*)alloca(60);
				sprintf(s_generic,"%f ",*(double*)arr_allData->value[i].value);

				len=append_strcpy(&s_out,len,s_generic);
				break;
			case ARRAY:
				// Convertimos el array en cadena para despues concatenarlo
				s_generic=printf_stack((struct Array*)arr_allData->value[i].value);
				len=append_sprintf(
					&s_out,
					len,
					4,
					"[ %s] ",// 4 -> "[ %s] "
					s_generic
				);
				free(s_generic);
				break;
			case CODES_BLOCKS:
				len=append_sprintf(
					&s_out,
					len,
					3,// 3 -> "{} "
					"{%s} ",
					(char*)arr_allData->value[i].value
				);
				break;
			case STRING:
				len=append_sprintf(
					&s_out,
					len,
					3,// 3 -> '"" '
					"\"%s\" ",
					(char*)arr_allData->value[i].value
				);
				break;
			case LONGINT:
				// Obtener la cadena.
				s_generic=mpz_get_str(NULL,0,*(mpz_t*)arr_allData->value[i].value);
				
				// 1 por: " "
				len=append_sprintf(
					&s_out,
					len,
					1,// 1 -> ' '
					"%s ",
					s_generic
				);
				FREE__(s_generic);
				break;
			case LONGFLOAT:
			  // Aprovechamos que gmp ofrece funciones para 
				// Obtener la cadena.
				s_generic=mpf_get_str(NULL, &mp_exponent, 10, 16, *(mpf_t*)arr_allData->value[i].value);
				mp_exponent+=(*s_generic=='-')?1:0;//Si es negativo se necesita sumar 1.

				// 2 -> ' ' and ','
				len_str_return=strlen(s_generic)+2;
				s_out=(char*)realloc(s_out,SIZE_CHAR(len+len_str_return));

				// 18: 7->"%.s,%s "+{s_generic 10 character}+1->\0.
				s_format=(char*)alloca(SIZE_CHAR(18));
				sprintf(s_format,"%%.%ds,%%s ",(int)mp_exponent);
				//Out:
				sprintf(
					s_out+SIZE_CHAR(len-1),
					s_format,
					s_generic,
					s_generic+(int)mp_exponent
				);
				len+=len_str_return;
				FREE__(s_generic);
				break;
			case VAR:
				s_generic=to_string_value(
					((struct Var*)arr_allData->value[i].value)->type,
					((struct Var*)arr_allData->value[i].value)->value
				);
				// 4 -> "(=) "
				len_str_return=strlen(((struct Var*)arr_allData->value[i].value)->name)+strlen(s_generic)+4;
				s_out=(char*)realloc(s_out,SIZE_CHAR(len+len_str_return));
				sprintf(
					s_out+SIZE_CHAR(len-1),
					"(%s=%s) ",
					((struct Var*)arr_allData->value[i].value)->name,
					s_generic
				);
				len+=len_str_return;
				free(s_generic);
				break;
			default:
				printf("Error: Tipo de dato \"%s\" no está tratado.",get_name_type(arr_allData->value[i].type));
				break;
		}
	}
	return s_out;
}
int64_t search_var_init(const char* s_name, unsigned const int i_initStr, struct Array* arr_var){
	unsigned int i_name=0;
	for (long int i=strlen(s_name)-1;i>-1;i--)
		i_name+=s_name[i];
	for (int64_t i=0;i<arr_var->i;i++){
		const struct Var* vr_varNow=(struct Var*)arr_var->value[i].value;//Variable actual.
		if (i_name==vr_varNow->i_name && !strcmp(&s_name[i_initStr],vr_varNow->name))//Es igual y retornamos.
			return i;
	}
	return -1;
}
void add_var(struct Array* arr_var,const char* s_name,enum TYPE typ_data,void* v_data){
	struct type_value tv_out={
		typ_data,
		v_data
	};
	struct Var* vr_now=(struct Var*)malloc(sizeof(struct Var));
	setValue_tv(
		vr_now,
		s_name,
		&tv_out
	);
	add_array(arr_var,VAR,vr_now);
}
char* to_string_value(const enum TYPE typ_data,void* v_data){
	char* s_out=NULL,
	*s_generic=NULL;
	mp_exp_t mp_exponent;
	void* vTmp;

	switch(typ_data){
		case INT:
			s_generic=(char*)alloca(30);
			itoa(*(int*)v_data,(char*)s_generic,10); 
			
			s_out=(char*)malloc(sizeof(char)*(strlen(s_generic)+1));
			strcpy(s_out,s_generic);
			break;
		case FLOAT:
			s_generic=(char*)alloca(30);
			sprintf(s_generic, "%.*lf", CLIMIT_FLOAT-2,*(double*)v_data);
			
			s_out=(char*)malloc(sizeof(char)*(strlen(s_generic)+1));
			strcpy(s_out,s_generic);
			break;
		case LONGINT:
			//Evitamos warning con los tester al no hacer: s_out=mpz_get_str(...);
			//2-> '-' is negative and '\0'
			s_out=(char*)malloc( SIZE_CHAR(mpz_sizeinbase((mpz_srcptr)v_data, 10) + 2) );
			mpz_get_str(s_out,10,*(mpz_t*)v_data);
			break;
		case LONGFLOAT:
			s_generic=mpf_get_str(NULL, &mp_exponent, 10, 0, *(mpf_t*)v_data);

			// 2 por: '\0' y ','
			s_out=(char*)malloc(strlen(s_generic)+2);

			// 19 -> 7 de "%.s,%s " + 10->2147483648 (10 caracteres)+ 1 -> \0+ 1 -> '-'.
			vTmp=alloca(sizeof(char)*19);
			sprintf(vTmp,"%%.%ds,%%s ",(int)mp_exponent);
			
			sprintf(s_out,vTmp,s_generic,s_generic+(int)mp_exponent);
			FREE__(s_generic);
		break;
		case CODES_BLOCKS:
		case STRING:
			s_out=(char*)malloc(
				SIZE_CHAR(strlen((char*)v_data)+1)
			);
			strcpy(s_out,v_data);
			break;
		case ARRAY:
			s_out=printf_stack((struct Array*)v_data);
			break;
		case FUNCTION:
			s_generic="(Native-Function)";
			s_out=(char*)malloc(sizeof(char)*(strlen(s_generic)+1));
			strcpy(s_out,s_generic);
			break;
		case VAR:
			s_generic=to_string_value(
				((struct Var*)v_data)->type,
				((struct Var*)v_data)->value
			);
			// 4 -> "(=)\0" 
			s_out=(char*)realloc(s_out,SIZE_CHAR(
				strlen( ((struct Var*)v_data)->name )+strlen(s_generic)+4)
			);
			sprintf(
				s_out,
				"(%s=%s)",
				((struct Var*)v_data)->name,
				s_generic
			);
			free(s_generic);
			break;
		default:
			return NULL;
	}
	return s_out;
}
#endif