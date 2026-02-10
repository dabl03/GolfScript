#ifndef SET_FLOAT_H
#define SET_FLOAT_H 1	
	#include <string.h>
	#include <gmp.h>
	#include <math.h>// log10
	#include "../header/str.h"
	#include "../header/stack.h"
	#include "../header/define.h"
	#include "../header/operators.h"

	typedef struct type_value type_value;
	typedef struct type_value_err type_value_err;

	/**
	 * Todas las funciones de aqui convierte uno o dos
	 * tipos de datos en float o gmp float.
	 * En el nombre se puede observar cual tipo de dato acepta la funciones:
	 * set_float_{dato1}_and_{dato2}.
	 * Las funciones pueden tener los siguientes parametros:
	 * @Param num_1, num_2 Los datos necesarios para la conversion.
	 * @Param is_right Para operaciones donde el orden es importante,
	 * 	O simplemente para poder reusar una existente.
	 * @Return type_value_err* El resultado con indicador de error.
	*/
	type_value_err* set_float_int(type_value* num_1, type_value* num_2, ...);
	type_value_err* set_float_long_int(type_value* num_1, type_value* num_2, ...);

	type_value_err* set_float_int_and_int(type_value* num_1, type_value* num_2, ...);

	type_value_err* set_float_int_and_long_int(type_value* num_1, type_value* num_2, bool is_right);
	type_value_err* set_float_long_int_and_int(type_value* num_1, type_value* num_2, ...);
	
	type_value_err* set_float_long_int_and_long_int(type_value* num_1, type_value* num_2, ...);
	
	type_value_err* set_float_str_and_basic_type(type_value* num_1, type_value* num_2, ...);
	type_value_err* set_float_code_blocks(type_value* num_1, ...);///@todo: Ver si eliminar.
	type_value_err* set_float_stack(type_value* num_1, ...);
#endif