#ifndef GLOBAL_VARS_H
#define GLOBAL_VARS_H
#include <string.h>
#include <gmp.h>
#include "str.h"
#include "stack.h"
/**
 * @brief Imprimimos el ultimo elemento por pantalla y lo liberamos.
 * 
 * @param stack Importante. De aquí sacaremos el ultimo elemento.
 * @param vars No se usará. solo está para adorno.
 * @param extend Algumento opcional. Solo para agregar una extension a la salida.
 * Pienso hacer una variable que determine esto dentro del interprete:)
 * 
 * @return unsigned short 
 */
unsigned short prinft_1_(struct Header_Stack* stack, struct Header_Stack* vars,char* extend);
/**
 * @brief Lo mismo que printf_1_ pero con salto de linea de extensión.
 * 
 * @param stack Importante. De donde se pasará el ultimo elemento.
 * @param vars Para colocarle el salto de linea declarado en la variable n.
 * @return unsigned short 
 */
unsigned short puts_operator(struct Header_Stack* stack,struct Header_Stack* vars);
/**
 * @brief El operador suma del interprete. Aqui analizaremos y realizamos la operaciones deacuerdo a su tipo.
 * 
 * @param stack Importante. Aquí es donde sacaremos los operando. Y liberamos el ultimo.
 * @param ... No necesitamos los demas parámetros que serán pasados igualmente.
 * @return unsigned short 
 */
unsigned short add_operator(struct Header_Stack* stack,...);
/**
 * @brief      Operador de resta.
 * @param      stack      The stack es donde se saca los numeros
 * @param[in]  ... No necesitamos mas parametros
 *
 * @return     is_error?
 */
unsigned short sub_operator(struct Header_Stack* stack,...);
/***
 * Terminamos la app.
*/
unsigned short end_app(void);
/**
 * @brief Funcion para reiniciar las variables globales. Nota: No aqui liberamos la memoria.
 * 
 * @param stack Copatibilidad. No es necesario pasarlo solo que el programa lo pasará de todos modos.
 * @param vars Sumamente importante. Esto es lo que vamos a liberar y volver a rellenar.
 * @param ... siempre se pasarán tres argumento y el tercero no nos importa.
 * @return unsigned short 
 */
unsigned short reset(struct Header_Stack* stack,struct Header_Stack* vars,...);
/**
 * Copia el stack en un array la libera he ingresa ese array en la pila.
 * @param  stack stack of user
 * @return       unsigned short error(si es 1 hubo error)
 */
unsigned short pack_stack(struct Header_Stack* stack,...);
/**
 * @brief      Muestra una descripción de las funciones predecterminadas.
 * @param[in]  stack -------------------@todo agregar la caracteristica de ver los operadores ver la parte de help para tener mas informacion.
 * @param[in]  vars -- Para obtener el salto de linea.
 * @param[in]  ...
 * @return     0-Por copatibilidad.
 */
unsigned short help(struct Header_Stack* stack,struct Header_Stack* vars,...);
/**
 * @brief Aqui iniciamos las variables globales. Tambien puede ser usada para reiniciar las variables globales.
 * 
 * @param vars Donde ingresar las variables globales.
 */
void init_gvars(struct Header_Stack* vars);
#endif