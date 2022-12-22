#ifndef STR_H
#define STR_H 1
#include <string.h>
#include "./define.h"
struct String{
    unsigned int max;
    unsigned int count;
    char* str;
};
/**
 * @brief Funcion que busca la subcadena dentro de otra cadena, si str_1 < str_2 entonces str_1 es la sub-cadena. Si son iguales entonces solo compara las cadenas.
 * @param str_1 char* -- Cadena 1.
 * @param str_2 char* -- Cadena 2.
 * @return bool
*/
unsigned short compare_sub_str(char* str_1,char* str_2);
/**
 * @brief Función que busca un char dentro de una cadena.
 * 
 * @param str  Cadena de donde buscar.
 * @param c Char que vamos a buscar.
 * @return true 
 * @return false 
 */
unsigned short search_char(const char* str,unsigned const char c);
/**
 * @brief Vemos si el char es una letra(A-Z o a-z).
 * @param c char a comprobar
 * @return bool
**/
unsigned short is_abc(unsigned const char c);
/**
 * @brief Vemos si el char se puede transformar en entero.
 * @param c char a comprobar
 * @return bool
**/
unsigned short is_num(unsigned const char c);
/**
 * @brief Get the end of str.
 * 
 * @param str cadena donde buscar.
 * @param init Posición del inicio de la subcadena.
 * @param i_end limite de la cadena. Si se coloca 0 se buscará hasta el final de la cadena.
 * @return unsigned int end si retorna 0 entonces no se consiguió el final.
 */
unsigned int get_end_str(const char* str,unsigned const int init,unsigned int i_end);
/**
 * @brief Funcion que transforma de string a entero.
 * 
 * @param str String que representa los numeros
 * @return int 
 */
int parseInt(const char* str);
/**
 * @brief Función que concatena dos cadena usando la estructura de String.
 * 
 * @param str_d Estructura con la cadena destino.
 * @param str_copy Cadena a copiar.
 * @param init Inicio de la cadena a copiar.
 * @param end Final de la cadena a copiar. Si es 0 se buscará el tamaño.
*/
void str_add_str_init_end(struct String* str_d,const char* str_copy,unsigned int init,unsigned int end);
#define str_add_str(x,y) str_add_str_init_end(x,y,0,0)
/**
 * @brief Solo iniciamos la estructura String.
 * 
 * @param str 
 * @param buffer 
 */
void init_str(struct String* str,unsigned int buffer);
#endif