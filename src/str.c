#ifndef STR_C
#define STR_C 1
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include "./include/define.h"
#include "./include/str.h"
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
	for(;i<i_end;i++){
		if (str[i]==type AND !is_scape)//Si es termino y no es un escape.
			return i;//Conseguimos el final de la cadena por lo que terminamos.
		is_scape=(str[i]=='\\' && !is_scape);
	}
	return 0;//No hay fin, por lo que retornamos 0
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
    if (str_d->count+len+1>=str_d->max){
        str_d->str=(char*)realloc(str_d->str,sizeof(char)*(str_d->max+=len+1));
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
/**
 * Funcion para identificar los tipos de datos.
 * @param  t Tipo
 * @return   Retorna una cadena no dinamica. No requiere liberar
 */
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
			if (*c=='"')//Como no tiene un scape entonces la cadena se mostrara sin ""
				continue;
			new_str[i_nstr++]=*c;
		}
	}
	new_str[i_nstr]='\0';
	new_str=(char*)realloc(new_str,i_nstr+1);
	return new_str;
}
/**
 * Funcion para verificar si la cadena necesita mas memoria para agregarle un char.
 * Nota leftover porque reserva mas memoria que la que necesita.
 * @param str_ Structura String.
 * @param c { char a agregar }
 */
void cadd_add_leftover(struct String* str_,char c){
	if (str_->count+3>=str_->max){
		str_->str=(char*)realloc(str_->str,str_->max+=20);
	}
	str_->str[str_->count++]=c;
}
/**
 * Funcion que retorna una cadena hechas con comillas dobles.
 * Si fue comillas simple lo que hace es escapar todas las comillas
 * dobles para que esto: '"hola' pase a esto: "\"hola"
 * @param  str_ Cadena con comillas dobles.
 * @param  init Donde inicia las comillas(Toda la evaluacion se hara deacuerdo a esto).
 * @param  end  Final de la cadena. Si se ingresa 0 se determina el tamaño dentro de la funcion.
 * @return      Cadena dinamica, recuerda liberar.
 */
char* get_sub_str(const char* str,U_INT init, U_INT end){
	NEW_STRING(out,20);
	U_INT end_c=(end)?end:strlen(str);
	char type=str[init++],is_scape=FALSE;
	out.str[out.count++]='"';
	for (int i=init;i<end_c;i++){
		if (str[i]==type AND !is_scape)//Si es termino y no es un escape.
			break;//Conseguimos el final de la cadena por lo que terminamos.
		if (type=='\'' && str[i]=='"')
			cadd_add_leftover(&out,'\\');
		cadd_add_leftover(&out,str[i]);
		is_scape=(str[i]=='\\' && !is_scape);
	}
	cadd_add_leftover(&out,'"');
	cadd_add_leftover(&out,'\0');
	return (char*)realloc(out.str,out.count);
}
#endif
