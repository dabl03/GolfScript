#ifndef RUN_C
#define RUN_C 1
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <limits.h>
	#include <gmp.h>
	#include "header/define.h"
	#include "header/str.h"
	#include "header/stack.h"
	#include "header/run.h"
	
	int run(struct Header_Stack* lines, struct Header_Stack* stack, struct Header_Stack* vars){
		U_INT tmp_istr=0;
		char* tmp_str=NULL;
		struct Stack_* stc_line=stack->stack;
		bool show_msg;
		
		while (stc_line!=NULL && !quit){
			if(stc_line->item.type!=STRING)continue;
			const char* s_line=stc_line->item.value;
			show_msg=true;
			U_INT i_end=strlen(s_line);

			for(U_INT i=0;i<i_end;i++){
				//Primero definimos nuestros signos constantes:
				if(s_line[i]=='{'){
					tmp_str=get_ie_block(s_line,i,'}',&tmp_istr);
					add_stack(stack,CODES_BLOCKS,tmp_str);
					//Terminamos.
					i=tmp_istr;
				}else if (IF_INIT_COMENT(s_line[i]))//Llegamos a un comentario.
					break;
				else if (s_line[i]==';'){
					struct type_value* tv_temp=pop_stack(stack);
					if (tv_temp!=NULL) {
						delete_item(tv_temp->type,tv_temp->value);
						free(tv_temp);
					}
					else{
						if (show_msg){
							puts("Warnign: La pila esta vacia.");
							show_msg=false;
						}
					}
				}else if(s_line[i]==':'){
					// We assign a variable
					if(IF_ENDL(s_line[i+1]))
						continue;
					i++;
					char* name=get_name_var(s_line,&i,i_end);
					if (!stack->i){
						//It is placed here to ignore the name of the variable
						free(name);
						printf("ERROR: La pila esta vacia.%s",ENDL);
						continue;
					}
					struct Var* vr_now=search_var(name,vars);
					if(vr_now!=NULL){
						// We overwrite the variable
						setValue_tv(vr_now,NULL,&stack->value[stack->i-1]);
					}else{
						// We initialize the variable
						struct type_value* stack_var=&stack->value[stack->i-1];

						setValue_tv(vr_now,name,stack_var);
						add_stack(vars,VAR,(void*)vr_now);
					}
					free(name);
				}else if(IF_ENDL(s_line[i])){
					continue;//Por velocidad. Ignoramos los saltos de lineas como el interprete original.
				}else if(s_line[i]=='['){//Arrays:
					tmp_str=get_ie_block(s_line,i,']',&tmp_istr);
					//Nueva linea y nuevo stack.
					struct Header_Stack hstc_line={NULL,NULL};
					struct Header_Stack* hstc_sub=(struct Header_Stack*)malloc(sizeof(struct Header_Stack));
					hstc_sub->stack=NULL;
					hstc_sub->father=stack;
					//Iniciamos la linea.
					add_stack(&hstc_line,STRING,tmp_str);
					//Ejecutamos y agregamos al stack el array.
					run(&hstc_line,sub_stack,vars);
					add_stack(stack,STACK,(void*)hstc_sub);
					delete_stack(&hstc_line);//Liberamos memoria
					//Terminamos.
					i=tmp_istr;
				}else{
					//Ahora vemos si existe la variable.
					char* name=get_name_var(s_line,&i,0);
					Var* vr_now=search_var(name,vars);
					if(vr_now!=NULL){
						// The variable exists
						process_data(stack,vars,vr_now);
					}else if(is_num(name[0]) || (name[0]=='-' && is_num(name[1])) ){
						// Variable not defined. We check if it is a number.
						unsigned int len=strlen(name)-1;
						if (len<=CLIMIT_INT){
							int* v=(int*)malloc(sizeof(int));
							*v=(int)atoi(name);
							add_stack(stack,INT,(void*)v);
						}else{
							mpz_t* n=(mpz_t*)malloc(sizeof(mpz_t));
							mpz_init_set_str(*n,name,0);
							add_stack(stack,LONGINT,n);
						}
					}else if (IF_INIT_STRING(s_line[i])){
						// It's a string
						unsigned int len=strlen(name);
						char* scape_=(char*)malloc(len);
						strncpy(scape_, name + 1, len-2);
            scape_[len-2]='\0';

						add_stack(stack,STRING,scape_);
					}
					free(name);
				}
			}
			stc_line=stc_line->next;
		}
		return 0;
	}
	
	char* get_name_var(const char* search,unsigned int* index,unsigned int end){
		struct String name = {3, 0, (char *)malloc(3)};
		unsigned int i = *index;// It will be used outside of loops
		end=(end)?end:strlen(search);

		if ( is_abc(search[*index]) ){
			// If it's an alphabet character, then it's variable name.
			for (;
				i < end && (is_abc(search[i]) || is_num(search[i]));
				i++
			);
			// i-1 per non-alphanumeric character
			str_add_str_end(&name, search+*index, i--);
		}else if (is_num(search[*index]) || (search[*index]=='-' && is_num(search[*index+1])) ){ // Si es un numero.
			for (
				i+=(search[*index]=='-')?1:0;
				i < end && is_num(search[i]);
				i++
			);
			str_add_str_end(&name, search+*index, i--);
		}else if(IF_INIT_STRING(search[*index])){//Obtenemos la cadena.
			*index=get_end_str(search+i,end);
			*index=(*index)?*index:end;
			free(name.str);
			return get_sub_str(search,i,*index);
		}else{ // Espacio y otros simbolos.
			name.str[0] = search[*index];
			name.str[1] = '\0';
			name.count=2;
		}
		*index=i;
		return (char*)realloc(name.str,name.count+1);
	}
	
	char* get_str_token(char* str,U_INT init,U_INT end){
		NEW_STRING(out,20);
		bool space=false;
		end=(end)?end:strlen(str);
		U_INT tmp_i=0;
		for(U_INT i=init;i<end;i++){
			if (str[i]=='\n' || str[i]==' ')
				space=true;
			else{
				if (space){
					cadd_leftover(&out,' ');
					space=false;
				}
				cadd_leftover(&out,str[i]);
				if (IF_INIT_STRING(str[i])){
					tmp_i=get_end_str(str+i,end);
					tmp_i=(tmp_i)?tmp_i:end;
					out.count--;//Porque necesitamos que ignore la entre comilla anterior.
					str_add_str_end(&out,str+i,tmp_i);
					i=tmp_i;
				}else if (IF_INIT_COMENT(str[i])){
					out.count--;
					for (;i<end && str[i]!='\n';i++);
					continue;
				}
			}
		}
		cadd_leftover(&out,'\0');
		return (char*)realloc(out.str,out.count);
	}
	
	char* get_ie_block(const char* input,const U_INT init, const char end, U_INT* out_end){
		NEW_STRING(out,20);
		char cinit=input[init];
		U_INT i=init+1,
		sub=1;
		U_INT tmp_i=0,
		iend=strlen(input);
		char* tmp_str=NULL;
		for (;i<iend;i++){

			if (input[i]==cinit)
				sub++;
			else if (input[i]==end){
				sub--;
				if (sub==0)
					break;
			}
			switch(input[i]){
				case '"': /*OR*/ case '\'':
					tmp_i=get_end_str(input+i,iend);
					tmp_str=get_sub_str(input,i,tmp_i);
					str_add_str(&out,tmp_str);
					i=(tmp_i)?tmp_i:iend;
					free(tmp_str);
					if (i+1<iend AND input[i+1]!=' ')
						cadd_leftover(&out,' ');
					break;
				case COMMENT:
					for (;input[i]!='\n' AND input[i]!='\0';i++);//Se ignora todo
				case '\n':
					if (input[i-1]!=' ')
						cadd_leftover(&out,' ');
					break;
				default:
					cadd_leftover(&out,input[i]);
			}
			
		}
		//Para mas velocidad no llamamos a cadd_leftover.
		if (out.count+sub+1>out.max){//Ya no necesitamos la variable max de la estructura.
			out.str=(char*)realloc(out.str,out.count+sub+2);
		}
		//Si sigue abierto la cerramos.
		while(sub>0){
			out.str[out.count++]=end;
			sub--;
		}
		if (out_end!=NULL)
			*out_end=i;
		out.str[out.count++]='\0';//Retornamos la cadena sin el inicio o el fin del bloque.
		return (char*)realloc(out.str,out.count);
	}
#endif
