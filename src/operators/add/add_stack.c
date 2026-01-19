#ifndef ADD_STACK_C
	#define ADD_STACK_C 1
	#include "../add.h"
	
	type_value_err* basic_type_add_stack(type_value* num_1, type_value* tv_stack, bool is_right){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		unsigned int i;
		out->type=STACK;
		out->err=NORMAL;
		out->value=tv_stack->value;
		tv_stack->type=NONE;// None no libera memoria

		if (!is_right){
			if (add_stack((struct Header_Stack*) tv_stack->value, num_1->type, num_1->value)){
				out->err=INSUFFICIENT_MEMORY;
				return out;// num_1->type It must be none
			}
		}else{
			i=len_stack((struct Header_Stack*)tv_stack->value);

			// Se cuelga en mas de un elemento. Ver
			if (stack_setItem((struct Header_Stack*)tv_stack->value, num_1, i, true)){
				out->err=INDEX_NOT_FOUND;
			}
		}
		num_1->type=NONE;
		return out;
	}
	type_value_err* stack_add_basic_type(type_value* tv_stack, type_value* num_1, ...){
		return basic_type_add_stack(num_1, tv_stack, false);
	}
#endif