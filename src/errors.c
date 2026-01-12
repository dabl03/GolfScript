#ifndef ERROR_C
#define ERROR_C 1
	#include "./header/define.h"
	#include "./header/stack.h"
	#include "./header/str.h"
	const char* RERR_STR_MSG[ERROR_MSG_LIMIT]={
		// R
		"Datos no tratados",
		"Caracteristica no disponible"
	};
	const char* GERR_STR_MSG[ERROR_MSG_LIMIT]={
		// G
		"Sintaxis incorrecta"
	};
	const char* BERR_STR_MSG[ERROR_MSG_LIMIT]={
		// B
		"Se han pasado insuficientes argumentos",
		"La pila está vacia"
	};
	U_INT error_code=NORMAL;
	/// Agregar prinft con valores por defectos, como linea, archivo, etc... y cada error indicará si quiere usar uno de eso, nota: Preparar un formateador para ello con $1 $2 $3,,,
	void show_error(const unsigned long int CODES,const unsigned int line){
		const short R_CODE=CODES>>4;
		const short G_CODE=(CODES<<2)>>4;
		const short B_CODE=(CODES<<4)>>4;
		if (R_CODE){
			msg_color("Error interno: ",RED_COLOR,BLACK_COLOR);
			puts(RERR_STR_MSG[R_CODE]);
		}
		if (G_CODE){
			msg_color("Error: ",RED_COLOR,BLACK_COLOR);
			puts(GERR_STR_MSG[G_CODE]);
		}
		if (B_CODE){
			msg_color("Advertensia: ",ORANGE_COLOR,BLACK_COLOR);
			puts(BERR_STR_MSG[B_CODE]);
		}
	}
	void func_error(
		const char* func,
		const unsigned int UI_ERR_CODES,
		const unsigned int count_parameters,
		struct type_value** parameters, ...
	){
		char* param_str=get_str_param(count_parameters, parameters);
		int err_codes=UI_ERR_CODES;
		printf(
			"Error in the function \"%s\"(%s). The mistakes are:\n",
			func, param_str
		);
		free(param_str);
		// Puts all errors.
		if (err_codes>=APP_UNKNOWN_DATA){
			puts(RERR_STR_MSG[err_codes-APP_UNKNOWN_DATA]);
			err_codes=(err_codes<<2)>>2;
		}
		if (err_codes>=NOT_FOUND){
			puts(RERR_STR_MSG[err_codes-NOT_FOUND]);
			err_codes=(err_codes<<2)>>2;
		}
		if (err_codes){
			puts(RERR_STR_MSG[err_codes]);
		}
	}
	char* get_str_param(const unsigned int count_parameters, struct type_value** parameters){
		unsigned int len=1;
		unsigned int len_tmp;
		int i=0;
		char* type,
			* value,
			*out=(char*)malloc(1);
			*out='\0';
		while (parameters[i]!=NULL){
			type=get_name_type(parameters[i]->type);
			value=to_string_value(parameters[i]->type, parameters[i]->value);
			len_tmp=strlen(type)+strlen(value)+len+15+((!value)?6:0);
			out=(char*)realloc(out, len_tmp);
			// TYPE=4 + ' '=1 + VALUE=5 + ""=2 + ' , '=3 + == 15 + (NONE)=6 == 21
			sprintf(out+(len-1), "TYPE=(%s)  VALUE=\"%s\" , ", 
				type,
				(value)? value:"(NONE)" // IF null then= NONE
			);
			len=len_tmp;
			if (value)
				free(value);
			i+=1;
			if (count_parameters-i!=0)
				break;
		}
		return out;
	}
#endif