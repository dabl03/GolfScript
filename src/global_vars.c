#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./include/str.h"
#include "./include/stack.h"
#include "./include/global_vars.h"
/*******{TODO}:{
	Operadores que faltan: 
	- .	cualquier	Duplicar en pila
	- \	cualquiera, cualquier	Intercambiar los dos elementos principales
	- @	Mueve el stack[-3] elemento de la pila como si lo estuviera appeneando. a_stack_3=stack[-3] Ahora stack[-3]=stack[-2] y stack[-2]=stack[-1] y stack[-1]=a_stack_3;
	- !	cualquier	Negación: empuja 1 si el argumento es 0 o cadena vacía / matriz / bloque, 0 en caso contrario.
	- ` combierte a adena, o escapa la cadena. (Ejemplo: 1 `  es lo mismo que "1", [1 2 3 4] ` "[1 2 3 4]", "hola" ` "\"hola\"").
	- ~
	-- Int	En sentido no bit a bit no
	-- cadena o bloque	Evaluar
	-- arreglo	Volcar los elementos en la pila
	- ,
	-- INT   Genera una lista desde 0 hasta el int pasado(pila)
	-- Lista   Devuelve la longitud de la lista
	-- lista, bloque	Filtro: selecciona los elementos que devuelven true cuando se les aplica el bloque.
	--- INVESTIGAR PORQUE NO ENTIENDO
    @todo En la funcion help:
        Agregar la caracteristica de ver los operadores ver la parte de help para tener mas informacion.
}
Para trabajar con flotante usa la varible globa climit_float
}
*/
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
unsigned short prinft_1_(struct Array* stack, struct Array* vars,char* extend){
	struct type_value* tv;
	//unsigned int len;//Para y out para array
	char* out;
    if (stack->i){
		tv=pop_array(stack);
        out=to_string_value(tv->type,tv->value);
        if (out==NULL){
            puts("ERROR: Interno, el tipo de dato desconocido.");
            delete_item(tv->type,tv->value);
            return 2;
        }
        printf("%s%s",out,extend);
        free(out);
        delete_item(tv->type,tv->value);
    }else{
        puts("Warnign: La pila esta vacia.");
        return 1;
    }
    return 0;
}
/**
 * @brief Lo mismo que printf_1_ pero con salto de linea de extensión.
 * 
 * @param stack Importante. De donde se pasará el ultimo elemento.
 * @param vars Para colocarle el salto de linea declarado en la variable n.
 * @return unsigned short 
*/
unsigned short puts_operator(struct Array* stack,struct Array* vars){
    int i_var=search_var("n",vars);
    struct Var* this_var=(struct Var*)vars->value[i_var].value;
    char* extend=to_string_value(this_var->type,this_var->value);
    i_var=prinft_1_(stack,NULL,(extend!=NULL)?extend:"");
    if (extend!=NULL)
        free(extend);
    return i_var;
}
/**
 * @brief El operador suma del interprete. Aqui analizaremos y realizamos la operaciones deacuerdo a su tipo.
 * 
 * @param stack Importante. Aquí es donde sacaremos los operando. Y liberamos el ultimo.
 * @param ... No necesitamos los demas parámetros que serán pasados igualmente.
 * @return unsigned short 
 */
unsigned short add_operator(struct Array* stack,...){
    if (stack->i<2){
        puts("Error: No hay suficientes elementos para hacer la suma");
        return 1;
    }
    struct type_value* num_2=pop_array(stack);//Dos se eliminará despues y uno quedará con los resultados.
    struct type_value* num_1=&stack->value[stack->i-1];
    void* tmp=NULL;
    U_INT tmp_len=0;
    switch (num_2->type){
    case INT:
        
        break;
    case CODES_BLOCKS:
        switch(num_1->type){ //[num_1 num_2]
            case INT:
                *(int*)num_1->value+=*(int*)num_2->value;
                break;
            case LONGINT:
                mpz_add_ui(*(mpz_t*)num_1->value,*(mpz_t*)num_1->value,*(int*)num_2->value);
                break;
            case FLOAT:
                *(double*)num_2->value+=*(double*)num_1->value;
                break;
            case STRING:
                tmp=to_string_value(INT,num_2->value);
                tmp_len=strlen(tmp)+strlen((char*)num_1->value)+1;
                num_1->value=realloc(num_1->value,tmp_len);
                strcat((char*)num_1->value,tmp);
                free(tmp);
                break;
            case CODES_BLOCKS:
                tmp=to_string_value(INT,num_2->value);
                tmp_len=strlen(tmp);
                num_1->value=realloc(num_1->value,tmp_len+strlen((char*)num_1->value)+4);
                tmp_len=strlen((char*)num_1->value);
                ((char*)num_1->value)[tmp_len-2]='\0';
                sprintf((char*)num_1->value,"%s%s }",(char*)num_1->value,tmp);
                free(tmp);
                break;
            case ARRAY:
                tmp=malloc(sizeof(int));
                *(int*)tmp=*(int*)num_2->value;
                add_array((struct Array*)num_1->value,INT,tmp);
                break;
            default:
                perror("Caracteristica no disponible en la funcion add_operator\n");
                printf("Type num_2{%s}, type num_1{%s}",get_name_type(num_2->type),get_name_type(num_1->type));
                exit(-8);
        }
        break;
    case STRING:
    case ARRAY:
        break;
    default:
        perror("Caracteristica no disponible en la funcion add_operator\n");
        printf("Type num_2{%s}, type num_1{%s}",get_name_type(num_2->type),get_name_type(num_1->type));
        exit(-8);
    }
    delete_item(num_2->type,num_2->value);
    return 0;
}
/**
 * @brief      Operador de resta.
 * @param      stack      The stack es donde se saca los numeros
 * @param[in]  ... No necesitamos mas parametros
 *
 * @return     is_error?
 */
