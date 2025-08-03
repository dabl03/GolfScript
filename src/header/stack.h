#ifndef STACK_H
#define STACK_H 1
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "define.h"
/**
 * @brief Estructura de las variables
 * @brief | Para la anotación Húngara,
 * recomiendo usar vr como sufijo. 
 * 
 * @param name Nombre usado para 
 * identificar la variable.
 * @param type  Tipo de variable.
 * @param value Valor de la variable.
 * @param i_name Sumatoria de todos los 
 * caracteres de name.
*/
struct Var{
	char*        name;
	enum TYPE    type;
	void*        value;
	unsigned int i_name; // Se usará para comparación.
};
/**
 * @brief Dato genérico | 
 * para la anotación Húngara usar tv.
 * 
 * @param type El tipo que este
 * dato tendrá.
 * @param value El dato a almacenar.
*/
struct type_value{
	enum TYPE type;
	void*     value;
};
/**
 * @brief La pila donde se guardará todo. 
 * para la anotación Húngara usar stc
 * 
 * @param item Item actual de la pila.
 * @param next Siguiente Item.
*/
struct Stack_{
	struct type_value item;
	struct Stack_* next;
	struct Stack_* previous;
};

/**
 * @brief Cabecera donde se guarda info util sobre
 * La pila. 
 * para la anotación Húngara usar hstc
 * 
 * @param item Item actual de la pila.
 * @param next Siguiente Item.
*/
struct Header_Stack{
	struct Stack_* stack;
	struct Header_Stack* father;// Si está vacia llamar al padre.
};
/**
 * @brief Agrega un elemento a la pila.
 * 
 * @param stc_out Donde guardar la nueva pila.
 * @param typ_data Tipo de dato del item
 * @param value Valor del item.
 * @return bool ¿Memoria insuficiente?.
 */
bool add_stack(struct Header_Stack* stc_out,const enum TYPE typ_data, void* value);
/**
 * @brief Saca un elemento de la pila.
 * 
 * @param hstc_modific Pila a modificar.
 * @return new type_value(data_type,data_value)
 */
struct type_value* pop_stack(struct Header_Stack* hstc_modific);
/**
 * @brief Elimina un dato.
 * 
 * @param[in] t_typValue Tipo de dato del item
 * @param[out] v_data     El dato a liberar
 */
void delete_item(const enum TYPE t_typValue, void* v_data);
/**
 * @brief Vacia toda la pila.
 * 
 * @param stc_data Pila a vaciar. (Nota: No se libera stc_data, solo su contenido)
 */
void delete_stack(struct Header_Stack* stc_data);
/**
 * Copia un item y retorna un puntero de la copia.
 * @param[out]  tv_src  Por si se quiere iniciar la estructura, de lo contrario pasar NULL
 * @param[io]  typ_io   Tipo de dato a copiar.
 * @param[io]  value_io Valor de dato a copiar.
 * @return          type_value*
 */
struct type_value* copy_item(struct type_value* tv_src, const enum TYPE typ_io, void* value_io);
/** 
 * @brief Una pila en otra.
 * 
 * @param[in]  stc_io Pila a copiar.
 * @return Nueva pila. (Recordar liberar).
 */
struct Header_Stack* copy_stack(const struct Header_Stack* stc_io);
/**
 * @brief Modifica o agrega un item en una posición especifica en la pila.
 * 
 * @param stc_out Pila a modificar
 * @param b_isAppend Indica si se modifica el elemento o se mueve los demas.
 * @param i_indexSet Indice a modificar.
 * @param typ_data Tipo de dato.
 * @param v_data Dato.
 * @return true Hubo un error.
 * @return false 
 */
bool stack_setItem(
	struct Header_Stack* stc_out,
	const bool b_isAppend,
	const int i_indexSet,
	const enum TYPE typ_data,
	void* v_data
);
/** Da valor a una variable creando un nuevo valor.
 * 
 * @param[out] vr_now Variable a configurar.
 * @param[in] s_name Nombre de la variable. Pasar NULL si ya estaba definida,
 ** para liberar la memoria del valor anteior.
 * @param[in] tv_setVar El tipo y el valor de la variable.
*/
void setValue_tv(struct Var* vr_now,const char* s_name,struct type_value* tv_setVar);
/**
 * @brief Liberamos la variable y ponemos
 * los punteros en NULL
 * @param vr_var La Variable a liberar.
 */
void delete_var(struct Var* vr_var);
/**
 * @brief Interpreta el dato pasado y agrega o quita lo debido en la pila.
 *
 * @param[out] hstc_stack Pila de dato a modificar.
 * @param[in] hstc_var Pila de las variables.
 * @param[in] vr_data    El dato a interpretar.
 */
void process_data(struct Header_Stack* hstc_stack,struct Header_Stack* hstc_var,struct Var* vr_data);
/**
 * @brief Creamos una cadena para mostrar todo
 * el contenido de la pila.
 * @param hstc_io pila a mostrar.
 * @return char* output Recuerda liberar memoria.
*/
char* printf_stack(const struct Header_Stack* hstc_io);

/**
 * @brief Busca la variable deacuerdo al nombre y 
 * retorna su posición en el array.
 *
 * @param[in] s_name Nombre de la variable a buscar.
 * @param[out] hstc_var Pila que contiene las variables.
 * @return Puntero a la variable, NULL si no se consigue.
 */
struct Var* search_var(const char* s_name, struct Header_Stack* hstc_var);
/**
 * @brief Crea una nueva variable y la agrega en la pila.
 * 
 * @param[out] hstc_out Pila a modificar.
 * @param[in] s_name   Nombre de la variable.
 * @param[in] typ_data Tipo de variable.
 * @param[in] v_data   Valor de la variable | 
 * Recordar liberar.
 */
void add_var(struct Header_Stack* hstc_out, const char* s_name, enum TYPE typ_data, void* v_data);
/**
 * @brief Retorna equivalente cadena del elemento pasado.
 * 
 * @param[in]   typ_data Tipo del dato
 * @param[out]  v_data   El dato.
 * @return Equivalente string del dato pasado |
 * Recordar liberar.
 */
char* to_string_value(const enum TYPE typ_data,void* v_data);
#endif