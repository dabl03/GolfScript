#ifndef STACK_C
#define STACK_C 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./header/str.h"
#include "./header/stack.h"
#include "./header/define.h"

bool add_array(struct Array* arr_allData,const enum TYPE typ_data, void* v_data){
	// Si nos quedamos sin espacio:
	if (arr_allData->i==arr_allData->max){
		// Reservamos 10 espacios libres.
		arr_allData->value=(struct type_value*)realloc(
			(void*)arr_allData->value,
			sizeof(struct type_value)*(arr_allData->max+=10)
		);
		if (arr_allData->value==NULL)
			return true;
	}else if (arr_allData->value==NULL){
	  arr_allData->i=0;
		arr_allData->max=0;
		return true;
	}
	
	arr_allData->value[arr_allData->i].type=typ_data;
	arr_allData->value[arr_allData->i++].value=v_data;
	return false;
}
struct type_value* pop_array(struct Array* arr_allData){
	// Liberammos lo que no usaremos.
	if (arr_allData->i<(arr_allData->max-10)){
		arr_allData->max-=10;
		arr_allData->value=(struct type_value*)realloc((void*)arr_allData->value,sizeof(struct type_value)*arr_allData->max);
	}
	return &arr_allData->value[--arr_allData->i];//Quitamos un elemento de la pila y retornamos ese elemento.
}
bool delete_array(struct Array* arr_allData){
	if(arr_allData->value==NULL)
		return true;
	
	for (unsigned int i=0;i<arr_allData->i;i++){
		delete_item(arr_allData->value[i].type,arr_allData->value[i].value);
	}
	
	//Liberamos el array y ponemos todo en NULL
	free(arr_allData->value);
	arr_allData->max=0;
	arr_allData->i=0;
	arr_allData->value=NULL;
	return false;
}
struct Array* copy_array(const struct Array* arr_allData){
	struct Array* arr_out=(struct Array*)malloc(sizeof(struct Array));
	void* v_generic;// Dinamic vars.
	
	arr_out->i=0;
	arr_out->max=0;
	arr_out->value=NULL;
	
