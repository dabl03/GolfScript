#ifndef STR_CPP
#define STR_CPP 1
#include <string.h>
#include <stdlib.h>
#include <gmp.h>
#include "./include/define.h"
#include "./include/str.h"

/**
 * @brief Funcion que busca la subcadena dentro de otra cadena, si str_1 < str_2 entonces str_1 es la sub-cadena. Si son iguales entonces solo compara las cadenas.
 * @param str_1 char* -- Cadena 1.
 * @param str_2 char* -- Cadena 2.
 * @return bool
*/
unsigned short compare_sub_str(char* str_1,char* str_2){
	unsigned int i_s1=strlen(str_1);
	unsigned int i_s2=strlen(str_2);
	unsigned int c=0;//Veces que hubo coincidencia.
	char* c1=str_1;
	char* c2=str_2;
	if (i_s1==i_s2){// Como son iguales no tenemos porque buscar una sub-cadena, solo debemos ver si son iguales.
		for (unsigned int i=0;i<i_s1;i++){
			if (c1[i]!=c2[i])
				return 0;
		}
		return 1;
	}else if(i_s1<i_s2){//Si la cadena 1 es menos que la cadena 2 entonces la cadena 1 es la sub cadena que debemos buscar en la cadena 2.
		c1=str_2;
		c2=str_1;
		c=i_s1;
		i_s1=i_s2;
		i_s2=c;
		c=0;
	}
	for (unsigned int i=0;i<i_s1;i++){
		if (c1[i]==c2[c]){//Vemos si la sub cadena se encuentra dentro de la cadena.
			c++;
			if (c==i_s2)//Si se encuentra.
				return 1;
		}else
			c=0;
	}
	return 0;//no se encuentra.
}
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
void str_add_char(struct String* str_d,char c){
	if (str_d->count+1>=str_d->max){
		str_d->str=(char*)realloc(str_d->str,sizeof(char)*(str_d->max+=1));
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
#endif