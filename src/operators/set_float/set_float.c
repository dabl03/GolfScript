#ifndef SET_FLOAT_C
#define SET_FLOAT_C 1
	#include "../set_float.h"
	#include <math.h>
	type_value_err* set_float_int(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		out->type=FLOAT;
		out->err=NORMAL;
		out->value=malloc(sizeof(double));

		*(double*)out->value=(double)(*(int*)num_2->value);
		return out;

	}
	type_value_err* set_float_long_int(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=(type_value_err*)malloc(sizeof(type_value_err));
		out->value=malloc(sizeof(mpf_t));
		out->type=LONGFLOAT;
		out->err=NORMAL;
		uint len=0;
		char* long_int;
		char* buffer;
		mpf_init(*(mpf_t*)out->value);
		long_int=long_int_to_string(num_2, &len);
		
		len+=3+strlen(SYSTEM_DECIMAL_POINT);// 1+'0'+'\0'
		buffer=(char*)malloc(sizeof(char)*len);
		
		sprintf(buffer,"%s%s0", long_int, SYSTEM_DECIMAL_POINT);
		mpf_set_str(*(mpf_t*)out->value, buffer, 10);
		free(buffer);
		FREE__(long_int);
		return out;
 	}
	type_value_err* set_float_int_and_int(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		const int int_decimal=(*(int*)num_2->value);
		double decimal=(double)(int_decimal>0)?int_decimal:~int_decimal;
		out->type=FLOAT;
		out->err=NORMAL;
		out->value=malloc(sizeof(double));

		*(double*)out->value=(double)(*(int*)num_1->value);
		for (int i=0;decimal>1; i++)
			decimal=decimal/10;
		*(double*)out->value+=decimal;
		return out;

	}
	type_value_err* set_float_int_and_long_int(type_value* num_1, type_value* num_2, bool is_right){
		type_value_err* out=(type_value_err*)malloc(sizeof(type_value_err));
		out->value=malloc(sizeof(mpf_t));
		out->type=LONGFLOAT;
		out->err=NORMAL;
		uint len=0;
		int num=*(int*)num_1->value;
		char* long_int;
		char* buffer;
		mpf_init(*(mpf_t*)out->value);
		
		long_int=long_int_to_string(num_2, &len);
		// log10 retorna la cantidad de digitos menos 1.
		len+=(int)log10( (double)((num>0)?num:~num) )+3+strlen(SYSTEM_DECIMAL_POINT);// 1+'-'+'\0'
		buffer=(char*)malloc(sizeof(char)*len);

		if (is_right){
			sprintf(buffer,"%d%s%s", *(int*)num_1->value, SYSTEM_DECIMAL_POINT, long_int);
		}else{
			sprintf(buffer,"%s%s%d", long_int, SYSTEM_DECIMAL_POINT, *(int*)num_1->value);
		}
		mpf_set_str(*(mpf_t*)out->value, buffer, 10);
		free(buffer);
		FREE__(long_int);
		return out;
 	}
	type_value_err* set_float_long_int_and_int(type_value* num_1, type_value* num_2, ...){
		return set_float_int_and_long_int(num_1, num_2, FALSE);
	}
	type_value_err* set_float_long_int_and_long_int(type_value* num_1, type_value* num_2, ...){}
	type_value_err* set_float_str_and_basic_type(type_value* num_1, type_value* num_2, ...){}// Se puede hacer un pequeno formato para obtener el o los decimales, o los decimales.>:)
	type_value_err* set_float_code_blocks(type_value* num_1, ...){}
	type_value_err* set_float_stack(type_value* num_1, ...){}
#endif