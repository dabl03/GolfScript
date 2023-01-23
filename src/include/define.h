#ifndef DEFINE_H
#define DEFINE_H 1
    #include <string.h>
    #define ABC_MINUSCULA(c) (c>='a' && c<='z')
    #define ABC_MAYUSCULA(c) (c>='A' && c<='Z')
    #define ERROR_DEFICCION_NO_VALIDA -30
    #define ABC(c) ((c=='_') || (ABC_MAYUSCULA(c) || ABC_MINUSCULA(c)))
    #define IF_INIT_STRING(c) (c=='"' || c=='\'')
    #define IF_INIT_COMENT(c) (c=='#')
    #define ERROR_INDEX_STR (-3)
    #define MEMORY_ERROR (-30)
    #define CHAR_TO_NUM(c) (c-'0')
    #define BUFFER 10000
    #ifndef ENDL
        //Desde el makefile se definirá si es linux.
        #define ENDL "\n"
    #endif
    #define IF_ENDL(c) (c=='\n')
    extern int CLIMIT_INT;
    extern int CLIMIT_FLOAT;
    enum TYPE{
        INT,
        LONGINT,
        FLOAT,
        LONGFLOAT,
        STRING,
        PCHAR,
        CHAR,
        VALUE_TYPE,
        /*Usado por el interprete:*/
        FUNCTION,
        VAR,
        CODES_BLOCKS,
        ARRAY
    };
#endif