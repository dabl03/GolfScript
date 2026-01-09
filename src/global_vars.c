#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./header/str.h"
#include "./header/stack.h"
#include "./header/global_vars.h"
#include "./operators/header/add.h"
#include "./header/operators.h"

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
	struct Var* vr_line_breack=search_var("n",vars);
	unsigned short out;
	char* extend=to_string_value(vr_line_breack->type,vr_line_breack->value);

	out=prinft_1_(stack,NULL,(extend!=NULL)?extend:"");
	if (extend!=NULL)
		free(extend);
	return out;
}

// Crear una funcion que ordene de la siguiente manera,
U_INT add_operator(struct Header_Stack* h_stack,...){
	if (h_stack->stack==NULL && (h_stack->stack->next==NULL && (h_stack->father==NULL && h_stack->father->stack->next==NULL)) ){
		return INSUFFICIENT_ARGUMENTS;
	}
	// num_1=num_1+num_2
	struct type_value* num_2=pop_stack(h_stack);
	struct type_value* num_1;
	struct type_value_err* tmp_tv;
	// Si no hay suficiente argumentos en esta pila
	// Agarramos de su padre.
	// [ 1 [ 1 ]  ] -> [ [ 2 ]  ]
	if (h_stack->stack==NULL){
		struct type_value* this_stack=pop_stack(h_stack->father);
		num_1=pop_stack(h_stack->father);
		add_stack(h_stack->father,STACK,this_stack->value);
		add_stack(h_stack,num_1->type,num_1->value);
		add_stack(h_stack,num_2->type,num_2->value);
		return add_operator(h_stack);
	}
	num_1=&h_stack->stack->item;
	tmp_tv=execute_sum(num_1, num_2);
	delete_item(num_1->type,num_1->value);
	delete_item(num_2->type,num_2->value);
	free(num_2);
	if (tmp_tv->err==NORMAL){
		h_stack->stack->item.type=tmp_tv->type;
		h_stack->stack->item.value=tmp_tv->value;
		free(tmp_tv);
	}else{
		delete_item(tmp_tv->type,tmp_tv->value);
		pop_stack(h_stack);
		free(num_1);
	}
	return tmp_tv->err;
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
	add_stack(stack,STACK,tmp);
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
	init_operators(false);
	return NORMAL;
}
#endif