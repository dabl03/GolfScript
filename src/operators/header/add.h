#ifndef ADD_H
#define ADD_H
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
 *
 * @return tatic struct type_valur* -  El resultado, no se requiere liberar la estructura.
 */
struct type_value* add_int(int num,enum TYPE type_n2,void* num_2);
/**
 * Agrega algo al bloque de codigo
 * @param codes    Bloque de codigo
 * @param t        Tipo de objeto agregar.
 * @param value    valor
 * @return Char* - Recordar liberar.
 */
char* add_codes_block(char* codes,enum TYPE t, void* value);
/**
 * Suma un entero grande con algo
 * @param  arr    Array
 * @param  t        Tipo de objeto agregar.
 * @param  value    valor
 * @return          struct type_value* . Nuevo objeto array. No es necesario liberar.
 */
struct type_value* add_longint(mpz_t* long_int,enum TYPE t, void* value);
/**
 * @brief      Concatena str con tipo de dato.
 *
 * @param str          The string
 * @param t        parameter_description
 * @param value        The value
 * @param is_right Es para saber si poner el resultado a la derecha.
 *
 * @return static struct type_value* -  El resultado, no se requiere liberar la estructura.
*/
struct type_value* add_str(char* str,enum TYPE t, void* value,bool is_right);
/**Operator add array: Suma un dato con un array
 * @param arr Array*, el array.
 * @param t TYPE, el tipo de dato a evaluar.
 * @param value void*, el dato.
 * @return &static struct type_value. Retorna type_value.type=none para indicar
 ** que no se debe liberar value porque se usa en el nuevo array.
 ** Ojo si se pasa array despues de copiar se hace free(((struct Array*)value)->value);
*/
struct type_value* opr_add_array(struct Array* arr,enum TYPE t, void* value);
#endif