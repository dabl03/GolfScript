#ifndef RUN_H
#define RUN_H 1
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "define.h"
    #include "str.h"
    #include "stack.h"
    /**
     * @brief Función que analiza y ejecuta la instrucciones.
     * 
     * @param lines Lineas de codigo que interpretar. 
     * @param stack stack o pila en español.
     * @param vars Variables declaradas.
     * @return int error_codes.
    */
    int run(struct Array* lines,struct Array* stack,struct Array* vars);
#endif