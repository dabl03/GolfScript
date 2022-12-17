#ifndef STR_CPP
#define STR_CPP 1
#include "./include/define.h"

/**
 * @brief Retornar el tamaño del la cadena.
 * @param str const char* cadena a retornar su tamaño.
 * @return unsigned int.
*/
unsigned int len(const char* str){
	unsigned int i=0;
	while(str[i]!='\0')
		i++;
	return i;
}
/**
 * @brief Funcion que busca la subcadena dentro de otra cadena, si str_1 < str_2 entonces str_1 es la sub-cadena. Si son iguales entonces solo compara las cadenas.
 * @param str_1 char* -- Cadena 1.
 * @param str_2 char* -- Cadena 2.
 * @return bool
*/
bool compare_sub_str(char* str_1,char* str_2){
	unsigned int i_s1=len(str_1);
	unsigned int i_s2=len(str_2);
	unsigned int c=0;//Veces que hubo coincidencia.
	char* c1=str_1;
	char* c2=str_2;
	if (i_s1==i_s2){// Como son iguales no tenemos porque buscar una sub-cadena, solo debemos ver si son iguales.
		for (unsigned int i=0;i<i_s1;i++){
			if (c1[i]!=c2[i])
				return false;
		}
		return true;
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
				return true;
		}else
			c=0;
	}
	return false;//no se encuentra.
}
/**
 * @brief Función que busca un char dentro de una cadena.
 * 
 * @param str  Cadena de donde buscar.
 * @param c Char que vamos a buscar.
 * @return true 
 * @return false 
 */
bool compare_sub_str(const char* str,unsigned const char c){
	for (unsigned int i=0;str[i]!='\0';i++)
		if ((unsigned int)str[i]==c)
			return true;//Se encontró:)
	return false;//No se encontro el char:(
}
/**
 * @brief Vemos si el char es una letra(A-Z o a-z).
 * @param c char a comprobar
 * @return bool
**/
bool is_abc(unsigned const char c){
    return ABC(c);
}
/**
 * @brief Vemos si el char se puede transformar en entero.
 * @param c char a comprobar
 * @return bool
**/
bool is_num(unsigned const char c){
	return (c>='0' && c<='9');
}
/**
 * @brief Get the end of str.
 * 
 * @param str cadena donde buscar.
 * @param init Posición del inicio de la subcadena.
 * @return unsigned int end.
 */
unsigned int get_end_str(const char* str,unsigned const int init,unsigned int i_end=0){
	unsigned int i=init;
	bool is_scape=false;
	unsigned const char type=str[i++];//Almacenamos el tipo de cadena para saber si hay un signo de escape+type, y si anida otro tipo de cadena lo tratamos como una subcadena.
	i_end=(!i_end)?len(str):i_end;//Asi aprovechamos si se paso el limite de la cadena.
	if (i==i_end){
		return i;
	}else if(i>i_end){
		err_msg="Error: Indice es mayor que la cadena.";
		throw err_msg;
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
#endif