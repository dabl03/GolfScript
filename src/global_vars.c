#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include <gmp.h>
#include "./include/str.h"
#include "./include/stack.h"
#include "./include/global_vars.h"
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
    if (stack->i){
        struct type_value* tv=pop_array(stack);
        switch (tv->type)
        {
        case INT:
            printf("%d%s",*(int*)tv->value,extend);
            break;
        case LONGINT:
            gmp_printf("%Zd%s",*(mpz_t*)tv->value,extend);
            break;
        case FLOAT:
            printf("%lf%s",*(double*)tv->value,extend);
            break;
        case LONGFLOAT:break;/**@Nota: para gmp double.*/
        case CODES_BLOCKS:
            printf("%s%s",(char*)tv->value,extend);
            break;
        case STRING:
        case ARRAY:
            unsigned int len=strlen((char*)tv->value);
            char* out=(char*)malloc(sizeof(char)*len+1);
            strncpy(out,&((char*)tv->value)[1],len-2);
            printf("%s%s",out,extend);
            free(out);
            break;
        default:
            puts("ERROR: La pila esta vacia.");
            free(tv->value);
            return 2;
        }
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
 * @param ... No nesecitamos mas argumentos.
 * @return unsigned short 
 */
unsigned short puts_operator(struct Array* stack,...){
    return prinft_1_(stack,NULL,"\n");
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
        return -1;
    }
    struct type_value* num_2=pop_array(stack);//Dos se eliminará despues y uno quedará con los resultados.
    struct type_value* num_1=&stack->value[stack->i-1];
    switch (num_2->type)
    {
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
            default://Para arrays.


        }
        if (num_2->type==INT){
            *(int*)num_2->value+=*(int*)num_1->value;
        }else{

        }
        break;
    case CODES_BLOCKS:
        break;
    case STRING:
    case ARRAY:
        break;
    default:
        printf("ERROR: Fallo en la app TYPE: %d, VALUE: %p",num_1->type,num_1->value);
    }
    free(num_1->value);
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
    init_vars_global_gl(vars);
    return 0;
}
/**
 * @brief Aqui iniciamos las variables globales. Tambien puede ser usada para reiniciar las variables globales.
 * 
 * @param vars Donde ingresar las variables globales.
 */
void init_vars_global_gl(struct Array* vars){
    add_var(vars,"reset",FUNCTION,(void*)reset);
    add_var(vars,"print",FUNCTION,(void*)prinft_1_);
    add_var(vars,"puts",FUNCTION,(void*)puts_operator);
    add_var(vars,"+",FUNCTION,(void*)add_operator);

}
#endif