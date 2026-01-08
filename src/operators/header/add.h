#ifndef ADD_H
#define ADD_H
#include "../../header/str.h"
#include "../../header/stack.h"
#include "../../header/global_vars.h"
#include "../../header/define.h"
/***
 * @Brief      Adds an integer.
 *
 * @Param num      The number
 * @Param type_n2  The type n 2
 * @Param num_2    The number 2
 * @Param is_right  It allows you to determine whether to 
 * place it at the beginning of the array (stack, code block, string)
 *  or at the end. If true, it goes at the beginning.
 *
 * @return tatic struct type_value_err* -  Result. 
 * Note: The result does not require releasing the structure.
 */
struct type_value_err* add_int(int num, enum TYPE type_n2, void* num_2, bool is_right);
/**
 * Agrega algo al bloque de codigo
 * @param codes    Bloque de codigo
 * @param t        Tipo de objeto agregar.
 * @param value    valor
 * @Param is_right  It allows you to determine whether to 
 * place it at the beginning of the array (stack, code block, string)
 *  or at the end. If true, it goes at the beginning.
 * @return Char* - Recordar liberar.
 */
char* add_codes_block(char* codes,enum TYPE t, void* value, bool is_right);
/**
 * Suma un entero grande con algo
 * @param  arr    Array
 * @param  t        Tipo de objeto agregar.
 * @param  value    valor
 * @return          struct type_value_err* . Nuevo objeto array. No es necesario liberar.
 */
struct type_value_err* add_longint(mpz_t* long_int,enum TYPE t, void* value);
/**
 * @brief      Concatena str con tipo de dato.
 *
 * @param str          The string
 * @param t        parameter_description
 * @param value        The value
 * @param is_right Es para saber si poner el resultado a la derecha.
 *
 * @return static struct type_value_err* -  El resultado, no se requiere liberar la estructura.
*/
struct type_value_err* add_str(char* str,enum TYPE t, void* value,bool is_right);
/**Operator add array: Suma un dato con un array
 * @param h_stack La pila actual.
 * @param t El tipo de dato a evaluar.
 * @param value El dato.
 * @return &static struct type_value_err. 
 ** Si type_value_err->type=none para indicar
 ** que no se debe liberar value porque se usa en el nuevo array.
 ** Ojo si se pasa array despues de copiar se hace free(((struct Array*)value)->value);
*/
struct type_value_err* opr_add_array(struct Header_Stack* h_stack,enum TYPE t, void* value);
#endif