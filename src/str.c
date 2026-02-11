#ifndef STR_C
#define STR_C 1
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include <locale.h>// localeconv
#include "./header/define.h"
#include "./header/str.h"

char* SYSTEM_DECIMAL_POINT;
static void __attribute__((constructor)) init_globals() {
	setlocale(LC_ALL, "");
	struct lconv* lc = localeconv();
	char* tmp=lc->decimal_point;
	SYSTEM_DECIMAL_POINT=(char*)MALLOC__(strlen(tmp));
	strcpy(SYSTEM_DECIMAL_POINT, tmp);
}
static void __attribute__((destructor)) free_globals() {
	FREE__(SYSTEM_DECIMAL_POINT);
}
unsigned short exist_char(const char* str,unsigned const char c){
	for (unsigned int i=0;str[i]!='\0';i++)
		if ((unsigned int)str[i]==c)
			return 1;//Se encontró:)
	return 0;//No se encontro el char:(
}

unsigned short is_abc(unsigned const char c){
	return ABC(c);
}

unsigned short is_num(unsigned const char c){
	return (c>='0' && c<='9');
}

uint get_end_str(const char* str, uint i_end){
	bool is_scape=false;
	unsigned const char CHAR_END=str[0]; //< Is " or ' and no with "\"
	i_end=(!i_end)?strlen(str):i_end;
	for( uint i=0; i<i_end; i++ ){
		if (str[i]==CHAR_END AND !is_scape)
			return i;
		is_scape=(str[i]=='\\' && !is_scape);
	}
	return 0;// Not found
}

int parseInt(const char* str){
	const bool IS_NEGATIVE=(str[0]=='-');
	int output=0;
	for (unsigned int i=IS_NEGATIVE;str[i]!='\0';i++){
		if (is_num(str[i])){
			output=(output*10)+CHAR_TO_NUM(str[i]);
		}
	}
	return (IS_NEGATIVE)?-output:output;
}

long int parseLongInt(const char* str){
	const bool IS_NEGATIVE=(str[0]=='-');
	long int output=0;
	for (int i=IS_NEGATIVE;str[i]!='\0';i++){
		if (is_num(str[i])){
			output=(output*10)+CHAR_TO_NUM(str[i]);
		}
	}
	return (IS_NEGATIVE)?-output:output;
}

void str_add_str_end(struct String* str_d,  const char* str_copy, int64_t count){
	count=(count < 0)? strlen(str_copy) : count;
	if ( str_d->count+count>=str_d->max ){
		str_d->str=(char*)realloc(str_d->str,sizeof(char)*(str_d->max+=count+1));
	}
	char* chr_now=(char*)str_copy;
	for (unsigned int i=0; i<count ; i++){
		str_d->str[str_d->count++]=*chr_now;
		chr_now++;
	}
	str_d->str[str_d->count]='\0';
}

void str_add_char(struct String* str_d,const char c){
	if (str_d->count+1>=str_d->max){
		str_d->str=(char*)realloc(
			str_d->str,
			sizeof(char)*(str_d->max+=( (str_d->max)?1:2) )
		);
	}
	str_d->str[str_d->count++]=c;
	str_d->str[str_d->count]='\0';
}

void init_str(struct String* str,unsigned int buffer){
	str->str=(char*)malloc(sizeof(char)*buffer);
	*str->str='\0';
	str->count=0;
	str->max=buffer;
}

char scape_char(const char c){
	switch (c){
		case 'n':
			return '\n';
		case 'b':
			return '\b';
		case 'r':
			return '\r';
		case 'a':
			return '\a';
		case 'e':
			return '\e';
		case 't':
			return '\t';
		case 'v':
			return '\v';
		case '\\':
			return '\\';
		case 'f':
			return '\f';
		/*case '\''://En golfscript esto no se usa
			return '\'';*/
		case '"':
			return '"';
		case '0':
			return '\0';
		case '?':
			return '\?';
		case 'x':
			return -2;
		default:
			return -1;
	}
}

char nscape_char(char c){
	switch(c){
		case '\n':
			return 'n';
		case '\a':
			return 'a';
		case '\e':
			return 'e';
		case '\b':
			return 'b';
		case '\f':
			return 'f';
		case '\r':
			return 'r';
		case '\t':
			return 't';
		case '\v':
			return 'v';
		case '\\':
			return '\\';
		/*case '\''://En golfscript esto no se usa
			return'\'';*/
		case '"':
			return '"';
		case '\?':
			return '\?';
		case '\0':
			return '0';
		default:
			return -1;
	}
}

