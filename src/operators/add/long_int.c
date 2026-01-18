#ifndef ADD_LONG_INT_C
#define ADD_LONG_INT_C 1
	#include "../add.h"

	type_value_err* long_int_add_int(type_value* num_1, type_value* num_2, ...){
		return int_add_long_int(num_2,num_1);
	}
	type_value_err* long_int_add_long_int(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();

		out->type=LONGINT;
		out->err=NORMAL;
		
		out->value=malloc(sizeof(mpz_t));
		mpz_init(*(mpz_t*)out->value);

		mpz_add(
			*(mpz_t*)out->value,
			*(mpz_t*)num_1->value,
			*(mpz_t*)num_2->value
		);
		return out;
	}
	type_value_err* long_int_add_float(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		mpf_t tmp;
				
		out->err=NORMAL;
		out->type=LONGFLOAT;
		out->value=malloc(sizeof(mpf_t));
		mpf_init(*(mpf_t*)out->value);

		mpf_set_z(*(mpf_t*)out->value,*(mpz_t*)num_1->value);

		mpf_init(tmp);
		mpf_set_d(tmp, *(double*)num_2->value);

		mpf_add(*(mpf_t*)out->value, *(mpf_t*)out->value, tmp);
		mpf_clear(tmp);
		return out;
	}
	type_value_err* long_int_add_long_float(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		out->err=NORMAL;
		out->type=LONGFLOAT;
		out->value=malloc(sizeof(mpf_t));

		mpf_init(*(mpf_t*)out->value);
		mpf_set_z(*(mpf_t*)out->value, *(mpz_t*)num_2->value);

		mpf_add(*(mpf_t*)out->value, *(mpf_t*)out->value, *(mpf_t*)num_2->value);
	}
#endif