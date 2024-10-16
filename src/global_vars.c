#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./header/str.h"
#include "./header/stack.h"
#include "./header/global_vars.h"
#include "./operators/header/add.h"

unsigned short prinft_1_(struct Array* stack, struct Array* vars,char* extend){
	struct type_value* tv;
	//unsigned int len;//Para y out para array
	char* out;
	if (stack->i){
		tv=pop_array(stack);
		if (tv->type==STRING){//Escapamos la cadena si la hay.
			out=get_str_escp(tv->value);
		}else
			out=to_string_value(tv->type,tv->value);
		if (out==NULL){
			puts("ERROR: Interno, el tipo de dato desconocido.");
			delete_item(tv->type,tv->value);
			return 2;
		}
		printf("%s%s",out,extend);
		free(out);
		delete_item(tv->type,tv->value);
	}else{
		puts("Warnign: La pila esta vacia.");
		return 1;
	}
	return 0;
}

unsigned short puts_operator(struct Array* stack,struct Array* vars){
	int i_var=search_var("n",vars);
	struct Var* this_var=(struct Var*)vars->value[i_var].value;
	char* extend=to_string_value(this_var->type,this_var->value);
	i_var=prinft_1_(stack,NULL,(extend!=NULL)?extend:"");
	if (extend!=NULL)
		free(extend);
	return i_var;
}

unsigned short add_operator(struct Array* stack,...){
	if (stack->i<2){
		puts("Error: No hay suficientes elementos para hacer la suma");
		return 1;
	}
	struct type_value* num_2=pop_array(stack);//Dos se eliminará despues y uno quedará con los resultados.
	struct type_value* num_1=&stack->value[stack->i-1],
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
	case ARRAY:
		tmp_tv=opr_add_array((struct Array*)num_1->value,num_2->type,num_2->value);
		if (tmp_tv->type==NONE){
			//No fue necesario liberar nada pues la misma funcion lo hizo, y se reuso lo que se pudo.
			return 0;
		}
		break;
	default:
		perror("Caracteristica no disponible en la funcion add_operator\n");
		printf("Type num_2{%s}, type num_1{%s}",get_name_type(num_2->type),get_name_type(num_1->type));
		exit(-8);
	}
	delete_item(num_2->type,num_2->value);
	delete_item(num_1->type,num_1->value);
	stack->value[stack->i-1].type=tmp_tv->type;
	stack->value[stack->i-1].value=tmp_tv->value;
	return 0;
}

unsigned short sub_operator(struct Array* stack,...){
	perror("caracteristica no disponible.\nFuncion sub_operator sin terminar.");
	exit(-7);
	return 0;
}

unsigned short end_app(void){
	quit=1;
	return 0;
}

unsigned short reset(struct Array* stack,struct Array* vars,...){
	if (vars->i==0)
		return 1;
	delete_array(vars);
	init_gvars(vars);
	return 0;
}

unsigned short pack_stack(struct Array* stack,...){
	struct Array* tmp=copy_array(stack);
	delete_array(stack);
	add_array(stack,ARRAY,tmp);
	return 0;
}

unsigned short help(struct Array* stack,struct Array* vars,...){
	char* key_fun[]={
			"reset",
			"print",
			"puts",
			"quit",
			"]",
			"help",
			"fin_cadena"//Marca un final este array.
	};
	char* value_fun[]={
		"Reinicia las variables globales si se rescriben",
		"Imprime el ultimo elemento en la pila","Imprime el ultimo elemento en la pila y agrega un salto de linea",
		"Termina el programa",
		"Reempraza todos los elementos en la pila con un array con el contenido de esta",
		"Muestra este menu de ayuda - Agrega una cadena en la pila con un operador y te muestra su descripción."
	};
	int i_var=search_var("n",vars);
	struct Var* this_var=(struct Var*)vars->value[i_var].value;
	char* extend=to_string_value(this_var->type,this_var->value);

	printf("%s%s %s%s  Version: %s",LICENSE,extend,AUTHOR,extend,VERSION);
	for (U_INT i=0;strcmp(key_fun[i],"fin_cadena");i++){
		printf("%s%s -- %s%s",extend,key_fun[i],value_fun[i],extend);
	}
	if (extend!=NULL)
		free(extend);
	return 0;
}

void init_gvars(struct Array* vars){
	add_var(vars,"reset",FUNCTION,(void*)reset);
	add_var(vars,"print",FUNCTION,(void*)prinft_1_);
	add_var(vars,"puts",FUNCTION,(void*)puts_operator);
	add_var(vars,"+",FUNCTION,(void*)add_operator);
	add_var(vars,"-",FUNCTION,(void*)sub_operator);
	add_var(vars,"quit",FUNCTION,(void*)end_app);
	add_var(vars,"]",FUNCTION,(void*)pack_stack);
	add_var(vars,"n",STRING,"\n");
	add_var(vars,"help",FUNCTION,(void*)help);
}
#endif