#ifndef STACK_H
#define STACK_H 1
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "define.h"
struct Var{
	char* name;//Nombre de la variable
	enum TYPE type;//El tipo.
	void* value;//Valor. Nota usare malloc/ calloc y readllock.
};
struct type_value{
    enum TYPE type;
    void* value;
};
/**
 * @brief Para crear array dinamicos de cualquier tipo.
 * 
 * @param max Maximo que puedes almacenar en el Array, cuando se supera se reselva mas memoria.
 * @param i El ultimo elemento almacenado.
 * @param value el Valor almacenado en el array.
 */
struct Array{
    unsigned int max;//Para asignar mas memoria de lo que se necesita asegurando velocidad.
    unsigned int i;//Contenido actual.
    struct type_value* value;//Por cada valor necesitamos algo que nos diga que es.
};
/**
 * @brief Ingresa un elemento en el array.
 * 
 * @param arr Array
 * @param type Tipo del elemento, determinado por el enum TYPE
 * @param value Cualquier clase de objeto que este definido en enum TYPE.
 * @return unsigned short 
*/
unsigned short add_array(struct Array* arr,enum TYPE type, void* value);
/**
 * @brief Elimina la ultima posición del array y retorna una estructura si la cantidad total es menor al tamaño maximo que puede obtener entonces se reduce el tamaño que puede obtener.
 * 
 * @param arr array.
 * @return struct type_value* Retornado el elemento sacado del array. Nota: No liberamos la memoria del elemento retornado.
 */
struct type_value* pop_array(struct Array* arr);
/**
 * @brief Libera toda la memoria ocupada por este array y la structura Var que tenga.
 * 
 * @param arr 
 * @return unsigned short 
 */
unsigned short delete_array(struct Array* arr);

/**
 * @brief Aqui configuramos la variable y si ya estubo definida(tv->value!=NULL)
 * la liberamos para redefinir el valor de la variable.
 * 
 * @param v Variable a configurar.
 * @param name Nombre de la variable.
 * @param tv El tipo y el valor de la variable.
 */
void setValue_tv(struct Var* v,char* name,struct type_value* tv);
/**
 * @brief Liberamos la memoria reserbada para almacenar una variable.
 * @param v Variable a eliminar.
 */
void delete_var(struct Var* v);
/**
 * @brief Función que ingresa el valor en la pila, o ejecuta una función en especifico.
 *
 * @param stack pila.
 * @param vars variables.
 * @return string -- Retorna una cadena vacia si
*/
char* interpret(struct Array* stack,struct Array* var,struct Var* v);
/**
 * @brief Enseñamos todo el contenido de la pila en [ elemento_1 elemento_2 elemento_3 ]
 * @param stack pila a mostrar.
 * @return char* output Recuerda liberar memoria. 
*/
char* printf_stack(struct Array* stack);

/**
 * @brief Función que busca la variable dentro de una cadena.
 *
 * @param name Nombre a buscar.
 * @param init_str_1 Donde se inicia en la cadena nombre.
 * @param var //Vector para ver las variables.
 * @return int Para saber si se encontró o no. si lo encontró se retorna el indice, sino -1
 */
int search_var_init(const char* name, unsigned const int init_str_1, struct Array* var);
#define search_var(name,var) search_var_init(name,0,var)
/**
 * @brief Creamos dinamicamente una variable(Nota: debes liberar name y la variable actual).
 * 
 * @param vars el array donde se guardará la variable.
 * @param name Nombre se creará una copia dinamica por lo que debes liberarla despues.
 * @param t 
 * @param value Se inserta el valor sin crear copia.
 */
void add_var(struct Array* vars,char* name,enum TYPE t,void* value);
#endif