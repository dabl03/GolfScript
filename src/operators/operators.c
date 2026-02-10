#ifndef RUN_C
#define RUN_C 1
	#include "../header/str.h"
	#include "../header/stack.h"
	#include "../header/define.h"
	// ADD
	#include "./add.h"
	#include "./set_float.h"
	
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
	type_value_err * (*OPT_FUNC[OPT_END_ELEMENT][END_ELEMENT][END_ELEMENT])(struct type_value *, struct type_value *, bool);
	static void __attribute__((constructor)) init_globals() {
		for (uint x=0; x<OPT_END_ELEMENT; x++)
			for (uint y=0; y<END_ELEMENT; y++)
				for (uint z=0; z<END_ELEMENT; z++)
					OPT_FUNC[x][y][z]=(type_value_err*(*)(struct type_value *, struct type_value *, bool))NULL;// In case any one is missing, it will give an error when accessing the missing one.
	}
	struct type_value_err * opt_execute(
		struct type_value* num_1,
		struct type_value* num_2,
		enum OPERATORS opt
	){
		if (OPT_FUNC[0][0][0]==NULL)
			init_operators(false);
		if (OPT_FUNC[opt][num_1->type][num_2->type]==NULL){
			struct type_value_err * err_out=NEW_TYPE_VALUE_ERR();
			err_out->type=NONE;
			err_out->err=FEATURE_NOT_AVAILABLE;
			return err_out;
		}
		return OPT_FUNC[opt][num_1->type][num_2->type](num_1,num_2,true);
	}
	void init_add(){
		// INT
		OPT_FUNC[0][0][0]                =int_add_int;
		OPT_FUNC[OPT_ADD][INT][INT]      =int_add_int;
		OPT_FUNC[OPT_ADD][INT][LONGINT]  =int_add_long_int;
		OPT_FUNC[OPT_ADD][INT][FLOAT]    =int_add_float;
		OPT_FUNC[OPT_ADD][INT][LONGFLOAT]=int_add_long_float;

		// LONGINT
		OPT_FUNC[OPT_ADD][LONGINT][INT]      =long_int_add_int;
		OPT_FUNC[OPT_ADD][LONGINT][LONGINT]  =long_int_add_long_int;
		OPT_FUNC[OPT_ADD][LONGINT][FLOAT]    =long_int_add_float;
		OPT_FUNC[OPT_ADD][LONGINT][LONGFLOAT]=long_int_add_long_float;

		// FLOAT
		OPT_FUNC[OPT_ADD][FLOAT][INT]      =float_add_int;
		OPT_FUNC[OPT_ADD][FLOAT][LONGINT]  =float_add_long_int;
		OPT_FUNC[OPT_ADD][FLOAT][FLOAT]    =float_add_float;
		OPT_FUNC[OPT_ADD][FLOAT][LONGFLOAT]=float_add_long_float;
		
		// LONGFLOAT
		OPT_FUNC[OPT_ADD][LONGFLOAT][INT]      =long_float_add_int;
		OPT_FUNC[OPT_ADD][LONGFLOAT][LONGINT]  =long_float_add_long_int;
		OPT_FUNC[OPT_ADD][LONGFLOAT][FLOAT]    =long_float_add_float;
		OPT_FUNC[OPT_ADD][LONGFLOAT][LONGFLOAT]=long_float_add_long_float;
		
		// Basic type add string
		OPT_FUNC[OPT_ADD][INT][STRING]      =basic_type_add_str;
		OPT_FUNC[OPT_ADD][LONGINT][STRING]  =basic_type_add_str;
		OPT_FUNC[OPT_ADD][FLOAT][STRING]    =basic_type_add_str;
		OPT_FUNC[OPT_ADD][LONGFLOAT][STRING]=basic_type_add_str;
		
		// STRING add basic type
		OPT_FUNC[OPT_ADD][STRING][INT]      =str_add_basic_type;
		OPT_FUNC[OPT_ADD][STRING][LONGINT]  =str_add_basic_type;
		OPT_FUNC[OPT_ADD][STRING][FLOAT]    =str_add_basic_type;
		OPT_FUNC[OPT_ADD][STRING][LONGFLOAT]=str_add_basic_type;

		// STRING add
		OPT_FUNC[OPT_ADD][STRING][STRING]      =operator_str_add_str;
		OPT_FUNC[OPT_ADD][STRING][CODES_BLOCKS]=str_add_codeblocks;
		OPT_FUNC[OPT_ADD][STRING][STACK]       =str_add_stack;
		
		// Basic type add codeblock.
		
		// CodeBlock
		OPT_FUNC[OPT_ADD][CODES_BLOCKS][STRING]=str_add_codeblocks;
		
		// Basic type add array
		OPT_FUNC[OPT_ADD][INT][STACK]      =basic_type_add_stack;
		OPT_FUNC[OPT_ADD][LONGINT][STACK]  =basic_type_add_stack;
		OPT_FUNC[OPT_ADD][FLOAT][STACK]    =basic_type_add_stack;
		OPT_FUNC[OPT_ADD][LONGFLOAT][STACK]=basic_type_add_stack;
		
		// STRING add basic type
		OPT_FUNC[OPT_ADD][STACK][INT]      =stack_add_basic_type;
		OPT_FUNC[OPT_ADD][STACK][LONGINT]  =stack_add_basic_type;
		OPT_FUNC[OPT_ADD][STACK][FLOAT]    =stack_add_basic_type;
		OPT_FUNC[OPT_ADD][STACK][LONGFLOAT]=stack_add_basic_type;
	}
	void init_set_float(){
		OPT_FUNC[OPT_SET_FLOAT][NONE][INT]   =set_float_int;
		OPT_FUNC[OPT_SET_FLOAT][NONE][LONGINT]=set_float_long_int;
		
		OPT_FUNC[OPT_SET_FLOAT][INT][INT]    =set_float_int_and_int;
		OPT_FUNC[OPT_SET_FLOAT][INT][LONGINT]=set_float_int_and_long_int;
		OPT_FUNC[OPT_SET_FLOAT][LONGINT][INT]=set_float_long_int_and_int;
		OPT_FUNC[OPT_SET_FLOAT][LONGINT][LONGINT]=set_float_long_int_and_long_int;

		OPT_FUNC[OPT_SET_FLOAT][INT][STRING]   =set_float_str_and_basic_type;
		OPT_FUNC[OPT_SET_FLOAT][LONGINT][STRING]=set_float_str_and_basic_type;
		OPT_FUNC[OPT_SET_FLOAT][FLOAT][STRING] =set_float_str_and_basic_type;
		OPT_FUNC[OPT_SET_FLOAT][LONGFLOAT][STRING]=set_float_str_and_basic_type;

		OPT_FUNC[OPT_SET_FLOAT][NONE][STACK]=str_add_stack;
	}
	void init_operators(bool reset){
		// Por si se quiere reiniciar los punteros a los operadores.
		init_add();
		printf("Init set float");
		init_set_float();
	}
#endif