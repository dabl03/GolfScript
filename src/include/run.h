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
/**
 * get init end block. Obtiene un bloque buscando en la cadena, desde init hasta conseguirn end block.
 * Nota: El inicio del bloque se obtiene deacuerdo a init: char inicio=input[init];.
 * No sirve para obtener cadena, en su lugar use get_sub_str o get_end_str.
 * @param  input   Cadena a buscar
 * @param  init    Desde donde empezar
 * @param  end     El caracter a buscar para ver el final.
 * @param  out_end Aqui almacenaremos el indice final para que fuera de la funcion se sepa.
 * @return         retorna el bloque buscado sin el inicio y el final. Recordar liberar cadena.
*/
char* get_ie_block(const char* input,const U_INT init, const char end, U_INT* out_end);
#endif