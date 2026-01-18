#ifndef ADD_H
#define ADD_H 1
	#include <string.h>
	#include <gmp.h>
	#include "../header/str.h"
	#include "../header/stack.h"
	#include "../header/define.h"
	#include "../header/operators.h"
	typedef struct type_value type_value;
	typedef struct type_value_err type_value_err;
	// Int
	type_value_err* int_add_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* int_add_long_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* int_add_float(type_value* num_1, type_value* num_2, ...);
	type_value_err* int_add_long_float(type_value* num_1, type_value* num_2, ...);
	// Long int
	type_value_err* long_int_add_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* long_int_add_long_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* long_int_add_float(type_value* num_1, type_value* num_2, ...);
	type_value_err* long_int_add_long_float(type_value* num_1, type_value* num_2, ...);
	// Float.c
	type_value_err* float_add_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* float_add_long_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* float_add_float(type_value* num_1, type_value* num_2, ...);
	type_value_err* float_add_long_float(type_value* num_1, type_value* num_2, ...);
	// Long float
	type_value_err* long_float_add_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* long_float_add_long_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* long_float_add_float(type_value* num_1, type_value* num_2, ...);
	type_value_err* long_float_add_long_float(type_value* num_1, type_value* num_2, ...);
	// String
	type_value_err* str_add_basic_type(type_value* num_1, type_value* num_2, bool is_right);
	type_value_err* basic_type_add_str(type_value* num_1, type_value* num_2, bool is_right);
	type_value_err* operator_str_add_str(type_value* num_1, type_value* num_2, bool is_right);
	type_value_err* str_add_codeblocks(type_value* num_1, type_value* num_2, bool is_right);
	type_value_err* str_add_stack(type_value* num_1, type_value* num_2, bool is_right);
	// Code blocks.
	type_value_err* codeblocks_add_str(type_value* num_1, type_value* num_2, bool is_right);
#endif