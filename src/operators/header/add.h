#ifndef OPERATOR_H
#define OPERATOR_H
#include "../../header/str.h"
#include "../../header/stack.h"
#include "../../header/global_vars.h"
#include "../../header/define.h"
/**
 * @brief      Adds an integer.
 *
 * @param num      The number
 * @param type_n2  The type n 2
 * @param num_2    The number 2
 * @param is_right Solo para array, blockes codes, y cadena.
 ** Determina en que posicion se le agrega el entero(principio o final)
 ** True para primero se coloca el entero y despues la cadena.
 *
 * @return tatic struct type_valur* -  El resultado, no se requiere liberar la estructura.
 */
struct type_value* add_int(int num,enum TYPE type_n2,void* num_2,bool is_right);
/**
 * Agrega algo al bloque de codigo
 * @param codes    Bloque de codigo
 * @param t        Tipo de objeto agregar.
 * @param value    valor
 * @param is_right Si se agregara al inicio o final
 * @return Char* - Recordar liberar.
 */
char* add_codes_block(char* codes,enum TYPE t, void* value,bool is_right);
/**
 * Suma un entero grande con algo
 * @param  arr    Array
 * @param  t        Tipo de objeto agregar.
 * @param  value    valor
 * @param  is_right Si se agregara al inicio o final
 * @return          struct type_value* . Nuevo objeto array. No es necesario liberar.
 */
struct type_value* add_longint(mpz_t* long_int,enum TYPE t, void* value,bool is_right);
/**
 * @brief      Concatena str con tipo de dato.
 *
 * @param str          The string
 * @param[in] t        parameter_description
 * @param value        The value
 * @param[in] is_right Indicates if right
 *
 * @return static struct type_value* -  El resultado, no se requiere liberar la estructura.
*/
struct type_value* add_str(char* str,enum TYPE t, void* value,bool is_right);
#endif