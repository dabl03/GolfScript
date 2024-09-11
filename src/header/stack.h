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
	// Pasar de Array a esto.
	struct type_value* item;
	//Ver si agregar: struct Stack_* previous;
	struct Stack_* next;
};
/**
 * @brief Agrega un elemento en el array.
 * 
 * @param[out] arr_allData El array a donde agregar los datos | 
 * si es la primera vez que usas "arr_allData" debes iniciar todo en NULL
 * @param[in] typ_data El tipo de dato a agregar
 * @param value El dato a agregar.
 * @return Error?
*/
bool add_array(struct Array* arr_allData,const enum TYPE typ_data, void* value);
/**
 * @brief Sacar un elemento de la pila.
 * 
 * @param[out] arr_allData Pila de elementos
 * @return Elemento de la pila. Nota: Liberar el elemento.
 */
struct type_value* pop_array(struct Array* arr_allData);
/**
 * @brief Libera todos los datos y el array
 * 
 * @param arr_allData Array a liberar.
 * @return error?
 */
bool delete_array(struct Array* arr_allData);
/**
 * @brief Elimina un dato.
 * 
 * @param[in] t_typValue Tipo de dato del item
 * @param[out] v_data     El dato a liberar
 */
void delete_item(const enum TYPE t_typValue, void* v_data);
/** 
 * @brief Copia los datos del array en un nuevo array.
 * 
 * @param[in]  arr_allData Array a copiar.
 * @return Nuevo array
 */
struct Array* copy_array(const struct Array* arr_allData);
/** Agrega un item en una posicion indicada a un array.
 *
 * @param[out] arr_allData El array a modificar.
 * @param[in]  b_isAppend  modo agregar(true) o modificar la posición del elemento(false)
 * @param[in]  i_indexSet  El indice. Si es -1. Se agregará al final.
 * @param[in]  typ_data      Tipo de dato del item
 * @param[in]  v_data      El valor del item (nota: Se usa el original sin crear uno nuevo)
*/
void array_set_item(struct Array* arr_allData,const bool b_isAppend,const int i_indexSet, const enum TYPE typ_data, void* v_data);
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