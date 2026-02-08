#ifndef ERROR_C
#define ERROR_C 1
	#include <time.h>
	#include "./header/define.h"
	#include "./header/stack.h"
	#include "./header/str.h"
	const char* RERR_STR_MSG[ERROR_MSG_LIMIT]={
		// R
		"Datos no tratados",
		"Caracteristica no disponible",
		"No hay suficiente memoria disponible para la operación"
	};
	const char* GERR_STR_MSG[ERROR_MSG_LIMIT]={
		// G
		"Sintaxis incorrecta"
	};
	const char* BERR_STR_MSG[ERROR_MSG_LIMIT]={
		// B
		"No hay suficiente elementos en la pila para continuar la operación",
		"La pila está vacia",
		"Indice no encontrado",
		"No es posible la conversion."
	};
	U_INT error_code=NORMAL;
	/// Agregar prinft con valores por defectos, como linea, archivo, etc... y cada error indicará si quiere usar uno de eso, nota: Preparar un formateador para ello con $1 $2 $3,,,
	void show_error(const unsigned long int CODES,const unsigned int line, const unsigned int col){
		const short R_CODE=((CODES>>16) & 0xFF)-1;
		const short G_CODE=((CODES>>8) & 0xFF)-1;
		const short B_CODE=(CODES & 0xFF)-1;
		if (R_CODE>=0){
			msg_color("Error interno: ",RED_COLOR,BLACK_COLOR);
			puts(RERR_STR_MSG[R_CODE]);
		}
		if (G_CODE>=0){
			msg_color("Error: ",RED_COLOR,BLACK_COLOR);
			puts(GERR_STR_MSG[G_CODE]);
		}
		if (B_CODE>=0){
			msg_color("Advertensia: ",ORANGE_COLOR,BLACK_COLOR);
			puts(BERR_STR_MSG[B_CODE]);
		}
		printf("En la linea: \"%d\", columna: \"%d\".\n", line, col);
	}
	void func_error(
		const char* func,
		const unsigned int count_parameters,
		struct type_value** parameters, ...
	){
		char* param_str=get_str_param(count_parameters, parameters);
		printf(
			"Error in the function \"%s\"(%s). The mistakes are:\n",
			func, param_str
		);
		free(param_str);
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
			type=(char*)get_name_type(parameters[i]->type);
			value=tv_to_string(parameters[i], &len_tmp);
			len_tmp+=strlen(type)+len+15+((!value)?6:0);
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
	clock_t init_time;
	void init_get_time(){
		 init_time = clock();
	}
	char* end_time(){
		static char buffer[20];
		double time;
		time = (double)(clock() - init_time) / CLOCKS_PER_SEC;
		sprintf(buffer, "%f Second", time);
		return buffer;
	}
#endif