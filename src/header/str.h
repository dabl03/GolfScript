#ifndef STR_H
#define STR_H 1
#include <string.h>
#include <gmp.h>
#include <limits.h>
#include <stdio.h>
#include "./define.h"
/**
 * @param max Maximo caracteres a insertar.
 * @param count caracteres actuales.
 * @param str cadena.
 * @Note: Recordar iniciar todo en NULL o que str debe tener '\0'
*/
struct String{
	uint max;
	uint count;
	char* str;
};
/**
 * @brief Función que busca un char dentro de una cadena.
 * 
 * @param str  Cadena de donde buscar.
 * @param c Char que vamos a buscar.
 * @return true 
 * @return false 
 */
unsigned short exist_char(const char* str,unsigned const char c);
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
/**Get the end of str.
 *  
 * @param str cadena donde buscar.
 * @param i_end limite de la cadena.
 ** Si se coloca 0 se buscará hasta el final de la cadena.
 *
 * @return uint end si retorna 0 entonces no se consiguió el final.
 */
uint get_end_str(const char* str, uint i_end);
/**
 * @brief Funcion que transforma de string a entero.
 * 
 * @param str String que representa los numeros
 * @return int 
 */
int parseInt(const char* str);
/**
 * @brief Mismo que parseInt pero para enteros long int.
 * 
 * @param str Numeros.
 * @return long int 
 */
long int parseLongInt(const char* str);
/**
 * @brief Función que concatena dos cadena usando la estructura de String.
 *  
 * @param str_d Estructura con la cadena destino.
 * @param str_copy Cadena a copiar.
 * @param end Cantidad de caracteres a copiar. Si es -1 se buscará el tamaño.
*/
void str_add_str_end(struct String* str_d,  const char* str_copy, int64_t count);
#define str_add_str(str_d, str_copy) str_add_str_end(str_d, str_copy, -1)
#define str_add_str_init_end(str_d,  str_copy, init,end) str_add_str_end(str_d,str_copy+init,end)
/**
 * @brief Agrega un char al final de la cadena.
 * 
 * @param str_d 
 * @param c 
 */
void str_add_char(struct String* str_d,const char c);
/**
 * @brief Solo iniciamos la estructura String.
 * 
 * @param str 
 * @param buffer 
 */
void init_str(struct String* str,uint buffer);
/**
 * Escapa un caracter.
 * @param  c Caracter normal(no especial)
 * @return   Si el caracter es uno especial se escapa(n->\n, b->\b,etc), sino es un caracter espercial se retorna -1
 */
char scape_char(const char c);
/**
 * Transforma los caracteres especiales en su equivalente sin \.
 * @param  c Caracter
 * @return   Retorna el caracter normal(\n->n, \a->a, etc...), sino es un caracter especial retorna -1.
 */
char nscape_char(char c);

/// Retorna True si chr es un caracter
///          invisible.
bool is_invisible_char(unsigned char chr);
/**Tratar la cadena para mostrar todos los caracteres especiales sin escapar al momento de hacer printf.
 * @param[char*] old_str { Cadena a escapar }
 * @return  nueva cadena reservando memoria dinamica. Recueda usar free.
*/
char* get_str_nescp(char* old_str);
/**
 * Tratar la cadena para escapar todos los caracteres especiales.
 * Nota: Hecha para escapar la entrada del usuario.
 * @param[char*] old_str { Cadena a escapar }
 * @return  nueva cadena reservando memoria dinamica. Recueda usar free.
*/
char* get_str_escp(char* old_str);
/**
 * Funcion para verificar si la cadena necesita mas memoria para agregarle un char.
 * Nota leftover porque reserva mas memoria que la que necesita.
 * @param str_ Structura String.
 * @param c { char a agregar }
 */
void cadd_leftover(struct String* str_,const char c);
/**
 * Funcion que retorna una cadena hechas con comillas dobles.
 * Si fue comillas simple lo que hace es escapar todas las comillas
 * dobles para que esto: '"hola' pase a esto: "\"hola"
 * @param  str_ Cadena con comillas dobles.
 * @param  init Donde inicia las comillas(Toda la evaluacion se hara deacuerdo a esto).
 * @param  end  Final de la cadena. Si se ingresa 0 se determina el tamaño dentro de la funcion.
 * @return      Cadena dinamica, recuerda liberar.
 */
char* get_sub_str(const char* str,uint init, uint end);
/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 * @param value int. El entero a convertir.
 * @param result char*. Donde se guardara.
 * @param base int. Que reprecentación queremos.
*/
char* itoa(int value, char* result, int base);
/**
 * Funcion para identificar los tipos de datos.
 * @param  t Tipo
 * @return   Retorna una cadena no dinamica. No requiere liberar
 */
const char* get_name_type(enum TYPE t);
/**Copies a string and returns one stored in dynamic memory.
 * @param str_io String to copy
 * @return Copy the string saved with malloc.
*/

char* copy_str(const char* str_io);

/**
 * Functions to convert the type to a string. Each type is
 * indicated in the function prefix, and will be used by
 * the program in the array CONVERT_TO_STRING_FNC[TYPE].
 * 
 * @param tv_element The element stored in a type_value
 *  structure. NOTE: For speed, it is not checked whether
 *  the type is the same as the one the function
 *  corresponds to.
 * @param[in] out_len Where to store the string length.
 *  If NULL is passed, nothing is done with the parameter.
 * @return The string that represents the type.
 *  Note: It should be free, but if you are debugging or testing,
 *  you should use FREE__() in long_int_to_string
*/
char* int_to_string(const struct type_value* tv_element, uint* out_len);
char* long_float_to_string(const struct type_value* tv_element, uint* out_len);
char* float_to_string(const struct type_value* tv_element, uint* out_len);
char* long_int_to_string(const struct type_value* tv_element, uint* out_len);
char* block_codes_to_string(const struct type_value* tv_element, uint* out_len);
char* str_to_string(const struct type_value* tv_element, uint* out_len);
char* stack_to_string(const struct type_value* tv_element, uint* out_len);
char* var_to_string(const struct type_value* tv_element, uint* out_len);
char* FUNCTION_to_string(const struct type_value* tv_element, uint* out_len);
char* NONE_to_string(const struct type_value* tv_element, uint* out_len);
char* tv_to_string(const struct type_value* tv_element, uint* out_len);
char* to_string(const enum TYPE type, const void* value, uint* out_len);
#endif