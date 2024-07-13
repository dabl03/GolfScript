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
	
	int run(struct Array* arrLines,struct Array* stack,struct Array* vars){
		U_INT tmp_istr=0;
		char* tmp_str=NULL;
		for (U_INT i_line=0;i_line<arrLines->i && !quit;i_line++){
			if(arrLines->value[i_line].type!=STRING)
				continue;
			const char* l=(char*)arrLines->value[i_line].value;
			U_INT i_end=strlen(l);
			for(U_INT i=0;i<i_end;i++){
				//Primero definimos nuestros signos constantes:
				if(l[i]=='{'){
					tmp_str=get_ie_block(l,i,'}',&tmp_istr);
					add_array(stack,CODES_BLOCKS,tmp_str);
					//Terminamos.
					i=tmp_istr;
				}else if (IF_INIT_COMENT(l[i]))//Llegamos a un comentario.
					break;
				else if (l[i]==';'){
					if (stack->i){
						struct type_value* tv=pop_array(stack);
						delete_item(tv->type,tv->value);
					}else{
						puts("Warnign: La pila esta vacia.");
					}
				}else if(l[i]==':'){//Para asignar una nueva variable.
					//Si la cadena esta vacia significa que no es variable.
					if (!stack->i){
						printf("ERROR: La pila esta vacia.%s",ENDL);
						break;
					}
					if(IF_ENDL(l[i+1]))//Por velocidad. Innoramos los saltos de lineas como el interprete original.
						continue;
					i++;
					char* name=get_name_var(l,&i,i_end);
					int i_var=search_var(name,vars);
					if(i_var!=-1){//Consiguio, se reemplaza.
						struct Var* this_var=(struct Var*)vars->value[i_var].value;
						setValue_tv(this_var,NULL,&stack->value[stack->i-1]);
						free(name);
						continue;
					}
					//No estubo definida antes.
					struct Var* this_var=(struct Var*)malloc(sizeof(struct Var));
					struct type_value* stack_var=&stack->value[stack->i-1];
					setValue_tv(this_var,name,stack_var);
					add_array(vars,VAR,(void*)this_var);
					free(name);
				}else if(IF_ENDL(l[i])){
					continue;//Por velocidad. Ignoramos los saltos de lineas como el interprete original.
				}else if(l[i]=='['){//Arrays:
					tmp_str=get_ie_block(l,i,']',&tmp_istr);
					//Nueva linea y nuevo stack.
					struct Array line={0,0,0};
					struct Array* sub_stack=(struct Array*)malloc(sizeof(struct Array));
					sub_stack->i=0;
					sub_stack->max=0;
					sub_stack->value=NULL;
					//Iniciamos la linea.
					add_array(&line,STRING,tmp_str);
					//Ejecutamos y agregamos al stack el array.
					run(&line,sub_stack,vars);
					add_array(stack,ARRAY,(void*)sub_stack);
					delete_array(&line);//Liberamos memoria
					//Terminamos.
					i=tmp_istr;
				}else{
					//Ahora vemos si existe la variable.
					char* name=get_name_var(l,&i,0);
					int i_var=search_var(name,vars);
					if(i_var!=-1){//Variable exists
						struct Var* v=vars->value[i_var].value;
						if (v->type==CODES_BLOCKS){
							struct Array arr={0,0,NULL};
							add_array(&arr,STRING,(char*)v->value);
							run(&arr,stack,vars);
							free(arr.value);
						}else{
							interpret(stack,vars,v);
						}
					}else if(is_num(name[0]) || name[0]=='-'){//Sino se ejecuta la condicion anterior, significa que - es para un numero.
						unsigned int len=strlen(name)-1;
						if (len<=CLIMIT_INT){
							int* v=(int*)malloc(sizeof(int));
							*v=(int)atoi(name);//parseInt(name);
							add_array(stack,INT,(void*)v);
						}else{
							mpz_t* n=(mpz_t*)malloc(sizeof(mpz_t));
							mpz_init_set_str(*n,name,0);
							add_array(stack,LONGINT,n);
						}
					}else if (IF_INIT_STRING(l[i])){//Llegamos a " o
						unsigned int len=strlen(name);
						char* scape_=(char*)malloc(len);
						strncpy(scape_, name + 1, len-2);
            scape_[len-2]='\0';

						add_array(stack,STRING,scape_);
					}
					free(name);
				}
			}
		}
		return 0;
	}
	
	char* get_name_var(const char* search,unsigned int* i,unsigned int end){
		struct String name = {3, 0, (char *)malloc(3)};
		unsigned int i_2 = *i;
		end=(end)?end:strlen(search);
		if (is_abc(search[*i])){ // Si es un nombre lo modificamos buscamos hasta fin de linea o espacio.
			for (; i_2 < end && (is_abc(search[i_2]) || is_num(search[i_2])); i_2++);
			str_add_str_init_end(&name, search, *i, i_2--);//Necesitamos disminuir para que el bucle vaja para el siguiente caracter.
		}else if (is_num(search[*i]) || search[*i]=='-'){ // Si es un numero.
			for (i_2+=(search[*i]=='-')?1:0;
				i_2 < end && is_num(search[i_2]); i_2++);
			str_add_str_init_end(&name, search, *i, i_2--);
		}else if(IF_INIT_STRING(search[*i])){//Obtenemos la cadena.
			*i=get_end_str(search,i_2,end);
			*i=(*i)?*i:end;
			free(name.str);
			return get_sub_str(search,i_2,*i);
		}else{ // Espacio y otros simbolos.
			name.str[0] = search[*i];
			name.str[1] = '\0';
			name.count=2;
		}
		*i=i_2;
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
					cadd_add_leftover(&out,' ');
					space=false;
				}
				cadd_add_leftover(&out,str[i]);
				if (IF_INIT_STRING(str[i])){
					tmp_i=get_end_str(str,i,end);
					tmp_i=(tmp_i)?tmp_i:end;
					out.count--;//Porque necesitamos que ignore la entre comilla anterior.
					str_add_str_init_end(&out,str, i,tmp_i);
					i=tmp_i;
				}else if (IF_INIT_COMENT(str[i])){
					out.count--;
					for (;i<end && str[i]!='\n';i++);
					continue;
				}
			}
		}
		cadd_add_leftover(&out,'\0');
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
					tmp_i=get_end_str(input,i,iend);
					tmp_str=get_sub_str(input,i,tmp_i);
					str_add_str_init_end(&out,tmp_str,0,0);
					i=(tmp_i)?tmp_i:iend;
					free(tmp_str);
					if (i+1<iend AND input[i+1]!=' ')
						cadd_add_leftover(&out,' ');
					break;
				case COMMENT:
					for (;input[i]!='\n' AND input[i]!='\0';i++);//Se ignora todo
				case '\n':
					if (input[i-1]!=' ')
						cadd_add_leftover(&out,' ');
					break;
				default:
					cadd_add_leftover(&out,input[i]);
			}
			
		}
		//Para mas velocidad no llamamos a cadd_add_leftover.
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
