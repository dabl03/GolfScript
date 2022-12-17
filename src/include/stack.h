#ifndef STACK_H
#define STACK_H 1
#include <string>
#include <vector>
#include "define.h"

using namespace std;
struct Vars{
	std::string name;//Nombre de la variable
	enum TYPE type;//El tipo.
	unsigned int memory;//Cantidad de memoria usada.//Pronto será eliminado.
	void* value;//Valor. Nota usare malloc/ calloc y readllock.
};
class Var{//Nobre del la definición y el tipo.
    public:
	struct Vars var;
    void (*func)(vector<Var>,vector<Var>);//No se usará con todas las variables, solo con la función.
    /**
     * @todo /\ busca una alternativa que no consuma tanta memoria.
     * @todo Tambien ver si este constructor puedo quitarle param t
     * @brief Construct a new Var object
     * 
     * @param n Nombre de la variable
     * @param t Tipo de variable.
     * @param v Valor.
     */
	Var(std::string n,enum TYPE t,void* v);
    /**
     * @brief Construct a new Var object
     * @param t Tipo de dato
     * @param v el dato como tal.
     */
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
     * @param vars variables.
     * @return string -- Retorna una cadena vacia si
     */
    string interpret(vector<Var>& stack,vector<Var>& vars);
    /**
     * @brief Liberamos la memoria reserbada para almacenar una variable.
    */
	void delete_var();
    ~Var();
    /**
     * @brief Enseñamos todo el contenido de la pila en [ elemento_1 elemento_2 elemento_3 ]
     * @param stack pila a mostrar.
     */
    static string* printf_stack(vector<Var> &stack);
    /**
     * @brief Función que busca la variable dentro de una cadena.
     *
     * @param str_1 cadena a buscar.
     * @param init_str_1 inicio de la primera cadena.
     * @param vars //Vector para ver las variables.
     * @param end Puntero a int, para saber el final de la cadena.
     * @return int Indice de la variable en el vector.
     */
    static int search_var(const string& str_1, unsigned int init_str_1, const vector<Var> vars, unsigned int *end);
};
#endif