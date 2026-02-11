#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H
#include <string.h>
#include <gmp.h>
#include "str.h"
#include "stack.h"

/**
 * Las siguientes funciones son utilizadas dentro del programa para
 * Hacer tareas especificas del lenguajes. Cada unas de ellas comparte
 * Las misma estructura:
 * @param stack   Puntero a la pila donde se va a trabajar,
 * @param vars    Las variables decrarada dentro del programa.
 * @param extend  El valor de la variable "n".
 * @return        Codigo de error. Mirar: errors.h
 * 
 * Nota: No todas las funciones usaran los tres parametros, pero
 * Cuando el usuario las llame, por el diseño igual se pasarán.
*/
/// @brief Imprimimos el ultimo elemento por pantalla y lo liberamos.
uint prinft_1_(struct Header_Stack* stack, struct Header_Stack* vars,char* extend);

/// @brief Lo mismo que printf_1_ pero con salto de linea de extensión.
uint puts_operator(struct Header_Stack* stack,struct Header_Stack* vars);
/**
 * Obtiene dos parametros de la pila y los retorna.
 * Nota: No se liberan.
 * Ejemplo: io -> [ num_1 num_2 ], [ num_1 [num_2] ] y [ num_1 num_2 [] ]
	    out -> [0]=num_1, [1]->num_2
	    io-> []
	    out-> *codes=INSUFFICIENT_ARGUMENTS
 * @Param h_stack Donde sacar los datos.
 * @Param codes Puntero donde se almacena INSUFFICIENT_ARGUMENTS
 *   en caso de no conseguir dos parametros.
 * @Return struct type_value[2] No se debe liberar.
*/
struct type_value** opt_get_param(struct Header_Stack* h_stack, uint* codes);

/// @brief El operador suma del interprete.
/// Aqui analizaremos y realizamos la operaciones
/// deacuerdo a su tipo.
uint add_operator(struct Header_Stack* stack,...);
/// @brief Operador de resta.
uint sub_operator(struct Header_Stack* stack,...);
/// @brief Terminar el programa.
uint end_app(void);
/// @brief Funcion para reiniciar las variables globales.
/// Nota: No aqui liberamos la memoria.
uint reset(struct Header_Stack* stack,struct Header_Stack* vars, ...);
/// @brief Empaqueta la pila dentro de un contenedor (otra pila).
uint pack_stack(struct Header_Stack* stack,...);
/// @brief      Muestra una descripción de las funciones predecterminadas.
/// @todo agregar la caracteristica de ver los operadores ver la parte de help para tener mas informacion.
/// @param[in]  vars -- Para obtener el salto de linea.
uint help(struct Header_Stack* stack,struct Header_Stack* vars,...);
/// @brief Aqui iniciamos las variables globales.
/// Nota: No es directamente llamada por el usuario, pero si por reset.
///
/// Tambien puede ser usada para reiniciar las variables globales.
/// @param vars Donde ingresar las variables globales.
uint init_gvars(struct Header_Stack* vars);
#endif