#ifndef STR_H
#define STR_H 1
#include "./define.h"

/**
 * @brief Retornar el tamaño del la cadena.
 * @param str const char* cadena a retornar su tamaño.
 * @return unsigned int.
*/
unsigned int len(const char* str);
/**
 * @brief Funcion que busca la subcadena dentro de otra cadena, si str_1 < str_2 entonces str_1 es la sub-cadena. Si son iguales entonces solo compara las cadenas.
 * @param str_1 char* -- Cadena 1.
 * @param str_2 char* -- Cadena 2.
 * @return bool
*/
bool compare_sub_str(char* str_1,char* str_2);
/**
 * @brief Función que busca un char dentro de una cadena.
 * 
 * @param str  Cadena de donde buscar.
 * @param c Char que vamos a buscar.
 * @return true 
 * @return false 
 */
bool compare_sub_str(const char* str,unsigned const char c);
/**
 * @brief Vemos si el char es una letra(A-Z o a-z).
 * @param c char a comprobar
 * @return bool
**/
bool is_abc(unsigned const char c);
/**
 * @brief Vemos si el char se puede transformar en entero.
 * @param c char a comprobar
 * @return bool
**/
bool is_num(unsigned const char c);
/**
 * @brief Get the end of str.
 * 
 * @param str cadena donde buscar.
 * @param init Posición del inicio de la subcadena.
 * @return unsigned int end.
 */
unsigned int get_end_str(const char* str,unsigned const int init,unsigned int i_end=0);
#endif