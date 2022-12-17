#ifndef STACK_H
#define STACK_H 1
#include <string>
#include <vector>
#include "define.h"

struct Vars{
	std::string name;//Nombre de la variable
	enum TYPE type;//El tipo.
	unsigned int memory;//Cantidad de memoria usada.//Pronto será eliminado.
	void* value;//Valor. Nota usare malloc/ calloc y readllock.
};
class Var{//Nobre del la definición y el tipo.
    public:
	struct Vars var; 
	Var(std::string n,enum TYPE t,void* v);
	Var(enum TYPE t,void*v);
    /**
     * @brief Aqui almacenamos la variable deacuerdo a su tipo de dato, y si ya está definida la liberamos para volverla a definir.
     * @param t El tipo que es la variable.
     * @param v valor de la variable.
    **/
	void setValue(enum TYPE t, void* v);
    /**
     * @brief Función que ingresa el valor en la pila, o ejecuta una función en especifico.
     *
     * @param stack pila.
     */
    string interpret(vector<Var> &stack);
    /**
     * @brief Liberamos la memoria reserbada para almacenar una variable.
    */
	void delete_var();
	~Var();
};
/**
 * @brief Función que busca la variable dentro de una cadena.
 * 
 * @param str_1 cadena a buscar.
 * @param init_str_1 inicio de la primera cadena.
 * @param vars //Vector para ver las variables.
 * @param end Puntero a int, para saber el final de la cadena.
 * @return int Indice de la variable en el vector.
 */
int search_var(const char* str_1,unsigned int init_str_1,const vector<Var> vars,unsigned int* end);
#endif