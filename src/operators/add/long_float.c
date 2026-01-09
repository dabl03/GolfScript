#ifndef ADD_LONG_FLOAT_C
#define ADD_LONG_FLOAT_C 1
	#include <string.h>
	#include <gmp.h>
	//Agrego esto más para que el IDE sepa cuáles son los prototipos
	#include "../../header/str.h"
	#include "../../header/stack.h"
	#include "../../header/define.h"
	#include "../../header/operators.h"
	
	struct type_value_err* long_float_add_int(struct type_value* num_1, struct type_value* num_2, ...){
		return int_add_long_float(num_2, num_1);
	}
	struct type_value_err* long_float_add_long_int(struct type_value* num_1, struct type_value* num_2, ...){
		return long_int_add_long_float(num_2, num_1);
	}
	struct type_value_err* long_float_add_float(struct type_value* num_1, struct type_value* num_2, ...){
		return float_add_long_float(num_2, num_1);
	}
	struct type_value_err* long_float_add_long_float(struct type_value* num_1, struct type_value* num_2, ...){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		out->err=NORMAL;
		out->type=LONGFLOAT;

		out->value=malloc(sizeof(mpf_t));
		mpf_init(*(mpf_t*)out->value);
		mpf_add(*(mpf_t*)out->value, *(mpf_t*)num_1, *(mpf_t*)num_2);
		return out;
	}
#endif