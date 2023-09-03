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
    /**
     * @brief Obtiene el nombre de la variable o un numero y retorna la cadena, ojo hay que liberarla.
     * 
     * @param search Cadena a buscar.
     * @param i indice actual a buscar.
     * @param end Fin de la cadena.
     * @return char* malloc/calloc/readlloc
     */
    char* get_name_var(const char* search,unsigned int* i,unsigned int end);
    /**Elimina los excesivos espacio dejando solo uno para separar cada palabra.
     * Tambien combertimos los saltos de lineas en espacio y se hace lo mismo que el anterior.
     * Nota: Tomamos en cuenta las cadenas ignorando todo lo que este adentro.
     * @param str Cadena
     * @param init { comienzo de la cadena a revisar }
     * @param end Fin de la cadena. si es 0 se busca el final.
     * @return     Cadena tratada para prevenir los excesos de espacio y los saltos de linea. Retorna cadena dinamica, liberar.
     */
    char* get_str_token(char* str,U_INT init,U_INT end);
#endif