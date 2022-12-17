#ifndef RUN_H
#define RUN_H 1
    #include <string>
    #include <vector>
    #include "./define.h"
    #include "./str.h"
    #include "./stack.h"
    /**
     * @brief Función que analiza y ejecuta la instrucciones.
     * 
     * @param lines Lineas de codigo que interpretar. 
     * @param stack stack o pila en español.
     * @param vars Variables declaradas.
     * @return int error_codes.
    */
    int analyze_and_run(const vector<std::string>& lines,vector<Var>& stack,vector<Var> vars);
#endif