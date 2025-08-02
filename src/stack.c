#ifndef STACK_C
#define STACK_C 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./header/str.h"
#include "./header/run.h"
#include "./header/stack.h"
#include "./header/define.h"

bool add_stack(struct Header_Stack* hstc_out,const enum TYPE typ_data, void* value){
	struct Stack_* stc_new=(struct Stack_*)malloc(sizeof(struct Stack_));
	if (stc_new==NULL)
		return true;
	//Enlazamos la cima con el suelo.
	// Para previous usar next como final, 
	// si usas previous terminaras en un bucle infinito.
	// do{}while(p_stack->next==NULL);
	if (hstc_out->stack!=NULL){
		stc_new->previous=(hstc_out->stack->next==NULL)?
			hstc_out->stack
		:
			hstc_out->stack->previous
		;
		hstc_out->stack->previous=stc_new;
	}else{
		stc_new->previous=NULL;
	}
	// Importante.
	stc_new->item.type=typ_data;
	stc_new->item.value=value;
	stc_new->next=hstc_out->stack;
	hstc_out->stack=stc_new;
	return false;
}
struct type_value* pop_stack(struct Header_Stack* hstc_modific){
	if (hstc_modific->stack==NULL)
		return NULL;
	// Nota: No static porque apuntará a otro elemento al llamar dos veces.
	struct type_value* tv_out=(struct type_value*)malloc(sizeof(type_value*));
	struct Stack_* stc_temp=hstc_modific->stack;
	hstc_modific->stack=stc_temp->next;
	hstc_modific->stack->previous=stc_temp->previous;

	tv_out->type=stc_temp->item.type;
	tv_out->value=stc_temp->item.value;
	free(stc_temp);
	return tv_out;
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
void delete_stack(struct Header_Stack* hstc_data){
	if (hstc_data->stack==NULL)
		return;
	struct Stack_* stc_now=hstcdata->stack;
	struct Stack_* stc_for_free=NULL;
	while (stc_now!=NULL){
		delete_item(stc_now->item.type,stc_now->item.value);
		stc_for_free=stc_now;
		stc_now=stc_now->next;
		free(stc_for_free);
	}
	hstc_data->stack=NULL;
	//stc_data->father=NULL;//No se si liberar al padre tambien.
}
struct type_value* copy_item(const type_value* tv_src,const TYPE typ_io,const void* value_io){
  struct type_value* tv_out=
  	(tv_src==NULL)?malloc(sizeof(struct type_value))
  		:
  	tv_src
  ;// Si pasa null creamos uno nuevo, de lo contrario usamos el que se paso.
  tv_out->type=typ_io;
  char* str;
	switch(typ_io){
			case VAR:// No se si agregarlo.
				perror("No se añadio la caracteristica.\n"
				  "En funcion copy_item->caso VAR.\n"
			  );
				exit(-4);
				break;
		case INT:
			tv_out->value=malloc(sizeof(int));
			*(int*)tv_out->value=*(int*)value_io;
			break;
		case FLOAT:
			tv_out->value=malloc(sizeof(double));
			*(double*)tv_out->value=*(double*)value_io;
			break;
		case LONGINT:
			tv_out->value=malloc(sizeof(mpz_t));
			mpz_init(*(mpz_t*)tv_out->value);//Importante.
			mpz_set(*(mpz_t*)tv_out->value,*(mpz_t*)value_io);
			break;
		case LONGFLOAT:
			tv_out->value=malloc(sizeof(mpf_t));
			mpf_init_set(*(mpf_t*)tv_out->value,*(mpf_t*)value_io);
			break;
		case STACK:
			tv_out->value=copy_stack((struct Header_Stack*)value_io);
			break;
		case STRING:
		case CODES_BLOCKS:
			str=(char*)malloc(sizeof(char)*(strlen((char*)value_io)+1));
			strcpy(str,(char*)value_io);
			tv_out->value=str;
			break;
		case FUNCTION:
			tv_out->value=value_io;
			break;
		default:
			tv_out->value=value_io;
			// Si no está tratado, no se copiará.
			#if defined(DEBUG) || defined(TEST_)
				printf(
					"Advertensia: No se puede copiar dato.\n  Function=copy_item\n  tipo=%s.\n",
					typ_io
				);
			#endif
	}
	return tv_out;
}
struct Header_Stack* copy_stack(const struct Header_Stack* hstc_io){
	struct Header_Stack* hstc_out=(struct Header_Stack*)malloc(sizeof(struct Header_Stack));
	struct Stack_* stc_now=hstc_io->stack;
	void* v_generic;// Dinamic vars.
	struct type_value tv_tmp;
	hstc_out->stack=NULL;
	hstc_out->father=hstc_io->father;// No se si copiamos los mismos padres.
	
