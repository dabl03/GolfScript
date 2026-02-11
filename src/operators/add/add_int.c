#ifndef ADD_INT_C
#define ADD_INT_C 1
	#include "../add.h"

	type_value_err* int_add_int(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		int64_t tmp_num;
		mpz_t* tmp_mpz;

		int a=*(int*)num_1->value,
			b=*(int*)num_2->value
		;
		out->type=INT;
		out->err=NORMAL;
		tmp_num=a+b;
		//Primero verificamos si hubo un desbordamiento.
		if (tmp_num<INT_MIN || tmp_num>INT_MAX){
			// Entonces convertir en LONG_INT gmp
			out->value=malloc(sizeof(mpz_t));
			out->type=LONGINT;
			mpz_init_set_si(*(mpz_t*)out->value, a);

			tmp_mpz=(mpz_t*)alloca(sizeof(mpz_t));
			mpz_init_set_si(*tmp_mpz, b);

			mpz_add(*(mpz_t*)out->value, *(mpz_t*)out->value,*tmp_mpz);
			mpz_clear(*tmp_mpz);
		}else{
			out->value=malloc(sizeof(int));
			*(int*)out->value=a+b;
		}
		return out;
	}
	type_value_err* int_add_long_int(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();

		out->type=LONGINT;
		out->err=NORMAL;
		// ADD
		out->value=malloc(sizeof(mpz_t));
		mpz_init_set_si(*(mpz_t*)out->value, *(int*)num_1->value);		
		mpz_add(
			*(mpz_t*)out->value,
			*(mpz_t*)out->value,
			*(mpz_t*)num_2
		);
		return out;
	}
	type_value_err* int_add_float(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		mpf_t tmp;

		out->type=FLOAT;
		out->err=NORMAL;
		out->value=malloc(sizeof(double));
		*(double*)out->value=(*(double*)num_1->value)+(*(double*)num_2->value);
		
		//Si se desborda el double
		if (isinf(*(double*)out->value)){
			free(out->value);
			//LONGFLOAT.
			out->type=LONGFLOAT;
			out->value=malloc(sizeof(mpf_t));

			mpf_init_set_d(*(mpf_t*)out->value, *(double*)num_2->value);
			mpf_init_set_d(tmp, *(double*)num_1->value);
					
			mpf_add(*(mpf_t*)out->value, *(mpf_t*)out->value, tmp);
			mpf_clear(tmp);
		}
		return out;
	}
	type_value_err* int_add_long_float(type_value* num_1, type_value* num_2, ...){
		type_value_err* out=NEW_TYPE_VALUE_ERR();
		mpf_t tmp;

		out->err=NORMAL;
		out->type=LONGFLOAT;
		out->value=malloc(sizeof(mpf_t));
		mpf_init_set(*(mpf_t*)out->value, *(mpf_t*)num_2->value);

		mpf_init_set_d(tmp, *(double*)num_1->value);

		mpf_add(*(mpf_t*)out->value,*(mpf_t*)out->value, tmp);
		mpf_clear(tmp);
		return out;
	}
#endif