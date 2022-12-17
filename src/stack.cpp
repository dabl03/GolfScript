#ifndef STACK_CPP
#define STACK_CPP 1
#include <vector>
#include <string>
#include "./include/str.h"
#include "./include/stack.h"
using namespace std;
/**
 * @todo /\ busca una alternativa que no consuma tanta memoria.
 * @todo Tambien ver si este constructor puedo quitarle param t
 * @brief Construct a new Var object
 *
 * @param n Nombre de la variable
 * @param t Tipo de variable.
 * @param v Valor.
 */
Var::Var(string n, enum TYPE t, void *v)
{
    var.name = n;
    setValue(t, v);
};
/**
 * @brief Construct a new Var object
 *
 * @param t Tipo de dato
 * @param v el dato como tal.
 */
Var::Var(enum TYPE t, void *v)
{
    var.type = t;
    var.value = v;
}
/**
 * @brief Aqui almacenamos la variable deacuerdo a su tipo de dato, y si ya está definida la liberamos para volverla a definir.
 * @param t El tipo que es la variable.
 * @param v valor de la variable.
 **/
void Var::setValue(enum TYPE t, void *v)
{
    if (var.value != NULL)
        delete_var();
    switch (var.type)
    {
    case INT:
        var.value = (void *)new int(*(int *)v);
        break;
    case VAR:
        var.value = v; // Por ahora voy a ver si me funciona así.
        // var.value=(void*)new struct Vars();//No será necesario, porque ya la variable v sera un puntero al include con la estructura Vars.
        // var.value->name=(struct Vars*)v->name;
        break;
    case FUNCTION:
        func = (void (*)(vector<Var>, vector<Var>))v;
        break;
    case STRING:
    case CODES_BLOCKS: // Será un string que representará el bloque de instrucciones.
    case ARRAY:        // Solo será un array, necesito crear una función que lea el string y retorne la posición.
        var.value = new string(*(string *)v);
        break;
    }
    var.type = t;
}
/**
 * @brief Liberamos la memoria reserbada para almacenar una variable.
 */
void Var::delete_var()
{
    if (var.value == NULL)
        return;
    switch (var.type)
    {
    case INT:
        delete (int *)var.value;
        break;
    case VAR:
    case FUNCTION: // Creo que en el lenguaje no existe, pero de todos modos lo voy a utilizar para los operadodes:)
        // No hay nada que liberar, pues solo estoy apuntando a una función no a una memoria dinamica.
        break;
    case STRING:
    case CODES_BLOCKS: // Será un string que representará el bloque de instrucciones.
    case ARRAY:        // Solo será un array, necesito crear una función que lea el string y retorne la posición.
        delete (string *)var.value;
    }
}
/**
 * @brief Función que ingresa el valor en la pila, o ejecuta una función en especifico.
 *
 * @param stack pila.
 * @param vars variables.
 * @return string -- Retorna una cadena vacia si
 */
string Var::interpret(vector<Var> &stack, vector<Var> &vars)
{
    switch (var.type)
    {
    case INT:
        stack.push_back(Var(INT, var.value));
        break;
    case FUNCTION: // Creo que en el lenguaje no existe, pero de todos modos lo voy a utilizar para los operadodes:)
        // No hay nada que liberar, pues solo estoy apuntando a una función no a una memoria dinamica.
        // Recuerda todas las funciónes deben tener estos dos argumentos:
        func(stack, vars);
        break;
    case STRING:
        stack.push_back(
            Var(STRING, (void *)(*(string *)var.value).c_str()));
        break;
    case CODES_BLOCKS:                                                            // Será un string que representará el bloque de instrucciones.
        return (*(string *)var.value).substr(1, (*(string *)var.value).length()); // Retornamos el bloque para que lo interprete.
    case ARRAY:                                                                   // Solo será un array, necesito crear una función que lea el string y retorne la posición.
        stack.push_back(
            Var(ARRAY, (void *)(*(string *)var.value).c_str()));
    }
    return "";
}
Var::~Var()
{
    delete_var();
}
/**
 * @brief Enseñamos todo el contenido de la pila en [ elemento_1 elemento_2 elemento_3 ]
 * @param stack pila a mostrar.
*/
string* Var::printf_stack(vector<Var> &stack){
    string* output=new string(" ");
    for(Var v:stack){
        switch(v.var.type){
            case INT:
                *output+=to_string(*(int*)v.var.value)+' ';
                break;
            case CODES_BLOCKS://Recuerda todo despues de esto es string.
            case ARRAY:
            case STRING:
                *output+=*(string*)v.var.value+' ';
        }
    }
    return output;
}
/**
 * @brief Función que busca la variable dentro de una cadena.
 *
 * @param str_1 cadena a buscar.
 * @param init_str_1 inicio de la primera cadena.
 * @param vars //Vector para ver las variables.
 * @param end Puntero a int, para saber el final de la cadena.
 * @return int Indice de la variable en el vector.
 */
int Var::search_var(const string& str_1, unsigned int init_str_1, const vector<Var> vars, unsigned int *end)
{
    // Ahora vemos si existe la variable.
    unsigned int is_squal = 0;
    unsigned int i=init_str_1;
    unsigned int i_end=str_1.length();
    for (Var v_2 : vars)
    {
        for (char c:*(string*)v_2.var.value)
        {
            if (i==i_end || str_1[i++]!=c){//No es igual y pasamos a la siguiente.
                i=init_str_1;
                is_squal=0;
                break;
            }
            is_squal++;
        }
        if (is_squal){//Es igual y retornamos.
            *end=init_str_1+i;
            return *end;
        }
    }
    //No hay coincidencia, por lo que retorno -1.
    *end = init_str_1+is_squal;
    return -1;
}
#endif