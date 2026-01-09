#ifndef ADD_STR_C
#define ADD_STR_C 1
	#include <string.h>
	#include <gmp.h>
	//Agrego esto más para que el IDE sepa cuáles son los prototipos
	#include "../../header/str.h"
	#include "../../header/stack.h"
	#include "../../header/define.h"
	#include "../../header/operators.h"

	struct type_value_err* str_add_basic_type(struct type_value* num_1, struct type_value* num_2, bool is_right){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		char* str=num_1->value,
			* basic_type=to_string_value(num_2->type, num_2->value)
		;

		out->type=STRING;
		out->err=NORMAL;

		out->value=malloc(strlen(str)+strlen(basic_type)+1);
		sprintf_with_invert(out->value, "%s%s", is_right, str, basic_type);
		// Para testear.
		#if defined(DEBUG) || defined(TEST_)
			if (num_2->type==LONGFLOAT || num_2->type==LONGINT)
				FREE__(basic_type);
			else
		#endif
			//else:
			free(basic_type);
		return out;
	}
	struct type_value_err* basic_type_add_str(struct type_value* num_1, struct type_value* num_2, bool is_right){
		return str_add_basic_type(num_2, num_1, false);
	}

#endif