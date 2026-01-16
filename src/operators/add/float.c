#ifndef ADD_FLOAT_C
#define ADD_FLOAT_C 1
	#include <string.h>
	#include <gmp.h>
	//Agrego esto más para que el IDE sepa cuáles son los prototipos
	#include "../../header/str.h"
	#include "../../header/stack.h"
	#include "../../header/define.h"
	#include "../../header/operators.h"

	struct type_value_err* float_add_int(struct type_value* num_1, struct type_value* num_2, ...){
		return int_add_float(num_2, num_1);
	}
	struct type_value_err* float_add_long_int(struct type_value* num_1, struct type_value* num_2, ...){
		return int_add_long_float(num_2, num_1);
	}
	struct type_value_err* float_add_float(struct type_value* num_1, struct type_value* num_2, ...){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		double tmp=*(double*)num_1->value+*(double*)num_2->value;
		mpf_t num_b;
		out->err=NORMAL;
		out->type=FLOAT;
		if (isinf(tmp)){
			out->type=LONGFLOAT;
			out->value=malloc(sizeof(mpf_t));

			mpf_init (*(mpf_t*)out->value);
			mpf_set_d (*(mpf_t*)out->value, *(double*)num_1->value);
			mpf_init (num_b);
			mpf_set_d (num_b, *(double*)num_2->value);

			mpf_add(*(mpf_t*)out->value, *(mpf_t*)out->value, num_b);
			mpf_clear(num_b);
		}else{
			out->value=malloc(sizeof(double));
			*(double*)out->value=tmp;
		}
		return out->value;
	}
	struct type_value_err* float_add_long_float(struct type_value* num_1, struct type_value* num_2, ...){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		out->err=NORMAL;
		out->type=LONGFLOAT;
		out->value=malloc(sizeof(mpf_t));

		mpf_init (*(mpf_t*)out->value);
		mpf_set_d (*(mpf_t*)out->value, *(double*)num_1->value);
		mpf_add(*(mpf_t*)out->value, *(mpf_t*)out->value, num_2->value);
		return out;
	}
#endif