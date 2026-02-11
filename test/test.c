#include "header/test.h"

FuncTest* func_test=NULL;

void push_function_test(bool (*func)(const char** out), char* name, bool is_name_dinamyc, bool is_stop){
	FuncTest* new_test_func=(FuncTest*)malloc(sizeof(FuncTest));
	if (FuncTest==NULL){
		func_test=new_test_func;
		func_test->next=NULL;
	}else{
		new_test_func->next=func_test->next;
		new_test_func->func=func_test->func;
		func_test->next=new_test_func;
	}
	func_test->func=func;
	func_test->name=name;
	func_test->is_name_dinamyc=is_name_dinamyc;
	func_test->is_stop=is_stop;
}
int ExecuteTest(){
	FuncTest* now=func_test;
	FuncTest* next;
	uint len=0;
	char* out_test=NULL;
	int count_error=0;
	if (now==NULL){
		puts("No se ha agregado funciones para testear.");
		return -1;
	}
	for (;now!=NULL;i++, now=now->next);
	// Lo convertimos a array porque es mas facil invertir el orden.
	FuncTest** all_test=(FuncTest**)malloc(sizeof(FuncTest**)*(len+1));
	for (int i=len-1; i>0; i--){
		if (all_test[i]->func(&out_test)){
			count_error++;
			show_repeat_chr('=', 80, true);
			show_str_center(' ', 80, "Test a fallado en la función:", true);
			show_str_center(' ', 80, all_test[i]->name, true);
			show_repeat_chr('=', 80, true);
			if (out_test!=NULL){
				printf("El mensaje del fallo:\n%s\n", out_test);
				free(out_test);
			}else{
				puts("No hay mensaje de esta funcion.");
			}
			if (all_test[i]->is_stop)
				break;
			show_repeat_chr('-', 80, true);
		}else{
			#ifdef LOG_TEST
			printf("Funcion \"%s\" ha pasado.\n", all_test[i]->name);
			#endif
			if (out_test!=NULL){
				#ifdef LOG_TEST
				printf("El mensaje de la funcion:\n%s\n", out_test);
				#endif
				free(out_test);
			}
		}
		out_test=NULL;
	}
	free(all_test);
	return count_error;
}
/*Nota: func_test->name se libera si is_name_dinamyc es true.*/
void delete_all_test(){
	FuncTest* now=func_test;
	FuncTest* next;
	while (now!=NULL){
		next=now->next;
		if (now->is_name_dinamyc)
			free(now->name);
		free(now);
		now=next;
	}
}
#ifndef MAIN
	#define MAIN "execute_test_main"
	#define execute_test_main test_c_main
#endif
int test_c_main(...){
	return ExecuteTest();
}
static void __attribute__((destructor)) free_globals() {
	delete_all_test();
}
void show_repeat_chr(char for_repeat, uint count, bool is_end_line){
	for (uint i=0; i<count; i++)
		putchar(for_repeat);
	if (is_end_line)
		putchar('\0');
}
void show_str_center(char separator, uint max_len, char* msg, bool is_end_line){
	uint len=strlen(msg);
	max_len=(len>max_len)?0:max_len;
	uint count_separator=(len-max_len)/2;
	repeat_chr(separator, count_separator, false);
	printf("%s", msg);
	repeat_chr(separator, count_separator, is_end_line);
}