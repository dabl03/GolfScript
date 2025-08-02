#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./header/str.h"
#include "./header/stack.h"
#include "./header/global_vars.h"
#include "./operators/header/add.h"

U_INT prinft_1_(struct Header_Stack* stack, struct Header_Stack* vars,char* extend){
	struct type_value* tv_now;
	char* out;
	if (stack->stack)
		return EMPTY_STACK;
	tv_now=pop_stack(stack);
	// Si es una cadena la escapamos.
	out=(tv_now->type==STRING)?
		get_str_escp(tv_now->value):
		to_string_value(tv_now->type,tv_now->value);
	if (out==NULL){
		delete_item(tv_now->type,tv_now->value);
		free(tv_now);
		return APP_UNKNOWN_DATA;
	}
	printf("%s%s",out,extend);
	delete_item(tv_now->type,tv_now->value);
	free(tv_now);
	free(out);
	return NORMAL;
}

U_INT puts_operator(struct Header_Stack* stack,struct Header_Stack* vars){
	struct Var vr_line_breack=search_var("n",vars);
	unsigned short out;
	char* extend=to_string_value(vr_line_breack->type,vr_line_breack->value);

	out=prinft_1_(stack,NULL,(extend!=NULL)?extend:"");
	if (extend!=NULL)
		free(extend);
	return out;
}

U_INT add_operator(struct Header_Stack* stack,...){
	if (stack->stack==NULL && (stack->stack->next==NULL && (stack->father==NULL && stack->father->next==NULL)) ){
		return INSUFFICIENT_ARGUMENTS;
	}
	// num_1=num_1+num_2
	struct type_value* num_2=pop_stack(stack);
	struct type_value* num_1;
	// Si no hay suficiente argumentos en esta pila
	// Agarramos de su padre.
	// [ 1 [  1 ]  ] -> [  [ 2 ]  ]
	if (stack->stack==NULL){
		struct type_value* this_stack=pop_stack(stack->father);
		num_1=pop_array(stack->father);
		add_stack(stack->father,STACK,this_stack->value);
		add_stack(stack,num_1->type,num_1->value);
		add_stack(stack,num_2->type,num_2->value);
		return add_operator(stack);
	}
	num_1=stack->stack->item,
	*tmp_tv=NULL;
	//[num_1 num_2]
	switch (num_1->type){
	case INT:
		tmp_tv=add_int(*(int*)num_1->value,num_2->type,num_2->value); 
		break;
	case LONGINT:
		tmp_tv=add_longint((mpz_t*)num_1->value,num_2->type,num_2->value);
		break;
	case CODES_BLOCKS:
		//Usamos alloca para no liberar este bloque:D
		tmp_tv=(struct type_value*)alloca(sizeof(struct type_value*));
		tmp_tv->type=CODES_BLOCKS;
		tmp_tv->value=add_codes_block(num_1->value,num_2->type,num_2->value);
		break;
	case STRING:
		tmp_tv=add_str((char*)num_1->value,num_2->type,num_2->value,true);
		break;
	case STACK:
		tmp_tv=opr_add_serror_codeack((struct Header_Stack*)num_1->value,num_2->type,num_2->value);
		if (tmp_tv->type==NONE){
			//No fue necesario liberar nada pues la misma funcion lo hizo, y se reuso lo que se pudo.
			return 0;
		}
		break;
	default:
		perror("Caracteristica no disponible en la funcion add_operator\n");
		printf("Type num_2{%s}, type num_1{%s}",get_name_type(num_2->type),get_name_type(num_1->type));
		exit(APP_UNKNOWN_DATA);
	}
	delete_item(num_2->type,num_2->value);
	delete_item(num_1->type,num_1->value);
	if (out->err==NORMAL){
		stack->stack->item.type=tmp_tv->type;
		stack->stack->item.value=tmp_tv->value;
	}else{
		delete_item(tmp_tv->type,tmp_tv->value);
	}
	return out->err;
}

U_INT sub_operator(struct Header_Stack* stack,...){
	return FEATURE_NOT_AVAILABLE;
}

U_INT end_app(void){
	quit=1;
	return error_code;
}

U_INT reset(struct Header_Stack* stack,struct Header_Stack* vars,...){
	delete_stack(vars);
	return init_gvars(vars);
}

U_INT pack_stack(struct Header_Stack* stack,...){
	struct Header_Stack* tmp=copy_stack(stack);
	delete_stack(stack);
	add_STACK(stack,STACK,tmp);
	return NORMAL;
}

U_INT help(struct Header_Stack* stack,struct Header_Stack* vars,...){
	char* key_fun[]={
			"reset",
			"print",
			"puts",
			"quit",
			"]",
			"help"
	};
	char* value_fun[]={
		"Reinicia las variables globales si se rescriben",
		"Imprime el ultimo elemento en la pila","Imprime el ultimo elemento en la pila y agrega un salto de linea",
		"Termina el programa",
		"Reempraza todos los elementos en la pila con un array con el contenido de esta",
		"Muestra este menu de ayuda - Agrega una cadena en la pila con un operador y te muestra su descripción."
	};
	struct Var* vr_breack_line=search_var("n",vars);
	char* extend=to_string_value(vr_breack_line->type,vr_breack_line->value);

	printf("%s%s %s%s  Version: %s",LICENSE,extend,AUTHOR,extend,VERSION);
	for (U_INT i=0;i<sizeof(key_fun);i++){
		printf("%s%s -- %s%s",extend,key_fun[i],value_fun[i],extend);
	}
	if (extend!=NULL)
		free(extend);
	return NORMAL;
}

U_INT init_gvars(struct Header_Stack* vars){
	add_var(vars,"reset",FUNCTION,(void*)reset);
	add_var(vars,"print",FUNCTION,(void*)prinft_1_);
	add_var(vars,"puts",FUNCTION,(void*)puts_operator);
	add_var(vars,"+",FUNCTION,(void*)add_operator);
	add_var(vars,"-",FUNCTION,(void*)sub_operator);
	add_var(vars,"quit",FUNCTION,(void*)end_app);
	add_var(vars,"]",FUNCTION,(void*)pack_stack);
	add_var(vars,"n",STRING,"\n");
	add_var(vars,"help",FUNCTION,(void*)help);
	return NORMAL;
}
#endif