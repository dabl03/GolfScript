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
	unsigned int len;//Para y out para array
	char* out;
    if (stack->i){
		tv=pop_array(stack);
        out=to_string_value(tv->type,tv->value);
        if (out==NULL){
            puts("ERROR: Interno, el tipo de dato desconocido.");
            free(tv->value);
            return 2;
        }
        printf("%s%s",out,extend);
        free(out);
        free(tv->value);
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
    switch (num_2->type){
    case INT:
        switch(num_1->type){
            case INT:
                *(int*)num_1->value+=*(int*)num_2->value;
                break;
            case LONGINT:
                mpz_add_ui(*(mpz_t*)num_1->value,*(mpz_t*)num_1->value,*(int*)num_2->value);
                break;
            case FLOAT:
                *(double*)num_2->value+=*(double*)num_1->value;
                break;
            case LONGFLOAT:
                break;//_____________________________________________Pronto se hara uso_____________________________________________
            //default://Para arrays.
			case STRING:
				break;
			
			/*
        PCHAR,
        CHAR,
        VALUE_TYPE,
        //Usado por el interprete:
        FUNCTION,
        VAR,
        CODES_BLOCKS,
        ARRAY
			*/
		}
	  /*> "a" 1+
		["a1"]
		> ; 1 "a" +
		["1a"]
		> ; [1 2 3] 1 +
		[[1 2 3 1]]*/
		
        break;
    case CODES_BLOCKS:
        break;
    case STRING:
    case ARRAY:
        break;
    default:
        printf("ERROR: Fallo en la app TYPE: %d, VALUE: %p",num_1->type,num_1->value);
    }
    free(num_2->value);
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
 * @brief Aqui iniciamos las variables globales. Tambien puede ser usada para reiniciar las variables globales.
 * 
 * @param vars Donde ingresar las variables globales.
 */
void init_gvars(struct Array* vars){
    add_var(vars,"reset",FUNCTION,(void*)reset);
    add_var(vars,"print",FUNCTION,(void*)prinft_1_);
    add_var(vars,"puts",FUNCTION,(void*)puts_operator);
    add_var(vars,"+",FUNCTION,(void*)add_operator);
    add_var(vars,"quit",FUNCTION,(void*)end_app);
	add_var(vars,"n",STRING,"\n");
}
#endif