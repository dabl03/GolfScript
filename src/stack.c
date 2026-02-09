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
	// Importante.
	stc_new->item.type=typ_data;
	stc_new->item.value=value;
	stc_new->next=hstc_out->stack;
	stc_new->previous=NULL;// Ver si eliminar.
	
	if (hstc_out->stack!=NULL){
		hstc_out->stack->previous=stc_new;
	}
	hstc_out->stack=stc_new;
	return false;
}
struct type_value* pop_stack(struct Header_Stack* hstc_modific){
	if (hstc_modific->stack==NULL)
		return NULL;
	// Nota: No static porque apuntará a otro elemento al llamar dos veces.
	struct type_value* tv_out=(struct type_value*)malloc(sizeof(struct type_value*));
	struct Stack_* stc_temp=hstc_modific->stack;

	hstc_modific->stack=stc_temp->next;
	if (hstc_modific->stack!=NULL)
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
		case NONE:
			// Nunca liberar NONE. Se dejara para el uso del desarrollador.
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
	struct Stack_* stc_now=hstc_data->stack;
	struct Stack_* stc_for_free=NULL;
	while (stc_now!=NULL){
		delete_item(stc_now->item.type, stc_now->item.value);
		stc_for_free=stc_now;
		stc_now=stc_now->next;
		free(stc_for_free);
	}
	hstc_data->stack=NULL;
	hstc_data->father=NULL;
}
struct type_value* copy_item(struct type_value* tv_src, const enum TYPE typ_io, void* value_io){
	// Si pasa null creamos uno nuevo, de lo contrario usamos el que se paso.
	struct type_value* tv_out=(tv_src)?tv_src:malloc(sizeof(struct type_value));
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
					get_name_type(typ_io)
				);
			#endif
	}
	return tv_out;
}
struct Header_Stack* copy_stack(const struct Header_Stack* hstc_io){
	struct Header_Stack* hstc_out=(struct Header_Stack*)malloc(sizeof(struct Header_Stack));
	struct Stack_* stc_now=hstc_io->stack;
	struct type_value tv_tmp;
	hstc_out->stack=NULL;
	hstc_out->father=NULL;
	
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
void setValue_tv(struct Var* vr_now, const char* s_name, struct type_value* tv_setVar){
	// This Var is defined
	if (s_name==NULL){
		delete_item(vr_now->item.type,vr_now->item.value);
	}else{
		if (vr_now==NULL){
			printf("Error: Se ha pasado NULL a vr_now, cuando deberia tener "
				"un puntero a variable para modificar su valor.\n "
				"	En la funcion %s",__FUNCTION__
			);
			exit(-1);
		}
		unsigned int len=strlen(s_name),
			i=0
		;
		vr_now->name=(char*)malloc(sizeof(char*)*(len+1));
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
	struct type_value tmp;
	copy_item(&tmp, tv_setVar->type, tv_setVar->value);
	vr_now->item.type=tmp.type;
	vr_now->item.value=tmp.value;
}
void delete_var(struct Var* vr_var){
	delete_item(vr_var->item.type, vr_var->item.value);
	free(vr_var->name);
	vr_var->item.value=NULL;
}
unsigned int process_data(struct Header_Stack* hstc_stack, struct Header_Stack* hstc_var, struct Var* vr_data){
	struct type_value tv_tmp;
	void* v_generic;
	//U_INT err=0;// Solo para funciones. // Sin usar
	switch (vr_data->item.type){
		// Funciones nativas del lenguaje
		case FUNCTION:
			/*err=*/
			return ((U_INT (*)(
				struct Header_Stack*,
				struct Header_Stack*,
				char* extend
			))vr_data->item.value) (hstc_stack,hstc_var,"");
		// Funciones definidas por el usuario.
		case CODES_BLOCKS:
			v_generic=malloc(sizeof(struct Header_Stack));
			((struct Header_Stack*)v_generic)->stack=NULL;
			((struct Header_Stack*)v_generic)->father=hstc_stack;

			add_stack((struct Header_Stack*)v_generic, STRING, vr_data->item.value);
			run((struct Header_Stack*)v_generic,hstc_stack,hstc_var);
			delete_stack((struct Header_Stack*)v_generic);
			free(v_generic);
			break;
		default:
			copy_item(&tv_tmp, vr_data->item.type, vr_data->item.value);
			add_stack(hstc_stack,tv_tmp.type,tv_tmp.value);
	}
	return NORMAL;
}

struct Var* search_var(const char* s_name, struct Header_Stack* hstc_var){
	struct Stack_* stc_var=hstc_var->stack;
	unsigned int i_name=0;
	for (long int i=strlen(s_name)-1;i>-1;i--)
		i_name+=s_name[i];

	while (stc_var!=NULL){
		struct Var* vr_varNow=(struct Var*)stc_var->item.value;//Variable actual.
		if (i_name==vr_varNow->i_name && !strcmp(s_name,vr_varNow->name))//Es igual y retornamos.
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
bool stack_setItem(struct Header_Stack* h_stack, struct type_value* item, const unsigned int index, const bool is_append){
	struct Stack_* stc_chosen=NULL;
	struct Stack_* stc_new_item;
	unsigned int i=0;
	if (index==0){
		add_stack(h_stack,item->type, item->value);
		return false;
	}else if (!h_stack->stack)// There is no element to modify
		return true;
	else{
		stc_chosen=h_stack->stack;
		for (; i!=index; i++){
			stc_new_item=stc_chosen;
			if (!stc_chosen)// There is no index
				return true;
			stc_chosen=stc_chosen->next;
		}
		if (!stc_chosen){// The search was successful, but now it points to null
			stc_chosen=stc_new_item;
		}
	}
	if (is_append){
		stc_new_item=(struct Stack_*)malloc(sizeof(struct Stack_));
		stc_new_item->item.type=item->type;
		stc_new_item->item.value=item->value;
		// Stc_new_item
		stc_new_item->next=stc_chosen->next;
		stc_new_item->previous=stc_chosen;
		stc_chosen->next=stc_new_item;

		if (stc_new_item->next){
			// If there is only one element, nothing is done with the previous element.
			stc_new_item->next->previous=stc_new_item;
		}
	}else{
		delete_item(stc_chosen->item.type, stc_chosen->item.value);
		stc_chosen->item.type=item->type;
		stc_chosen->item.value=item->value;
	}
	return false;
}
unsigned int len_stack(const struct Header_Stack* h_stack){
	unsigned int out=0;

	for (struct Stack_* stc_now=h_stack->stack; stc_now!=NULL; stc_now=stc_now->next)
		out++;
	return out;
}
#endif