unsigned short sub_operator(struct Array* stack,...){
    perror("caracteristica no disponible.\nFuncion sub_operator sin terminar.");
    exit(-7);
    return 0;
}
/**
 * @brief Terminaoms la app.
 * 
 * @return unsigned short 
 */
unsigned short end_app(void){
    quit=1;
    return 0;
}
/**
 * @brief Funcion para reiniciar las variables globales. Nota: No aqui liberamos la memoria.
 * 
 * @param stack Copatibilidad. No es necesario pasarlo solo que el programa lo pasará de todos modos.
 * @param vars Sumamente importante. Esto es lo que vamos a liberar y volver a rellenar.
 * @param ... siempre se pasarán tres argumento y el tercero no nos importa.
 * @return unsigned short 
 */
unsigned short reset(struct Array* stack,struct Array* vars,...){
    if (vars->i==0)
        return 1;
    delete_array(vars);
    init_gvars(vars);
    return 0;
}
/**
 * Copia el stack en un array la libera he ingresa ese array en la pila.
 * @param  stack stack of user
 * @return       unsigned short error(si es 1 hubo error)
 */
unsigned short pack_stack(struct Array* stack,...){
    struct Array* tmp=copy_array(stack);
    delete_array(stack);
    add_array(stack,ARRAY,tmp);
    return 0;
}
/**
 * @brief      Muestra una descripción de las funciones predecterminadas.
 * @param[in]  stack 
 * @param[in]  vars -- Para obtener el salto de linea.
 * @param[in]  ...
 * @return     0-Por copatibilidad.
 */
unsigned short help(struct Array* stack,struct Array* vars,...){
    char* key_fun[]={
            "reset",
            "print",
            "puts",
            "quit",
            "]",
            "help",
            "fin_cadena"//Marca un final este array.
    };
    char* value_fun[]={
        "Reinicia las variables globales si se rescriben",
        "Imprime el ultimo elemento en la pila","Imprime el ultimo elemento en la pila y agrega un salto de linea",
        "Termina el programa",
        "Reempraza todos los elementos en la pila con un array con el contenido de esta",
        "Muestra este menu de ayuda - Agrega una cadena en la pila con un operador y te muestra su descripción."
    };
    int i_var=search_var("n",vars);
    struct Var* this_var=(struct Var*)vars->value[i_var].value;
    char* extend=to_string_value(this_var->type,this_var->value);

    printf("%s%s %s%s  Version: %s",LICENSE,extend,AUTHOR,extend,VERSION);
    for (U_INT i=0;strcmp(key_fun[i],"fin_cadena");i++){
        printf("%s%s -- %s%s",extend,key_fun[i],value_fun[i],extend);
    }
    if (extend!=NULL)
        free(extend);
    return 0;
}
/**
 * @brief Aqui iniciamos las variables globales. Tambien puede ser usada para reiniciar las variables globales.
 * 
 * @param vars Donde ingresar las variables globales.
 */
void init_gvars(struct Array* vars){
    add_var(vars,"reset",FUNCTION,(void*)reset);
    add_var(vars,"print",FUNCTION,(void*)prinft_1_);
    add_var(vars,"puts",FUNCTION,(void*)puts_operator);
    add_var(vars,"+",FUNCTION,(void*)add_operator);
    add_var(vars,"-",FUNCTION,(void*)sub_operator);
    add_var(vars,"quit",FUNCTION,(void*)end_app);
    add_var(vars,"]",FUNCTION,(void*)pack_stack);
	add_var(vars,"n",STRING,"\n");
    add_var(vars,"help",FUNCTION,(void*)help);
}
#endif