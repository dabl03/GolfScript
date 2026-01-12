#ifndef ADD_CODES_BLOCKS_C
#define ADD_CODES_BLOCKS_C 1
	#include <string.h>
	#include <gmp.h>
	#include "../../header/str.h"
	#include "../../header/stack.h"
	#include "../../header/define.h"
	#include "../../header/operators.h"

	struct type_value_err* codeblocks_add_str(struct type_value* num_1, struct type_value* num_2, bool is_right){
		return str_add_codeblocks(num_1, num_2, FALSE);
	}
#endif