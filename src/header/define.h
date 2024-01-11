#ifndef DEFINE_H
#define DEFINE_H 1
    #include <string.h>
    #include <math.h> //isinf para ver si se deborda un double.
    #include <stdint.h>//int64_t
    #include <stdlib.h>//Alloca function
    #if defined(_MSC_VER) || defined(__MINGW32__)
        #include <malloc.h>     // alloca
    #else
        #include <alloca.h>     // alloca
    #endif
    #define ABC_MINUSCULA(c) (c>='a' && c<='z')
    #define ABC_MAYUSCULA(c) (c>='A' && c<='Z')
    #define ERROR_DEFICCION_NO_VALIDA -30
    #define ABC(c) ((c=='_') || (ABC_MAYUSCULA(c) || ABC_MINUSCULA(c)))
    #define IF_INIT_STRING(c) (c=='"' || c=='\'')
    #define COMMENT '#'
    #define IF_INIT_COMENT(c) (c==COMMENT)
    #define ERROR_INDEX_STR (-3)
    #define MEMORY_ERROR (-30)
    #define CHAR_TO_NUM(c) (c-'0')
    #define BUFFER 10000
    #ifndef ENDL
        //Desde el makefile se definirá si es linux.
        #define ENDL "\n"
    #endif
    #define IF_ENDL(c) (c=='\n' || c=='\0')
    #define U_INT unsigned int
    #define AND &&
    #ifndef bool
        #include <stdbool.h>
    #endif
    #define NOT !
    #define MAX_INT_DIG 9
    extern int CLIMIT_INT;//Para ver el limite del entero.
    extern int CLIMIT_FLOAT;//Para ver el limite del flotante.
    extern int quit;//Para saber si terminó la app.
    extern const char* VERSION;//0 porque todavia se esta en desarrollo.
    extern const char* AUTHOR;
    extern const char* LICENSE_URL;
    extern const char* LICENSE;
    enum TYPE{
        NONE,
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