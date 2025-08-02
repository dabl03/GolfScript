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
*/
struct String{
  unsigned int max;
  unsigned int count;
  char* str;
};
#define INIT_STRING(str___,max_____) str___.str=(char*)malloc(max_____);\
  str___.max=max_____; \
  str___.count=0;
#define NEW_STRING(str___,max_____) struct String str___={max_____,0,(char*)malloc(max_____)}
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
/**Get the end of str.
 *  
 * @param str cadena donde buscar.
 * @param i_end limite de la cadena.
 ** Si se coloca 0 se buscará hasta el final de la cadena.
 *
 * @return unsigned int end si retorna 0 entonces no se consiguió el final.
 */
unsigned int get_end_str(const char* str, U_INT i_end);
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
 * @param end Final de la cadena a copiar. Si es 0 se buscará el tamaño.
*/
void str_add_str_end(struct String* str_d, const char* str_copy, unsigned int end);
#define str_add_str(str_d, str_copy) str_add_str_end(str_d,str_copy,0)
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
void init_str(struct String* str,unsigned int buffer);
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
/**
 * Funcion para identificar los tipos de datos.
 * @param  t Tipo
 * @return   Retorna una cadena no dinamica. No requiere liberar
 */
const char* get_name_type(enum TYPE t);
/**Tratar la cadena para mostrar todos los caracteres especiales sin escapar al momento de hacer printf.
 * @param[char*] old_str { Cadena a escapar }
 * @return  nueva cadena reservando memoria dinamica. Recueda usar free.
*/
char* get_str_nescp(char* old_str);
/**Tratar la cadena para escapar todos los caracteres especiales.
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
char* get_sub_str(const char* str,U_INT init, U_INT end);
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
 * We take advantage of the fact that we know the end of the string
 * so we don't have to copy and add the string
 * with the desired format.
 * @param out char*. String to append the string to.
 * @param len_out int. Size of the original string (Note: The \0 is counted).
 * @param len_add_format int. Extra size to add the substring to.
 * @param format char*. Format to use in sprintf.
 * @param str_add char*. String to add.
 * @return unsigned int The final size of the new string.
*/
unsigned int append_sprintf(char** out,const unsigned int len_out,const unsigned int len_add_format,const char* format,const char* str_add);
/**
 * We take advantage of knowing the end of the string
 * to add the substring at the end.
 * @param str_out char*. Where to save the substring.
 * @param len int. Size of the original string.
 * @param str_io char*. String to add
 * @return unsigned int The final size of the new string.
*/
unsigned int append_strcpy(char** str_out,const unsigned int len,const char* str_io);
/**Copies a string and returns one stored in dynamic memory.
 * @param str_io String to copy
 * @return Copy the string saved with malloc.
*/
char* convert_static_str_to_dynamic(const char* str_io);
#endif