	while (stc_now!=NULL){
		copy_item(
			&tv_tmp,
			stc_now->item.type,
			stc_now->item.value
		);
		add_stack(hstc_out, tv_tmp.type, tv_tmp.value);
		stc_now=stc_now->next;
	}
	return hstc_out;
}
void setValue_tv(struct Var* vr_now,const char* s_name,struct type_value* tv_setVar){
	// This Var is defined
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
	copy_item(vr_now->type,tv_setVar->type,tv_setVar->value);
}
void delete_var(struct Var* vr_var){
	delete_item(vr_var->type,vr_var->value);
	free(vr_var->name);
	vr_var->value=NULL;
}
void process_data(struct Header_Stack* hstc_stack,struct Header_Stack* hstc_var,struct Var* vr_data){
	struct type_value tv_tmp;
	void* v_generic;
	U_INT err=0;// Solo para funciones.
	switch (vr_data->type){
		// Funciones nativas del lenguaje
		case FUNCTION:
			err=((U_INT (*)(
				struct Header_Stack*,
				struct Header_Stack*,
				char* extend
			))vr_data->value) (hstc_stack,hstc_var,"");
			break;
		// Funciones definidas por el usuario.
		case CODES_BLOCKS:
			v_generic=malloc(sizeof(struct Header_Stack));
			((struct Header_Stack*)v_generic)->stack=NULL;
			((struct Header_Stack*)v_generic)->father=hstc_stack;

			add_stack((struct Header_Stack*)v_generic,STRING,vr_data->value);
			run((struct Header_Stack*)v_generic,hstc_stack,hstc_var);
			delete_stack((struct Header_Stack*)v_generic);
			free(v_generic);
			break;
		default:
			copy_item(&tv_tmp,vr_data->type,vr_data->value);
			add_stack(arr_allData,tv_tmp->type,tv_tmp->value);
	}
}
char* printf_stack(const struct Header_Stack* hstc_io){
	// Ver si debo hacer una función to_value_string y usarla en vez de esto.
	char* s_out=(char*)malloc(sizeof(char)*2);// Iniciamos para despues agregarle.
	char* s_generic;//Uso general.
	*s_out='\0';
	char* s_format;
	unsigned int len=1;
	unsigned int len_str_return;
	mp_exp_t mp_exponent;
	struct Stack_* stc_now=hstc_io->stack;

	while (stc_now!=NULL){
		switch(stc_now->item.type){
			case INT:
				// Creamos una cadena temporal y guardamos el antero
				s_generic=(char*)alloca(50);
				sprintf(s_generic,"%d ",*(int*)stc_now->item.value);

				len=append_strcpy(&s_out,len,s_generic);
				break;
			case FLOAT:
				// Creamos una cadena temporal y guardamos el flotante.
				s_generic=(char*)alloca(60);
				sprintf(s_generic,"%f ",*(double*)stc_now->item.value);

				len=append_strcpy(&s_out,len,s_generic);
				break;
			case STACK:
				// Convertimos el array en cadena para despues concatenarlo
				s_generic=printf_stack((struct Header_Stack*)stc_now->item.value);
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
					(char*)stc_now->item.value
				);
				break;
			case STRING:
				len=append_sprintf(
					&s_out,
					len,
					3,// 3 -> '"" '
					"\"%s\" ",
					(char*)stc_now->item.value
				);
				break;
			case LONGINT:
				// Obtener la cadena.
				s_generic=mpz_get_str(NULL,0,*(mpz_t*)stc_now->item.value);
				
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
				s_generic=mpf_get_str(NULL, &mp_exponent, 10, 16, *(mpf_t*)stc_now->item.value);
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
					((struct Var*)stc_now->item.value)->type,
					((struct Var*)stc_now->item.value)->value
				);
				// 4 -> "(=) "
				len_str_return=strlen(((struct Var*)stc_now->item.value)->name)+strlen(s_generic)+4;
				s_out=(char*)realloc(s_out,SIZE_CHAR(len+len_str_return));
				sprintf(
					s_out+SIZE_CHAR(len-1),
					"(%s=%s) ",
					((struct Var*)stc_now->item.value)->name,
					s_generic
				);
				len+=len_str_return;
				free(s_generic);
				break;
			default:
				printf("Error: Tipo de dato \"%s\" no está tratado.",get_name_type(stc_now->item.type));
				break;
		}
		stc_now=stc_now->next;
	}
	return s_out;
}
struct Var* search_var(const char* s_name, struct Header_Stack* hstc_var){
	struct Stack_* stc_var=hstc_var->stack;
	unsigned int i_name=0;
	for (long int i=strlen(s_name)-1;i>-1;i--)
		i_name+=s_name[i];

	while (stc_var!=NULL){
		const struct Var* vr_varNow=(struct Var*)stc_var->item.value;//Variable actual.
		if (i_name==vr_varNow->i_name && !strcmp(&s_name[i_initStr],vr_varNow->name))//Es igual y retornamos.
			return vr_varNow;
		stc_var=stc_var->next;
	}
	return NULL;
}
void add_var(struct Header_Stack* hstc_out, const char* s_name, enum TYPE typ_data, void* v_data){
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
	add_stack(hstc_out,VAR,vr_now);
}
char* to_string_value(const enum TYPE typ_data, void* v_data){
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
		case STACK:
			s_out=printf_stack((struct Header_Stack*)v_data);
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
// @todo: stack_setItem
#endif