const char* get_name_type(enum TYPE t){
	switch(t){
		case INT:
			return "(INT)";
		case LONGINT:
			return "(LONG INT)";
		case FLOAT:
			return "(FLOAT)";
		case LONGFLOAT:
			return "(LONG FLOAT)";
		case STRING:
			return "(STRING)";
		case VALUE_TYPE:
			return "VALUE_TYPE";
			/*Usado por el interprete:*/
		case FUNCTION:
			return "(NATIVE FUNCTION)";
		case VAR:
			return "(VAR)";
		case CODES_BLOCKS:
			return "(CODES BLOCKS)";
		case STACK:
			return "(STACK)";
		case NONE:
			return "(NONE)";
		default:
			return NULL;
	}
}
/// Muestra los caracteres invisibles.
char* get_str_nescp(char* old_str){
	uint len=strlen(old_str),
	i_nstr=0;
	char* new_str=(char*)malloc(len);//Tamaño base.
	char chr;
	for (char* c=old_str;*c!='\0';c++){
		if (i_nstr+5>=len){// "\x00"+'\0'
			new_str=(char*)realloc(new_str,len=i_nstr+20);
		}
		if (is_invisible_char(*c)){
			// Caracter invisible.
			chr=nscape_char(*c);
			new_str[i_nstr++]='\\';
			if (chr==-1){
				// Mostrar como hexadecimal.
				new_str[i_nstr++]='x';
				i_nstr=sprintf(new_str+i_nstr, "%02x", *c)+i_nstr;
			}else{
				// Mostrar un caracter mas grafico.
				new_str[i_nstr++]=chr;
			}
		}else{
			new_str[i_nstr++]=*c;
		}
	}
	new_str[i_nstr]='\0';
	return (char*)realloc(new_str,i_nstr+1);
}
/// @TODO: Mejorar la sintaxis para una mejor lectura.
/// Transforma las secuencia de escape en el caracter
///              correspondiente
char* get_str_escp(char* old_str){
	uint len=strlen(old_str),
	i_nstr=0;
	char* new_str=(char*)malloc(len);//Tamaño base.
	char chr=0;
	bool is_scape=false;
	for (char* c=old_str;*c!='\0';c++){
		if (i_nstr+3>=len){
			new_str=(char*)realloc(new_str,len=i_nstr+20);
		}
		if (*c=='\\' AND NOT is_scape){
			is_scape=true;
			continue;
		}else if (is_scape){
			chr=scape_char(*c);
			if (chr==-1){
				new_str[i_nstr++]=*c;
			}else{
				if (chr==-2 AND (*(c+1)!='\0' AND *(c+2)!='\0')){//Convertimos a hexadecimal.
					char* tmp=(char*)alloca(3);
					tmp[0]=*(++c);
					tmp[1]=*(++c);
					tmp[2]='\0';
					uint otro=(int) strtol(tmp, NULL, 16);
					//Si el entero sobrepasa los limites entonces hacemos mas hexadecimales.
					if (otro>CHAR_MAX){
						new_str[i_nstr++]=CHAR_MAX;
						chr=(char)otro-CHAR_MAX;
					}else
						chr=(char)otro;
				}
				new_str[i_nstr++]=chr;
			}
			is_scape=false;
			continue;
		}else{
			if (*c=='"')//Como no tiene un scape entonces la cadena se mostrara sin ""
				continue;
			new_str[i_nstr++]=*c;
		}
	}
	new_str[i_nstr]='\0';
	return (char*)realloc(new_str,i_nstr+1);
}

void cadd_leftover(struct String* str_,const char c){
	if (str_->count+3>=str_->max)
		str_->str=(char*)realloc(str_->str,str_->max+=20);
	str_->str[str_->count++]=c;
	str_->str[str_->count]='\0';
}

char* get_sub_str(const char* str,uint init, uint end){
	uint end_c=(end)?end:strlen(str);
	struct String out;
	init_str(&out, end_c+1);
	char type=str[init++];
	bool is_scape=false;
	out.str[out.count++]='"';
	for (int i=init;i<end_c;i++){
		if (str[i]==type AND !is_scape)//Si es termino y no es un escape.
			break;//Conseguimos el final de la cadena por lo que terminamos.
		if (type=='\'' && str[i]=='"')
			cadd_leftover(&out,'\\');
		cadd_leftover(&out,str[i]);
		is_scape=(str[i]=='\\' && !is_scape);
	}
	cadd_leftover(&out,'"');
	cadd_leftover(&out,'\0');
	return (char*)realloc(out.str,out.count);
}

