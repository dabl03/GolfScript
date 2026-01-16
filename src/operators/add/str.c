#ifndef ADD_STR_C
#define ADD_STR_C 1
	#include <string.h>
	#include <gmp.h>
	#include "../../header/str.h"
	#include "../../header/stack.h"
	#include "../../header/define.h"
	#include "../../header/operators.h"

	struct type_value_err* str_add_basic_type(struct type_value* num_1, struct type_value* num_2, bool is_right){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		char* str=(char*)num_1->value,
			* basic_type=to_string_value(num_2->type, num_2->value)
		;

		out->type=STRING;
		out->err=NORMAL;

		out->value=malloc(strlen(str)+strlen(basic_type)+1);
		sprintf_with_invert(out->value, "%s%s", is_right, str, basic_type);
		// Para testear.
		#if defined(DEBUG) || defined(TEST_)
			if (num_2->type==LONGFLOAT || num_2->type==LONGINT)
				FREE__(basic_type);
			else
		#endif
			//else:
			free(basic_type);
		return out;
	}
	struct type_value_err* basic_type_add_str(struct type_value* num_1, struct type_value* num_2, bool is_right){
		return str_add_basic_type(num_2, num_1, false);
	}
	struct type_value_err* operator_str_add_str(struct type_value* num_1, struct type_value* num_2, bool is_right){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		char* str_1=(char*)num_1->value,
			* str_2=(char*)num_2->value
		;
		unsigned int len_str_1=strlen(str_1);
		out->type=STRING;
		out->err=NORMAL;

		out->value=malloc(len_str_1+strlen(str_2)+1);
		strcpy(out->value, str_1);
		strcpy(out->value+len_str_1, str_2);
		return out;
	}
	struct type_value_err* str_add_codeblocks(struct type_value* num_1, struct type_value* num_2, bool is_right){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		out->type=CODES_BLOCKS;
		out->err=NORMAL;
		out->value=(char*)malloc(
			strlen((char*)num_1->value)+strlen((char*)num_1->value)+4
		);
		sprintf_with_invert(
			out->value,
			// String 4 or CodeBlock 2.
			"%s %s",
			is_right,
			(char*)num_1->value, (char*)num_2->value
		);
		return out;
	}
	struct type_value_err* str_add_stack(struct type_value* num_1, struct type_value* num_2, bool is_right){
		struct type_value_err* out=NEW_TYPE_VALUE_ERR();
		struct type_value_err* tv_tmp=NEW_TYPE_VALUE_ERR();

		struct Stack_* stc_now=((struct Header_Stack*)num_2)->stack;
		struct type_value* now;
		char* str_tmp;
		unsigned int len_str_out=1;
		unsigned int int_tmp=0;

		out->type=STRING;
		out->err=NORMAL;
		out->value=malloc(1);
		*(char*)out->value='\0';
		//Ponemos todo el arraincludey en una cadena.
		while (stc_now){
			now=&((struct Stack_*)stc_now)->item;
			switch(now->type){
				case INT:
				// I don't know how to add the decimal
				// So it's treated as a whole number.
				case FLOAT:
					/* In GolfScript, numbers inside
					the stack are converted to characters
					The conversion is not done*/
					if (*(int*)now->value>-1 && *(int*)now->value<=127){
						out->value=realloc(out->value, len_str_out+1);
						((char*)out->value)[len_str_out-1]=*(int*)now->value;
						((char*)out->value)[len_str_out]='\0';
						len_str_out++;
					}else{
						// It exceeds the capacity limit of a character.
						// So we convert it to a string.
						
						// Max int 2 147 483 647 -> 10 digit+2 just in case
						str_tmp=(char*)malloc(12);
						sprintf(str_tmp,"%d",*(int*)now->value);
						int_tmp=strlen(str_tmp);

						out->value=realloc(out->value, len_str_out+int_tmp);
						sprintf(out->value+(len_str_out-1), "%s", str_tmp);
						free(str_tmp);
						len_str_out+=int_tmp;
					}
				break;
				case STACK:
					tv_tmp=str_add_stack(
						(struct type_value*)out,
						(struct type_value*)now,
						TRUE
					);
					free(out->value);

					out->value=tv_tmp;
					out->err+=tv_tmp->err;
					free(tv_tmp);
					len_str_out=strlen(out->value);// @todo: Ver si debe ser +1
					break; 
				default:
					str_tmp=to_string_value(now->type, now->value);
					if (!str_tmp)// is it null?
						continue;
					int_tmp=strlen(str_tmp);

					out->value=realloc(out->value, len_str_out+int_tmp);
					sprintf(out->value+(len_str_out-1), "%s", str_tmp);
					free(str_tmp);

					// For testing and debugging
					#ifdef DEBUG
						if (
							now->type==LONGFLOAT || 
							now->type==LONGINT
						)
							// The gmplib API (str_tmp=string) allocates
							// memory without using the malloc macro
							// during debugging.
							// So we release directly
							// to avoid generating false
							// warnings.
							FREE__(str_tmp);
						else
					#endif
							free(str_tmp);
					len_str_out+=int_tmp;
			}
			stc_now=stc_now->next;
		}
		if (!len_str_out){
			func_error(__func__,
				FEATURE_NOT_AVAILABLE,
				2, num_1, num_2
			);
			free(out->value);
			out->err=FEATURE_NOT_AVAILABLE;
			out->type=NONE;
			return out;
		}
		//Hacemos el valor a retornar.
		str_tmp=out->value;
		out->value=malloc(strlen((char*)num_1->value)+len_str_out);
		sprintf_with_invert(out->value, "%s%s", is_right, (char*)num_1->value, str_tmp);

		free(str_tmp);
		return out;
	}
#endif