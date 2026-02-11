#ifndef ADD_CODES_BLOCKS_C
#define ADD_CODES_BLOCKS_C 1
	#include "../add.h"

	type_value_err* codeblocks_add_str(type_value* num_1, type_value* num_2, bool is_right){
		return str_add_codeblocks(num_1, num_2, FALSE);
	}
#endif