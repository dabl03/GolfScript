#ifndef STR_C
#define STR_C 1
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include "./include/define.h"
#include "./include/str.h"
/*********{TODO}:{
	Hacer una funcion que escape y descape una cadena para ponerlo en el visualizador de pila.
}
	
*/
/**
 * @brief Función que busca un char dentro de una cadena.
 * 
 * @param str  Cadena de donde buscar.
 * @param c Char que vamos a buscar.
 * @return true 
 * @return false 
 */
unsigned short search_char(const char* str,unsigned const char c){
	for (unsigned int i=0;str[i]!='\0';i++)
		if ((unsigned int)str[i]==c)
			return 1;//Se encontró:)
	return 0;//No se encontro el char:(
}
/**
 * @brief Vemos si el char es una letra(A-Z o a-z).
 * @param c char a comprobar
 * @return bool
**/
unsigned short is_abc(unsigned const char c){
    return ABC(c);
}
/**
 * @brief Vemos si el char se puede transformar en entero.
 * @param c char a comprobar
 * @return bool
**/
unsigned short is_num(unsigned const char c){
	return (c>='0' && c<='9');
}
/**
 * @brief Get the end of str.
 * 
 * @param str cadena donde buscar.
 * @param init Posición del inicio de la subcadena.
 * @param i_end limite de la cadena. Si se coloca 0 se buscará hasta el final de la cadena.
 * @return unsigned int end si retorna 0 entonces no se consiguió el final.
 */
unsigned int get_end_str(const char* str,unsigned const int init,unsigned int i_end){
	unsigned int i=init;
	unsigned short is_scape=0;
	unsigned const char type=str[i++];//Almacenamos el tipo de cadena para saber si hay un signo de escape+type, y si anida otro tipo de cadena lo tratamos como una subcadena.
	i_end=(!i_end)?strlen(str):i_end;//Asi aprovechamos si se paso el limite de la cadena.
	if (i==i_end){
		return i;
	}else if(i>i_end){
		return 0;
	}
	for(;i<i_end;i++){
		if (str[i]==type){//Type debe ser ' "
			if (!is_scape){//No es parte de la subcadena.
				return i;//Conseguimos el final de la cadena por lo que terminamos.
			}
		}
		is_scape=(str[i]=='\\' && !is_scape);
	}
	return init;//No hay fin, por lo que retornamos el inicio.
}
/**
 * @brief Funcion que transforma de string a entero.
 * 
 * @param str String que representa los numeros
 * @return int 
 */
int parseInt(const char* str){
    unsigned short is_negative=(str[0]=='-');
    int output=0;
    for (int i=is_negative;str[i]!='\0';i++){
        if (is_num(str[i])){
            output=(output*10)+CHAR_TO_NUM(str[i]);
        }
    }
    return output;
}
/**
 * @brief Mismo que parseInt pero para enteros long int.
 * 
 * @param str Numeros.
 * @return long int 
 */
long int parseLongInt(const char* str){
    unsigned short is_negative=(str[0]=='-');
    long int output=0;
    for (int i=is_negative;str[i]!='\0';i++){
        if (is_num(str[i])){
            output=(output*10)+CHAR_TO_NUM(str[i]);
        }
    }
    return output;
}
/*struct type_value* get_int(const char* str){
	unsigned int len=strlen(str);
	if (len>=10){
		
	}
	return NULL;
}*/
/**
 * @brief Función que concatena dos cadena usando la estructura de String.
 * 
 * @param str_d Estructura con la cadena destino.
 * @param str_copy Cadena a copiar.
 * @param init Inicio de la cadena a copiar.
 * @param end Final de la cadena a copiar. Si es 0 se buscará el tamaño.
*/
void str_add_str_init_end(struct String* str_d,const char* str_copy,unsigned int init,unsigned int end){
	end=(end)?end:strlen(str_copy);
	unsigned int len=end-init;
    if (str_d->count+len>=str_d->max){
        str_d->str=(char*)realloc(str_d->str,sizeof(char)*(str_d->max+=len));
    }
    unsigned int i=init;
	char* c=&str_copy[i];
    for (;i<end && *c!='\0';i++){
        str_d->str[str_d->count++]=*c;
		c++;
    }
    str_d->str[str_d->count]='\0';
}
/**
 * @brief Agrega un char al final de la cadena.
 * 
 * @param str_d 
 * @param c 
 */
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
/**
 * @brief Solo iniciamos la estructura String.
 * 
 * @param str 
 * @param buffer 
 */
