#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H
#include "./str.h"
#include "./stack.h"
#include "./global_vars.h"
#include "./define.h"
/**
 * @brief      Adds an integer.
 *
 * @param[in]  num      The number
 * @param[in]  type_n2  The type n 2
 * @param      num_2    The number 2
 * @param[] is_right    Solo para array, blockes codes, y cadena.
 ** Determina en que posicion se le agrega el entero(principio o final)
 ** True para primero se coloca el entero y despues la cadena.
 *
 * @return     struct type_valur* . Nuevo objeto. No es necesario liberar
 */
struct type_value* add_int(int num,enum TYPE type_n2,void* num_2,char is_right);
/**
 * Agrega algo al bloque de codigo
 * @param  codes    Bloque de codigo
 * @param  t        Tipo de objeto agregar.
 * @param  value    valor
 * @param  is_right Si se agregara al inicio o final
 * @return          Char* . Recordar liberar.
 */
char* add_codes_block(char* codes,enum TYPE t, void* value,char is_right);
/**
 * Suma un entero grande con algo
 * @param  arr    Array
 * @param  t        Tipo de objeto agregar.
 * @param  value    valor
 * @param  is_right Si se agregara al inicio o final
 * @return          struct type_value* . Nuevo objeto array. No es necesario liberar.
 */
struct type_value* add_longint(mpz_t* long_int,enum TYPE t, void* value,char is_right);
#endif