#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./header/str.h"
#include "./header/stack.h"
#include "./header/global_vars.h"
#include "./header/operators.h"

U_INT prinft_1_(struct Header_Stack* stack, struct Header_Stack* vars,char* extend){
	struct type_value* tv_now;
	char* out;
	if (stack->stack==NULL)
		return EMPTY_STACK;
	tv_now=pop_stack(stack);
	// Si es una cadena la escapamos.
	out=(tv_now->type==STRING)?
		(char*)tv_now->value:
		tv_to_string(tv_now, NULL);
	if (out==NULL){
		delete_item(tv_now->type,tv_now->value);
		free(tv_now);
		return APP_UNKNOWN_DATA;
	}
	printf("%s%s",out,extend);
	if (tv_now->type!=STRING)
		free(out);// Ya se ha liberado.
	delete_item(tv_now->type, tv_now->value);

	free(tv_now);
	return NORMAL;
}

U_INT puts_operator(struct Header_Stack* stack,struct Header_Stack* vars){
	struct Var* vr_line_breack=search_var("n",vars);
	unsigned short out;
	char* extend=to_string(
		vr_line_breack->item.type,
		vr_line_breack->item.value,
		NULL
	);

	out=prinft_1_(stack,NULL,(extend!=NULL)?extend:"");
	if (extend!=NULL)
		free(extend);
	return out;
}

struct type_value** opt_get_param(struct Header_Stack* h_stack, unsigned int* codes){
	static struct type_value* out[2];
	// io -> [ num_1 num_2 ]
	// out -> [0]=num_1, [1]->num_2
	// Si no hay suficiente argumentos en la sub pila.
	// [ num_1 [ num_2 ]  ] o [ num_1 num_2 [ ] ]
	// [] -> *codes=INSUFFICIENT_ARGUMENTS
	if (h_stack->stack==NULL){
		if (h_stack->father==NULL && h_stack->father->stack==NULL){
			*codes=INSUFFICIENT_ARGUMENTS;
			return NULL;
		}
		out[1]=&h_stack->father->stack->item;
		out[0]=&h_stack->father->stack->next->item;
	}else{
		out[1]=&h_stack->stack->item;
		if (h_stack->stack->next!=NULL)
			out[0]=&h_stack->stack->next->item;
		else if (h_stack->father==NULL && h_stack->father->stack==NULL){
			*codes=INSUFFICIENT_ARGUMENTS;
			return NULL;
		}else
			out[0]=&h_stack->father->stack->item;
	}
	return out;
}

// Crear una funcion que ordene de la siguiente manera,
U_INT add_operator(struct Header_Stack* h_stack, ...){
	unsigned int codes=NORMAL;
	struct type_value** tv_param=opt_get_param(h_stack,&codes);
	if (codes!=NORMAL){
		return codes;
	}
	struct type_value_err * result=opt_execute(tv_param[0], tv_param[1], OPT_ADD);
	struct type_value* for_delete;
	if (result->err==NORMAL){
		delete_item(tv_param[0]->type, tv_param[0]->value);
		tv_param[0]->type=result->type;
		tv_param[0]->value=result->value;
		free(result);
		// Eliminamos el elemento sobrante.
		for_delete=pop_stack(h_stack);
		if (result==NULL){
			for_delete=pop_stack(h_stack->father);
		}
		delete_item(tv_param[1]->type, tv_param[1]->value);
	}
	free(for_delete);
	return result->err;
}
uint opt_setFloat(struct Header_Stack* h_stack, ...){
	unsigned int codes=NORMAL;
	struct type_value** tv_param=opt_get_param(h_stack,&codes);
	struct type_value_err * result;
	if (codes!=NORMAL){
		return codes;
	}
	char num=(tv_param[0]->type<tv_param[1]->type);

	if (tv_param[1]->type==FLOAT || tv_param[1]->type==LONGFLOAT){
		return CONVERTION_NOT_FOUND;
	}else if (tv_param[num]->type<FLOAT){ // INT, LONGINT
		result=opt_execute(tv_param[0], tv_param[1], OPT_SET_FLOAT);
	}else{// Stack, CodeBlocks, String
		// @TODO: Verificar si hay suficientes elementos en lo de arriba para ejecutar opt_setFloat.
	}
	if (result->err==NORMAL){

	}else{
		codes=result->err;
		free(result);
	}
	return codes;
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

U_INT pack_stack(struct Header_Stack* h_stack,...){
	if (h_stack->stack==NULL)
		return EMPTY_STACK;
	struct Header_Stack* tmp=(struct Header_Stack*)malloc(
		sizeof(struct Header_Stack)
	);
	tmp->stack=h_stack->stack;
	tmp->father=h_stack;
	h_stack->stack=NULL;
	add_stack(h_stack, STACK, tmp);
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
	char* extend=to_string(vr_breack_line->item.type, vr_breack_line->item.value, NULL);

	printf("%s%s %s%s  Version: %s",LICENSE,extend,AUTHOR,extend,VERSION);
	for (U_INT i=0;i<sizeof(key_fun);i++){
		printf("%s%s -- %s%s",extend,key_fun[i],value_fun[i],extend);
	}
	if (extend!=NULL)
		free(extend);
	return NORMAL;
}

U_INT init_gvars(struct Header_Stack* vars){
	add_var(vars,"reset",FUNCTION, (void*)reset        );
	add_var(vars,"print",FUNCTION, (void*)prinft_1_    );
	add_var(vars,"puts", FUNCTION, (void*)puts_operator);
	add_var(vars,"+",    FUNCTION, (void*)add_operator );
	add_var(vars,"-",    FUNCTION, (void*)sub_operator );
	add_var(vars,"quit", FUNCTION, (void*)end_app      );
	add_var(vars,"]",    FUNCTION, (void*)pack_stack   );
	add_var(vars,"help", FUNCTION, (void*)help         );
	add_var(vars,"n",    STRING,   (void*)copy_str("\n"));
	add_var(vars,"float",FUNCTION, (void*)opt_setFloat );
	add_var(vars,"prompt",STRING,  (void*)copy_str("?> "));
	add_var(vars,"sub_prompt",STRING,(void*)copy_str("... "));
	return NORMAL;
}
#endif