char* itoa(int value, char* result, int base) {
	// check that the base if valid
	if (base < 2 || base > 36){
		*result = '\0';
		return result;
	}
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;
	do {
		tmp_value = value;
		value /= base;
		*ptr++ ="zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );
	// Apply negative sign
	if (tmp_value < 0)
		*ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr){
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

bool is_invisible_char(unsigned char chr){
	return (chr<32 && chr!=9) || chr=='"';// 9 is tab horizontal and 32 is space.
}
char* copy_str(const char* str_io){
	char* out=(char*)malloc( SIZE_CHAR(strlen(str_io)+1) );
	strcpy(out, str_io);
	return out;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-int"
// Convert to string operatos...
#define TO_STRING_FUNCTIONS_BEGIN(NAME, INDEX) \
	char* NAME(const struct type_value* tv_element, unsigned int* out_len){ \
		unsigned int len=0; \
		char *out;
#define TO_STRING_FUNCTIONS_END() if (out_len!=NULL){ \
			*out_len=len; \
		} \
		return (char*)realloc(out, len+1); \
	}

TO_STRING_FUNCTIONS_BEGIN(int_to_string, INT);
	out=malloc(SIZE_CHAR(13)); // Max int 32bits is 12 digits.
	len=sprintf(out,"%d", *(int*)tv_element->value);// Buscar una funcion mas eficiente.
TO_STRING_FUNCTIONS_END();
// Para long_int_to_string se debe usar: FREE__() con la cadena retornada
TO_STRING_FUNCTIONS_BEGIN(long_int_to_string, LONGINT);
	out=mpz_get_str(NULL, 0, *(mpz_t*)tv_element->value);// Error.
	len=strlen(out);
	#if defined(DEBUG) || defined(TEST_)
		// Si out vá a realloc,
		// entonces dará error
		if (out_len!=NULL)
			*out_len=len;
		return out;
	#endif
TO_STRING_FUNCTIONS_END();

TO_STRING_FUNCTIONS_BEGIN(float_to_string, FLOAT);
	out=malloc(SIZE_CHAR(60)); // Max int 32bits is 12 digits.
	len=sprintf(out, "%.*lf", CLIMIT_FLOAT-2,*(double*)tv_element->value);
TO_STRING_FUNCTIONS_END();
TO_STRING_FUNCTIONS_BEGIN(long_float_to_string, LONGFLOAT);
	mp_exp_t mp_exponent;
	char* s_generic=mpf_get_str(
		NULL,
		&mp_exponent,
		10, 0,
		*(mpf_t*)tv_element->value
	);
	// 18: 6->"%.s%s "+{s_generic 10 character}+1->\0.
	char format[18];
	mp_exponent+=*s_generic=='-';// Si es negativo se necesita sumar 1.

	len=strlen(s_generic+mp_exponent)+strlen(SYSTEM_DECIMAL_POINT)+mp_exponent+1;
	out=(char*)malloc(SIZE_CHAR(len));
	// format="%.mp_exponents%s%s"
	sprintf(format," %%.%lds%%s%%s", (long)mp_exponent);
	len=sprintf(out, format, s_generic, SYSTEM_DECIMAL_POINT, s_generic+mp_exponent);
	FREE__(s_generic);
TO_STRING_FUNCTIONS_END();

TO_STRING_FUNCTIONS_BEGIN(block_codes_to_string, CODES_BLOCKS);
	out=(char*)malloc(strlen((char*)tv_element->value)+3);// 2 -> {} 1 -> \0
	len=sprintf(out, "{%s}", (char*)tv_element->value);
TO_STRING_FUNCTIONS_END();
TO_STRING_FUNCTIONS_BEGIN(str_to_string, STRING);
	char* str_tmp=get_str_nescp((char*)tv_element->value);
	out=(char*)malloc(strlen(str_tmp)+3);// 2 -> "" 1 -> \0
	len=sprintf(out, "\"%s\"", str_tmp);
	free(str_tmp);
TO_STRING_FUNCTIONS_END();

TO_STRING_FUNCTIONS_BEGIN(stack_to_string, STACK);
	char* s_temp;
	char* out_copy;
	unsigned int len_tmp=0;
	struct Stack_* stc_now=((struct Header_Stack*)tv_element->value)->stack;
	out=(char*)malloc(1);
	*out='\0';
	if (stc_now==NULL)
		return out;
	while (stc_now!=NULL){
		s_temp=tv_to_string(&stc_now->item, &len_tmp);
		out_copy=(char*)malloc(SIZE_CHAR(len_tmp+len+1));
		len=sprintf(out_copy, " %s%s", s_temp, out)+len;
		free(out);
		out=out_copy;
	#if defined(DEBUG) || defined(TEST_)
		if (stc_now->item.type==LONGINT)
			FREE__(s_temp);
		else
			free(s_temp);
	#else
		free(s_temp);// Evitar advertensia.
	#endif
		
		stc_now=stc_now->next;
	}
	out_copy=out;
	out=(char*)malloc(SIZE_CHAR(len+5));// 2->' '+\0 and 3-> [ ] (end)
	len=sprintf(out,"[%s ]", out_copy);
	free(out_copy);
TO_STRING_FUNCTIONS_END();

TO_STRING_FUNCTIONS_BEGIN(var_to_string, VAR);
	char* s_generic=to_string(
		((struct Var*)tv_element)->item.type,
		((struct Var*)tv_element)->item.value,
		&len
	);
	// 4 -> "(=)\0" 
	out=(char*)malloc(SIZE_CHAR(
			strlen( ((struct Var*)tv_element)->name )+len+4
		)
	);
	len=sprintf(
		out,
		"(%s=%s)",
		((struct Var*)tv_element)->name,
		s_generic
	);
	free(s_generic);
TO_STRING_FUNCTIONS_END();
TO_STRING_FUNCTIONS_BEGIN(function_to_string, FUNCTION);
	char* s_generic="(Native-Function)";
	len=strlen(s_generic);
	out=(char*)malloc(SIZE_CHAR(len+1));
	strcpy(out, s_generic);
TO_STRING_FUNCTIONS_END();
TO_STRING_FUNCTIONS_BEGIN(none_to_string, NONE);
	char* s_generic="(NONE)";
	len=strlen(s_generic);
	out=(char*)malloc(SIZE_CHAR(len+1));
	strcpy(out, s_generic);
TO_STRING_FUNCTIONS_END();
#pragma GCC diagnostic pop

char* tv_to_string(const struct type_value* tv_element, unsigned int * out_len){
	static char * (*CONVERT_TO_STRING_FNC[END_ELEMENT])(
		const struct type_value* tv_element,
		unsigned int * out_len
	)={NULL};
	char* out;
	if (CONVERT_TO_STRING_FNC[0]!=NULL){
		out=CONVERT_TO_STRING_FNC[tv_element->type](tv_element, out_len);
		#if defined(DEBUG) || defined(TEST_)
			if (tv_element->type==LONGINT){
				char* out_tmp=(char*)malloc(*out_len+1);
				strcpy(out_tmp, out);
				FREE__(out);
				out=out_tmp;
			}
		#endif
		return out;
	}
	CONVERT_TO_STRING_FNC[INT]=int_to_string;
	CONVERT_TO_STRING_FNC[LONGINT]=long_int_to_string;
	CONVERT_TO_STRING_FNC[FLOAT]=float_to_string;
	CONVERT_TO_STRING_FNC[LONGFLOAT]=long_float_to_string;
	CONVERT_TO_STRING_FNC[CODES_BLOCKS]=block_codes_to_string;
	CONVERT_TO_STRING_FNC[STRING]=str_to_string;
	CONVERT_TO_STRING_FNC[STACK]=stack_to_string;
	CONVERT_TO_STRING_FNC[VAR]=var_to_string;
	CONVERT_TO_STRING_FNC[FUNCTION]=function_to_string;
	CONVERT_TO_STRING_FNC[NONE]=none_to_string;
	return tv_to_string(tv_element, out_len);
}
char* to_string(const enum TYPE type, const void* value, unsigned int* out_len){
	const struct type_value tv_io={type, (void*)value};
	return tv_to_string(&tv_io, out_len);
}

#undef TO_STRING_FUNCTIONS_BEGIN
#undef TO_STRING_FUNCTIONS_END
#endif
