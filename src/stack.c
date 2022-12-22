#ifndef STACK_CPP
#define STACK_CPP 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./include/str.h"
#include "./include/stack.h"
/**
 * @brief Ingresa un elemento en el array.
 * 
 * @param arr Array
 * @param type Tipo del elemento, determinado por el enum TYPE
 * @param value Cualquier clase de objeto que este definido en enum TYPE.
 * @return unsigned short 
*/
unsigned short add_array(struct Array* arr,enum TYPE type, void* value){
    if (arr->i==arr->max){//Si se necesita aumentar el espacio.
        arr->value=(struct type_value*)realloc((void*)arr->value,sizeof(struct type_value)*10);
        arr->max+=10;
        if (arr->max==NULL)
            return 1;//No hay mas RAN.
    }
    arr->value[arr->i].type=type;
    arr->value[arr->i++].value=value;//lo inicializamos y Apuntamos al siguiente elemento.
    return 0;
}
/**
 * @brief Elimina la ultima posición del array y retorna una estructura si la cantidad total es menor al tamaño maximo que puede obtener entonces se reduce el tamaño que puede obtener.
 * 
 * @param arr array.
 * @return struct type_value* Retornado el elemento sacado del array. Nota: No liberamos la memoria del elemento retornado.
 */
struct type_value* pop_array(struct Array* arr){
    if (arr->i<(arr->max-10)){//Para asegurar de que no se desperdicie demasiada memoria.
        arr->max-=10;
        arr->value=(struct type_value*)realloc((void*)arr->value,sizeof(struct type_value)*arr->max);
        
    }
    return &arr->value[--arr->i];//Quitamos un elemento de la pila y retornamos ese elemento.
}
/**
 * @brief Libera toda la memoria ocupada por este array y la structura Var que tenga.
 * 
 * @param arr 
 * @return unsigned short 
 */
unsigned short delete_array(struct Array* arr){
    if(arr->value==NULL)
        return 1;
    for (unsigned int i=0;i<arr->i;i++){
        if(arr->value[i].type==VAR){//Si es una variable o un tipo de dato.
            struct Var* v=(struct Var*)arr->value[i].value;
            free(v->name);
            if(v->type!=FUNCTION)//Como es una funcion no necesita liberar memoria.
                free(v->value);
            free(arr->value[i].value);
        }else if(arr->value[i].type!=FUNCTION){//Creo que no es necesario.
            free(arr->value[i].value);
        }
    }
    //Reiniciamos la estructura para que se pueda volver a usar.
    free(arr->value);
    arr->max=0;
    arr->i=0;
    arr->value=NULL;
    return 0;
}
/**
 * @brief Aqui almacenamos la variable deacuerdo a su tipo de dato, y si ya está definida la liberamos para volverla a definir.
 * @param v Variable actual.
 * @param t El tipo que es la variable.
 * @param v valor de la variable.
 **/
void setValue(struct Var* v,enum TYPE t, void *value){
    if (v->value != NULL)
        delete_var(v);
    switch(t){
        case INT:
            v->value=malloc(sizeof(int));
            *(int*)v->value=*(int*)value;
            break;
        case ARRAY:
        case STRING:
        case CODES_BLOCKS:
            char* str=(char*)malloc(sizeof(char)*(strlen((char*)value)+1));
            strcpy(str,(char*)value);
            v->value=value;
    }
    v->type = t;
}
/**
 * @brief Liberamos la memoria reserbada para almacenar una variable.
 * @param v Variable a eliminar.
 */
void delete_var(struct Var* v){
    if (v->value == NULL)
        return;
    if (v->type!=FUNCTION){
        free(v->value);
    }
    v->value=NULL;
}
/**
 * @brief Función que ingresa el valor en la pila, o ejecuta una función en especifico.
 *
 * @param stack pila.
 * @param vars variables.
 * @return string -- Retorna una cadena vacia si
 */
char* interpret(struct Array* stack,struct Array* var,struct Var* v){
    void* value;
    switch (v->type){
    case INT:
        value=malloc(sizeof(int));
        *(int*)value=*(int*)v->value;
        break;
    /*case FUNCTION: // Creo que en el lenguaje no existe, pero de todos modos lo voy a utilizar para los operadodes:)
        // No hay nada que liberar, pues solo estoy apuntando a una función no a una memoria dinamica.
        // Recuerda todas las funciónes deben tener estos dos argumentos:
        ((v->value)(struct Array* stack,struct Array* vars))(stack,var);
        break;*/
    case CODES_BLOCKS://Interpretamos el bloque de código.
        return (char*)v->value;
    case STRING:
    case ARRAY:
        value=malloc(sizeof(char)*(strlen((char*)v->value)+1));
        strcpy((char*)value,(char*)v->value);
    }
    add_array(stack,v->type,value);
    return NULL;
}
/**
 * @brief Enseñamos todo el contenido de la pila en [ elemento_1 elemento_2 elemento_3 ]
 * @param stack pila a mostrar.
 * @return char* output Recuerda liberar memoria.
*/
char* printf_stack(struct Array* stack){
    char* output=(char*)malloc(sizeof(char)*2);
    output[0]='\0';
    unsigned int len=1;
    for(unsigned int i=0;i<stack->i;i++){
        switch(stack->value[i].type){
            case INT:
                len+=8;
                output=(char*)realloc(output,sizeof(char)*len);
                sprintf(output,"%s%d ",output,*(int*)stack->value[i].value);
                break;
            case CODES_BLOCKS://Recuerda todo despues de esto es string.
            case ARRAY:
            case STRING:
                len+=strlen((char*)stack->value[i].value)+2;
                output=(char*)realloc(output,sizeof(char)*len);
                sprintf(output,"%s%s ",output,(char*)stack->value[i].value);
        }
    }
    return output;
}
/**
 * @brief Función que busca la variable dentro de una cadena.
 *
 * @param name Nombre a buscar.
 * @param init_str_1 Donde se inicia en la cadena nombre.
 * @param var //Vector para ver las variables.
 * @param end Puntero a int, para saber el final de la cadena.
 * @return int Para saber si se encontró o no. si lo encontró se retorna el indice, sino -1
 */
int search_var_init_end(const char* name, unsigned const int init_str_1, struct Array* var, unsigned int *end)
{
    // Ahora vemos si existe la variable.
    unsigned int is_squal = 0;
    for (unsigned int i=0;i<var->i;i++){
        struct Var* v_2=(struct Var*)var->value[i].value;
        char* n=&name[init_str_1];
        for (char* c=&v_2->name[0];*c!='\0';c++)
        {
            if (*n=='\0' || *n!=*c){//No es igual y pasamos a la siguiente.
                is_squal=0;
                i=init_str_1;
                break;
            }
            n++;
            is_squal++;
            *end++;
        }
        if (is_squal)//Es igual y retornamos.
            return i;
    }
    //No hay coincidencia, por lo que retorno -1.
    *end = init_str_1+1;
    return -1;
}
#endif