	for (unsigned int i=0;i<arr_allData->i;i++){
		switch(arr_allData->value[i].type){
			case VAR:
				perror("No se añadio la caracteristica.\n"
				  "En funcion copy_array->caso VAR.\n"
			  );
				exit(-4);
				break;
			case ARRAY:
				v_generic=copy_array((struct Array*)arr_allData->value[i].value);
				break;
			case INT:
				v_generic=malloc(sizeof(int));
				*(int*)v_generic=*(int*)arr_allData->value[i].value;
				break;
			case FLOAT:
				v_generic=malloc(sizeof(double));
				*(double*)v_generic=*(double*)arr_allData->value[i].value;
				break;
			case LONGINT:
				v_generic=malloc(sizeof(mpz_t));
				mpz_init(*(mpz_t*)v_generic);
				mpz_set(*(mpz_t*)v_generic,*(mpz_t*)arr_allData->value[i].value);
				break;
			case LONGFLOAT:
				v_generic=malloc(sizeof(mpf_t));
				mpf_init_set(*(mpf_t*)v_generic,*(mpf_t*)arr_allData->value[i].value);
				break;
			case CODES_BLOCKS:
			case STRING:
				v_generic=malloc(strlen((char*)arr_allData->value[i].value)+1);
				strcpy((char*)v_generic,(char*)arr_allData->value[i].value);
				break;
			default:
				perror("Error interno de la app, en la funcion copy_array->default.\n");
				printf("arr_allData->value[i].type= %s\n",get_name_type(arr_allData->value[i].type) );
				exit(-2);
		}
		add_array(arr_out,arr_allData->value[i].type,v_generic);
	}
	return arr_out;
}
void array_set_item(struct Array* arr_allData,const bool b_isAppend,const int i_indexSet, const enum TYPE typ_data, void* v_data){
	// Modo modificar
	if (!b_isAppend){
		unsigned int i_index=(i_indexSet!=-1)?i_indexSet:arr_allData->i-1;

		delete_item(arr_allData->value[i_index].type,arr_allData->value[i_index].value);
		arr_allData->value[i_index].type=typ_data;
		arr_allData->value[i_index].value=v_data;
		return;
	// Agregar al final.
	}else if (i_indexSet==-1){
		add_array(arr_allData,typ_data,v_data);
		return;
	}else if(i_indexSet>arr_allData->i || i_indexSet<0){
		printf("Error: se intenta acceder a una posición que no se puede.\n"
			"\nIndice pasado: %d  -  Indice maximo a modificar: %d.\n",i_indexSet,arr_allData->i);
		return;
	}
	struct type_value tv_nowItem,
	  tv_afterItem;
	//Modo Append:
	//Aumentamos el array
	add_array(arr_allData,PCHAR,NULL);
	//Agregamos en el indice el dato y guardamos el dato anteior.
	tv_afterItem.type=arr_allData->value[i_indexSet].type;
	tv_afterItem.value=arr_allData->value[i_indexSet].value;
	arr_allData->value[i_indexSet].type=typ_data;
	arr_allData->value[i_indexSet].value=v_data;

	// Hacemos que todos los elementos sean
	// Corridos
	for (U_INT i=i_indexSet+1;i<arr_allData->i;i++){
		//Intercambiamos:
		tv_nowItem.type=arr_allData->value[i].type;
		tv_nowItem.value=arr_allData->value[i].value;

		arr_allData->value[i].type=tv_afterItem.type;
		arr_allData->value[i].value=tv_afterItem.value;

		tv_afterItem.type=tv_nowItem.type;
		tv_afterItem.value=tv_nowItem.value;
	}
}
void setValue_tv(struct Var* vr_now,const char* s_name,struct type_value* tv_setVar){
	char* str;
	// Ya estaba definida. Ahora liberamos
	// para poder darle otro dato
	if (!s_name){
		delete_item(vr_now->type,vr_now->value);
	// Nueva variable:
	}else{
		vr_now->name=(char*)malloc(sizeof(s_name));
		strcpy(vr_now->name,s_name);
	}

	vr_now->type = tv_setVar->type;
	switch(vr_now->type){
		case INT:
			vr_now->value=malloc(sizeof(int));
			*(int*)vr_now->value=*(int*)tv_setVar->value;
			break;
		case FLOAT:
			vr_now->value=malloc(sizeof(double));
			*(double*)vr_now->value=*(double*)tv_setVar->value;
			break;
		case LONGINT:
			vr_now->value=malloc(sizeof(mpz_t));
			mpz_init(*(mpz_t*)vr_now->value);//Importante.
			mpz_set(*(mpz_t*)vr_now->value,*(mpz_t*)tv_setVar->value);
			break;
		case LONGFLOAT:
			vr_now->value=malloc(sizeof(mpf_t));
			mpf_init_set(*(mpf_t*)vr_now->value,*(mpf_t*)tv_setVar->value);
			break;
		case ARRAY:
			vr_now->value=copy_array((struct Array*)tv_setVar->value);
			break;
		case STRING:
		case CODES_BLOCKS:
			str=(char*)malloc(sizeof(char)*(strlen((char*)tv_setVar->value)+1));
			strcpy(str,(char*)tv_setVar->value);
			vr_now->value=str;
			break;
		case FUNCTION:break;//¡No librerar!
		default:
			printf("Error tipo %s no tratado en la función setValue_tv\n",get_name_type(tv_setVar->type) );
			exit(-3);
	}
}
void delete_var(struct Var* vr_var){
	delete_item(vr_var->type,vr_var->value);
	free(vr_var->name);
	vr_var->value=NULL;
}
void delete_item(const enum TYPE t_typValue, void* v_data){
	switch(t_typValue){
		case VAR:
			delete_var((struct Var*)v_data);
			free(v_data);
			break;
		case FUNCTION:break;// ¡No liberar!
		case ARRAY:
			delete_array((struct Array*)v_data);
			free(v_data);
		break;
		case LONGINT:
			mpz_clear(*(mpz_t*)v_data);
			free(v_data);
			break;
		case LONGFLOAT:
			mpf_clear(*(mpf_t*)v_data);
			free(v_data);
			break;
		case INT:
		case CODES_BLOCKS:
		case STRING:
		case FLOAT:
			free(v_data);
			break;
		default:
			perror("Error: Se intenta liberar un tipo de dato no tratado. \nFunción \"delete_item\" ");
			printf("Type: %s",get_name_type(t_typValue));
			exit(-130);
	}
}
char* interpret(struct Array* arr_allData,struct Array* arr_allVars,struct Var* vr_data){
	void* v_generic;
	switch (vr_data->type){
	case INT:
		v_generic=malloc(sizeof(int));
		*(int*)v_generic=*(int*)vr_data->value;
		break;
	case FLOAT:
		v_generic=malloc(sizeof(double));
		*(double*)v_generic=*(double*)vr_data->value;
		break;
	case FUNCTION: //Usamos funciones dentro del mismo programa para esta.
		((unsigned short (*)(struct Array* arr_allData,struct Array* vars,char* extend))vr_data->value)(arr_allData,arr_allVars,"");
		return NULL;
	case CODES_BLOCKS://Interpretamos el bloque de código.
		return (char*)vr_data->value;
	case STRING:
		v_generic=malloc(sizeof(char)*(strlen((char*)vr_data->value)+1));
		strcpy((char*)v_generic,(char*)vr_data->value);
		break;
	case ARRAY:
		v_generic=copy_array((struct Array*)vr_data->value);
		break;
	case LONGINT:
		v_generic=malloc(sizeof(mpz_t));
		mpz_init(*(mpz_t*)v_generic);
		mpz_set(*(mpz_t*)v_generic, *(mpz_t*)vr_data->value);
		break;
	case LONGFLOAT:
		v_generic=malloc(sizeof(mpf_t));
		mpf_init_set(*(mpf_t*)v_generic, *(mpf_t*)vr_data->value);
		break;
	default:
		printf("Error tipo %s no tratado en la función setValue_tv\n",get_name_type(vr_data->type) );
		exit(-3);
	}
	// Agregamos el dato ya copiado
	add_array(arr_allData,vr_data->type,v_generic);
	return NULL;
}
char* printf_stack(struct Array* arr_allData){
	char* s_out=(char*)malloc(sizeof(char)*2);// Iniciamos para despues agregarle.
	char* s_generic;//Uso general.
	*s_out='\0';
	char* s_format;
	unsigned int len=1;
	unsigned int len_str_return;
	mp_exp_t mp_exponent;
	for(unsigned int i=0;i<arr_allData->i;i++){
		switch(arr_allData->value[i].type){
			case INT:
				// Creamos una cadena temporal y guardamos el antero
				s_generic=(char*)alloca(50);
				sprintf(s_generic,"%d ",*(int*)arr_allData->value[i].value);

				len=append_strcpy(&s_out,len,s_generic);
				break;
			case FLOAT:
				// Creamos una cadena temporal y guardamos el flotante.
				s_generic=(char*)alloca(60);
				sprintf(s_generic,"%f ",*(double*)arr_allData->value[i].value);

				len=append_strcpy(&s_out,len,s_generic);
				break;
			case ARRAY:
				// Convertimos el array en cadena para despues concatenarlo
				s_generic=printf_stack((struct Array*)arr_allData->value[i].value);
				len=append_sprintf(
					&s_out,
					len,
					4,
					"[ %s] ",// 4 -> "[ %s] "
					s_generic
				);
				free(s_generic);
				break;
			case CODES_BLOCKS:
				len=append_sprintf(
					&s_out,
					len,
					3,// 3 -> "{} "
					"{%s} ",
					(char*)arr_allData->value[i].value
				);
				break;
			case STRING:
				len=append_sprintf(
					&s_out,
					len,
					3,// 3 -> '"" '
					"\"%s\" ",
					(char*)arr_allData->value[i].value
				);
				break;
			case LONGINT:
				// Obtener la cadena.
				s_generic=mpz_get_str(NULL,0,*(mpz_t*)arr_allData->value[i].value);
				
				// 1 por: " "
				len=append_sprintf(
					&s_out,
					len,
					1,// 1 -> ' '
					"%s ",
					s_generic
				);
				FREE__(s_generic);
				break;
			case LONGFLOAT:
			  // Aprovechamos que gmp ofrece funciones para 
				// Obtener la cadena.
				s_generic=mpf_get_str(NULL, &mp_exponent, 10, 16, *(mpf_t*)arr_allData->value[i].value);
				mp_exponent+=(*s_generic=='-')?1:0;//Si es negativo se necesita sumar 1.

				// 2 -> ' ' and ','
				len_str_return=strlen(s_generic)+2;
				s_out=(char*)realloc(s_out,SIZE_CHAR(len+len_str_return));

				// 18: 7->"%.s,%s "+{s_generic 10 character}+1->\0.
				s_format=(char*)alloca(SIZE_CHAR(18));
				sprintf(s_format,"%%.%ds,%%s ",(int)mp_exponent);
				//Out:
				sprintf(
					s_out+SIZE_CHAR(len-1),
					s_format,
					s_generic,
					s_generic+(int)mp_exponent
				);
				len+=len_str_return;
				FREE__(s_generic);
				break;
			default:
				printf("Error: Tipo de dato \"%s\" no está tratado.",get_name_type(arr_allData->value[i].type));
				break;
		}
	}
	return s_out;
}
int64_t search_var_init(const char* s_name, unsigned const int i_initStr, struct Array* arr_var){
	unsigned int i_name=0;
	for (long int i=strlen(s_name)-1;i>-1;i--)
		i_name+=s_name[i];
	for (int64_t i=0;i<arr_var->i;i++){
		const struct Var* vr_varNow=(struct Var*)arr_var->value[i].value;//Variable actual.
		if (i_name==vr_varNow->i_name && !strcmp(&s_name[i_initStr],vr_varNow->name))//Es igual y retornamos.
			return i;
	}
	return -1;
}
void add_var(struct Array* arr_var,const char* s_name,enum TYPE typ_data,void* v_data){
	struct Var* vr_now=(struct Var*)malloc(sizeof(struct Var));
	const unsigned int LEN=strlen(s_name);
	//Aqui da error de memoria:
	vr_now->name=(char*)malloc(sizeof(char)*(LEN+1));
	if (vr_now->name==NULL){
		PRINTF_MEMORY_ERROR(s_name);
		return;
	}
	vr_now->i_name=0;

	for (unsigned int i=0;s_name[i]!='\0';i++){
		vr_now->name[i]=s_name[i];
		vr_now->i_name+=s_name[i];// Para saber la sumatoria de carácteres.
	}
	vr_now->name[LEN]='\0';
	vr_now->type=typ_data;
	vr_now->value=v_data;
	add_array(arr_var,VAR,vr_now);
}
char* to_string_value(const enum TYPE typ_data,void* v_data){
	/**
	 * @todo Hacer que prinft_stack use esta funcion, tambien 
	 * hacer que la funcion reciba un int* para enseñar la longitud de la cadena.
	 * Tambien cambiar los nombres: to_string_value -> toStringValue
	 */
	char* s_out=NULL,
	*s_generic=NULL;
	mp_exp_t mp_exponent;
	void* vTmp;

	switch(typ_data){
		case INT:
			s_generic=(char*)alloca(30);
			itoa(*(int*)v_data,(char*)s_generic,10); 
			
			s_out=(char*)malloc(sizeof(char)*(strlen(s_generic)+1));
			strcpy(s_out,s_generic);
			break;
		case FLOAT:
			s_generic=(char*)alloca(30);
			sprintf(s_generic, "%.*lf", CLIMIT_FLOAT-2,*(double*)v_data);
			
			s_out=(char*)malloc(sizeof(char)*(strlen(s_generic)+1));
			strcpy(s_out,s_generic);
			break;
		case LONGINT:
			//Evitamos warning con los tester al no hacer: s_out=mpz_get_str(...);
			//2-> '-' is negative and '\0'
			s_out=(char*)malloc( SIZE_CHAR(mpz_sizeinbase((mpz_t*)v_data, 10) + 2) );
			mpz_get_str(s_out,10,*(mpz_t*)v_data);
			break;
		case LONGFLOAT:
			s_generic=mpf_get_str(NULL, &mp_exponent, 10, 0, *(mpf_t*)v_data);

			// 2 por: '\0' y ','
			s_out=(char*)malloc(strlen(s_generic)+2);

			// 19 -> 7 de "%.s,%s " + 10->2147483648 (10 caracteres)+ 1 -> \0+ 1 -> '-'.
			vTmp=alloca(sizeof(char)*19);
			sprintf(vTmp,"%%.%ds,%%s ",(int)mp_exponent);
			
			sprintf(s_out,vTmp,s_generic,s_generic+(int)mp_exponent);
			FREE__(s_generic);
		break;
		case CODES_BLOCKS:
		case STRING:
			s_out=(char*)malloc(sizeof(char)*(strlen((char*)v_data)+1));
			strcpy(s_out,v_data);
			break;
		case ARRAY:
			s_out=printf_stack((struct Array*)v_data);
			break;
		case FUNCTION:
			s_generic="(Native-Function)";
			s_out=(char*)malloc(sizeof(char)*(strlen(s_generic)+1));
			strcpy(s_out,s_generic);
			break;
		default:
			return NULL;
	}
	return s_out;
}
#endif