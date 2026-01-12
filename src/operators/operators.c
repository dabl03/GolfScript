#ifndef RUN_C
#define RUN_C 1
	#include "../header/str.h"
	#include "../header/stack.h"
	#include "../header/global_vars.h"
	#include "../header/define.h"
	// ADD
	// Note: Maintain the order of headings.
	#include "./add/int.c"
	#include "./add/long_int.c"
	#include "./add/float.c"
	#include "./add/long_float.c"
	#include "./add/str.c"
	
	void sprintf_with_invert(
		char* ptr_out,
		const char* ptr_format,
		bool is_right,
		const char* str_1, const char* str_2
	){
		if (is_right)
			sprintf(ptr_out, ptr_format, str_1, str_2);
		else
			sprintf(ptr_out, ptr_format, str_2, str_1);
	}
	typedef struct type_value_err * (*ADD_FUNC)(struct type_value *, struct type_value *, bool);
	ADD_FUNC ADD_PTR[END_ELEMENT][END_ELEMENT];
	// Hacer un array de dos dimenciones que haga arr[INT][FLOAT] y retorne la funcion correspondiente del archivo add.h
	// Hacer una funcion que obtenga esa funcion del array que debe ser estatico.
	// Hacer pequenas funciones que sumen con orden (tenga un orden para poder reusarla.)
	struct type_value_err * execute_sum(struct type_value* num_1, struct type_value* num_2){
		if (ADD_PTR[num_1->type][num_2->type]==NULL){
			printf("Error: No se ha inicialiado los punteros necesarios para los operadores.\n"
				"Porfavor llamar una vez init_operators();.\n");
			exit(-1);
		}
		return ADD_PTR[num_1->type][num_2->type](num_1,num_2,true);
	}
	void init_add(){
		// INT
		ADD_PTR[INT][INT]=&int_add_int;
		ADD_PTR[INT][LONGINT]=&int_add_long_int;
		ADD_PTR[INT][FLOAT]=&int_add_float;
		ADD_PTR[INT][LONGFLOAT]=&int_add_long_float;

		// LONGINT
		ADD_PTR[LONGINT][INT]=&long_int_add_int;
		ADD_PTR[LONGINT][LONGINT]=&long_int_add_long_int;
		ADD_PTR[LONGINT][FLOAT]=&long_int_add_float;
		ADD_PTR[LONGINT][LONGFLOAT]=&long_int_add_long_float;

		// FLOAT
		ADD_PTR[FLOAT][INT]=&float_add_int;
		ADD_PTR[FLOAT][LONGINT]=&float_add_long_int;
		ADD_PTR[FLOAT][FLOAT]=&float_add_float;
		ADD_PTR[FLOAT][LONGFLOAT]=&float_add_long_float;
		
		// LONGFLOAT
		ADD_PTR[LONGFLOAT][INT]=&long_float_add_int;
		ADD_PTR[LONGFLOAT][LONGINT]=&long_float_add_long_int;
		ADD_PTR[LONGFLOAT][FLOAT]=&long_float_add_float;
		ADD_PTR[LONGFLOAT][LONGFLOAT]=&long_float_add_long_float;
		
		// Basic type add string
		ADD_PTR[INT][STRING]=&basic_type_add_str;
		ADD_PTR[LONGINT][STRING]=&basic_type_add_str;
		ADD_PTR[FLOAT][STRING]=&basic_type_add_str;
		ADD_PTR[LONGFLOAT][STRING]=&basic_type_add_str;
		
		// STRING add basic type
		ADD_PTR[STRING][INT]=&str_add_basic_type;
		ADD_PTR[STRING][LONGINT]=&str_add_basic_type;
		ADD_PTR[STRING][FLOAT]=&str_add_basic_type;
		ADD_PTR[STRING][LONGFLOAT]=&str_add_basic_type;

		// STRING add
		ADD_PTR[STRING][STRING]=&operator_str_add_str;
		ADD_PTR[STRING][CODES_BLOCKS]=&str_add_codeblocks;
		ADD_PTR[STRING][STACK]=&str_add_stack;
		
		// Basic type add codeblock.
		
		// CodeBlock
		ADD_PTR[CODES_BLOCKS][STRING]=&str_add_codeblocks;
		
		// Basic type add array
		
		/*
			INT, // Entero de 32bits
			LONGINT, // Entero de 64bits
			FLOAT, // double
			LONGFLOAT, // Big float of gmp

			STRING, // string
			PCHAR,// Elominar y cambiar por None
			CHAR, // char* // Eliminar.
			VALUE_TYPE,

			FUNCTION, // Función estandar del programa. Nota: No debe ser liberado.
			VAR, // Variable o estructura Var
			CODES_BLOCKS, // string que representa un código golfscript
			STACK,
			ARRAY, // Compatibilidd
	*/
	}
	void init_operators(bool reset){
		// Por si se quiere reiniciar los punteros a los operadores.
		if (!reset && ADD_PTR[INT][INT]!=NULL)
			return;
		init_add();
	}
#endif