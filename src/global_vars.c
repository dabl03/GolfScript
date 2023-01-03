#ifndef GLOBAL_VARS_C
#define GLOBAL_VARS_C
#include <string.h>
#include "./include/str.h"
#include "./include/stack.h"
unsigned short prinft_1_(struct Array* stack, struct Array* vars,char* extend){
    if (stack->i){
        struct type_value* tv=pop_array(stack);
        switch (tv->type)
        {
        case INT:
            printf("%d%s",*(int*)tv->value,extend);
            break;
        case CODES_BLOCKS:
            printf("%s%s",(char*)tv->value,extend);
            break;
        case STRING:
        case ARRAY:
            unsigned int len=strlen((char*)tv->value);
            char* out=(char*)malloc(sizeof(char)*len+1);
            strncpy(out,&((char*)tv->value)[1],len-2);
            printf("%s%s",out,extend);
            break;
        default:
            puts("ERROR: La pila esta vacia.");
            break;
        }
        free(tv->value);
    }else{
        puts("Warnign: La pila esta vacia.");
    }
    return 0;
}
#endif