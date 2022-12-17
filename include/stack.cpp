#ifndef STACK_CPP
#define STACK_CPP 1
#include <vector>
#include <string>
#include "./head/str.h"

using namespace std;
struct Vars
{
    string name;    // Nombre de la variable
    enum TYPE type;      // El tipo.
    unsigned int memory; // Cantidad de memoria usada. //Pronto será eliminado.
    void *value;         // Valor. Nota usare malloc/ calloc y readllock.
};
class Var
{ // Nobre del la definición y el tipo.
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
    Var(string n, enum TYPE t, void *v)
    {
        var.name = n;
        setValue(t, v);
    }
    /**
     * @brief Construct a new Var object
     * 
     * @param t Tipo de dato
     * @param v el dato como tal.
     */
    Var(enum TYPE t, void *v)
    {
        var.type = t;
        var.value = v;
    }
    /**
     * @brief Aqui almacenamos la variable deacuerdo a su tipo de dato, y si ya está definida la liberamos para volverla a definir.
     * @param t El tipo que es la variable.
     * @param v valor de la variable.
     **/
    void setValue(enum TYPE t, void *v)
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
            // var.value=(void*)new struct Vars();//No será necesario, porque ya la variable v sera un puntero al head con la estructura Vars.
            // var.value->name=(struct Vars*)v->name;
            break;
        case FUNCTION:
            func = (void (*)(vector<Var>,vector<Var>))v;
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
    void delete_var()
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
    string interpret(vector<Var>& stack,vector<Var>& vars){
        switch (var.type)
        {
        case INT:
            stack.push_back(Var(INT,var.value));
            break;
        case FUNCTION: // Creo que en el lenguaje no existe, pero de todos modos lo voy a utilizar para los operadodes:)
            // No hay nada que liberar, pues solo estoy apuntando a una función no a una memoria dinamica.
            //Recuerda todas las funciónes deben tener estos dos argumentos:
            func(stack,vars);
            break;
        case STRING:
            stack.push_back( 
                Var(STRING,(void*)( *(string*)var.value).c_str())
            );
            break;
        case CODES_BLOCKS: // Será un string que representará el bloque de instrucciones.
            return (*(string*)var.value).substr(1,(*(string*)var.value).length());//Retornamos el bloque para que lo interprete.
        case ARRAY:        // Solo será un array, necesito crear una función que lea el string y retorne la posición.
            stack.push_back(
                Var(ARRAY,(void*)(*(string*)var.value).c_str())
            );
        } 
        return "";
    }
    ~Var(){
        delete_var();
    }
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
int search_var(const char* str_1,unsigned int init_str_1,const vector<Var> vars,unsigned int* end){
    //Ahora vemos si existe la variable.
    unsigned int i_str_2 = 0;
    unsigned int i_temp;
    for (Var v_2 : vars)
    {
        i_temp = init_str_1;
        for (char c : v_2.var.name)
        {
            if (str_1[i_temp] == c && c!='\0')//No hay peligro que se desborde porque a pensas llegue a '\0' se reiniciará.
            {
                i_str_2++;
                i_temp++;
            }
            else if (c == '\0')
            { // Si es la variable que buscamos.
                break;
            }
            else
            {                // No es la variable que buscamos.
                i_str_2 = 0; // Debemos ponerlo en 0 para indicar que no es la variable despues.
                break;
            }
        }
        if (i_str_2){
            *end=i_temp;
            return i_temp;
        }
    }
    *end=i_temp;
    return -1;
}
#endif