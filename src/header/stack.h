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
 * @param type El tipo que este
 * dato tendrá.
 * @param value El dato a almacenar.
*/
struct type_value{
	enum TYPE type;
	void*     value;
};
/**
 * @brief Array para almacenar datos
 * genéricos | Para notación Húngara usar arr
 * 
 * @param max Máximo número de elementos que se
 * almacenarán en el array.
 * @param i El ultimo elemento almacenado.
 * @param value Los datos almacenados en el array.
 */
struct Array{
	unsigned int       max;
	unsigned int       i;
	struct type_value* value;
};
/**
 * @brief La pila donde se guardará todo.
 * 
 * @param item Item actual de la pila.
 * @param next Siguiente Item.
*/
struct Stack_{
	struct type_value item;
	struct Stack_* next;
	struct Stack_* previous;
};
struct Header_Stack{
	struct Stack_* stack;
	struct Stack_* father;// Si está vacia llamar al padre.
}
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
 * @param stc_ Encabezado de la pila.
 * @return struct type_value* El item. (Recordar liberar)
 */
struct type_value* pop_stack(struct Header_Stack* stc_);
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
 * @brief Una pila en otra.
 * 
 * @param[in]  stc_io Pila a copiar.
 * @return Nueva pila. (Recordar liberar).
 */
struct Header_Stack* copy_stack(const struct Header_Stack* stc_io);
/** Agrega un item en una posicion indicada a un array.
 *
 * @param[out] arr_allData El array a modificar.
 * @param[in]  b_isAppend  modo agregar(true) o modificar la posición del elemento(false)
 * @param[in]  i_indexSet  El indice. Si es -1. Se agregará al final.
 * @param[in]  typ_data      Tipo de dato del item
 * @param[in]  v_data      El valor del item (nota: Se usa el original sin crear uno nuevo)
*/
void array_set_item(
	struct Array* arr_allData,
	const bool b_isAppend,
	const int i_indexSet,
	const enum TYPE typ_data,
	void* v_data
);
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
 * @param[out] arr_allData El array de datos.
 * @param[in] arr_allVars Las variables.
 * @param[in] vr_data    El dato a interpretar.
 */
void process_data(struct Array* arr_allData,struct Array* arr_allVars,struct Var* vr_data);
/**
 * @brief Creamos una cadena para mostrar todo
 * el contenido del array de datos.
 * @param arr_allData pila a mostrar.
 * @return char* output Recuerda liberar memoria.
*/
char* printf_stack(struct Array* arr_allData);

/**
 * @brief Busca la variable deacuerdo al nombre y 
 * retorna su posición en el array.
 *
 * @param[in] s_name Nombre de la variable a buscar.
 * @param[in] i_initStr Posición inicial para comparar la cadena | 
 * Pasar 0 para buscar la cadena completa.
 * @param[out] arr_var El Array de las Variables.
 * @return Posición de la variable | -1 si no se consigue.
 */
int64_t search_var_init(const char* s_name, unsigned const int i_initStr, struct Array* arr_var);
#define search_var(name,arr_var) search_var_init(name,0,arr_var)
/**
 * @brief Crea una nueva variable y la agrega en el array.
 * 
 * @param[out] arr_var Array destino.
 * @param[in] s_name   Nombre de la variable.
 * @param[in] typ_data Tipo de variable.
 * @param[in] v_data   Valor de la variable | 
 * Recordar liberar.
 */
void add_var(struct Array* arr_var,const char* s_name,enum TYPE typ_data,void* v_data);
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