void init_str(struct String* str,unsigned int buffer){
	str->str=(char*)malloc(sizeof(char)*buffer);
	str->count=0;
	str->max=buffer;
}
/**
 * Escapa un caracter.
 * @param  c Caracter normal(no especial)
 * @return   Si el caracter es uno especial se escapa(n->\n, b->\b,etc), sino es un caracter espercial se retorna -1
 */
char scape_char(const char c){
	switch (c)
	{
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
	default:
		return -1;
	}
}
/**
 * Transforma los caracteres especiales en su equivalente sin \.
 * @param  c Caracter
 * @return   Retorna el caracter normal(\n->n, \a->a, etc...), sino es un caracter especial retorna -1.
 */
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
/*void scape_str(char* out,const char* io){
	unsigned int len=strlen(io);
	if(!len)
		return;
	for (unsigned int i=0;i<len;i++){
		char c=io[i];
		if (c=='\\'){
			c=scape_char(io[++i]);
			if (c==-1){
				continue;
			}
		}
		*out=c;
		out++;
	}
}*/
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
		case PCHAR:
			return "no se pchar";
		case CHAR:
			return "CHAR";
		case VALUE_TYPE:
			return "VALUE_TYPE";
        /*Usado por el interprete:*/
        case FUNCTION:
			return "(NATIVE FUNCTION)";
		case VAR:
			return "(VAR)";
		case CODES_BLOCKS:
			return "(CODES BLOCKS)";
		case ARRAY:
			return "(ARRAY)";
		default:
			return NULL;
	}
}
/**Tratar la cadena para mostrar todos los caracteres especiales sin escapar al momento de hacer printf.
 * @param[char*] old_str { Cadena a escapar }
 * @return  nueva cadena reservando memoria dinamica. Recueda usar free.
*/
char* get_str_nescp(char* old_str){
	U_INT len=strlen(old_str),
	i_nstr=0;
	char* new_str=(char*)malloc(len);//Tamaño base.
	char chr;
	for (char* c=old_str;*c!='\0';c++){
		if (i_nstr+3>=len){//Necesitamos mas memoria para esta cadena.
			new_str=(char*)realloc(new_str,len=i_nstr+20);
		}
		chr=nscape_char(*c);
		if (chr==-1)//No es un caracter especial.
			new_str[i_nstr++]=*c;
		else{
			new_str[i_nstr++]='\\';
			new_str[i_nstr++]=chr;
		}
	}
	new_str[i_nstr]='\0';
	new_str=(char*)realloc(new_str,i_nstr+1);
	return new_str;
}
/**Tratar la cadena para escapar todos los caracteres especiales.
 * @param[char*] old_str { Cadena a escapar }
 * @return  nueva cadena reservando memoria dinamica. Recueda usar free.
*/
char* get_str_escp(char* old_str){
	U_INT len=strlen(old_str),
	i_nstr=0;
	char* new_str=(char*)malloc(len);//Tamaño base.
	char is_scape=FALSE,chr=0;
	for (char* c=old_str;*c!='\0';c++){
		if (i_nstr+3>=len){
			new_str=(char*)realloc(new_str,len=i_nstr+20);
		}
		if (*c=='\\' AND NOT is_scape){
			is_scape=TRUE;
			continue;
		}else if (is_scape){
			chr=scape_char(*c);
			if (chr==-1){
				new_str[i_nstr++]=*c;
			}else{
				new_str[i_nstr++]=chr;
			}
			is_scape=FALSE;
			continue;
		}else{
			new_str[i_nstr++]=*c;
		}
	}
	new_str[i_nstr]='\0';
	new_str=(char*)realloc(new_str,i_nstr+1);
	return new_str;
}
#endif
