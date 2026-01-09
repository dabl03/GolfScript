#ifndef OPERATORS_H
#define OPERATORS_H 1
	#include <string.h>
	#include <gmp.h>
	//Agrego esto más para que el IDE sepa cuáles son los prototipos
	#include "./str.h"
	#include "./stack.h"
	#include "./global_vars.h"
	#include "./define.h"
	
	#define NEW_TYPE_VALUE_ERR() (struct type_value_err*)malloc(sizeof(struct type_value_err*))
	/** It performs a springform and sorts according to 
	 * `is_right`. If `is_right` is true, it uses `ptr_format_1`;
	 * otherwise, it uses `ptr_format_2`.
	 * @Param ptr_out	Where to save the result.
	 * @Param ptr_format	Format that will be passed to sprintf.
	 * @Param is_right	Indicate whether to use the left or right variable (str_1 or str_2)
	 * @Param str_1, str_2	Strings to be added to ptr_out
	 * */
	void sprintf_with_invert(
		char* ptr_out,
		const char* ptr_format,
		bool is_right,
		const char* str_1, const char* str_2
	);
	/// type_value with error codes.
	struct type_value_err {
		enum TYPE type;
		void* value;
		unsigned long int err;
	};
	struct type_value_err * execute_sum(struct type_value* num_1, struct type_value* num_2);
	/**
	 * Inicializa el puntero a los operadores.
	 * Nota: Si se quiere usar execute_sum se debe llamar por 1 vez en el programa..
	 * @param bool Indica si se quiere reinicilizar los operadores.
	 */
	void init_operators(bool);
#endif