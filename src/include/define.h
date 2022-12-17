#ifndef DEFINE_H
#define DEFINE_H 1
    #include <string>
    #define ABC_MINUSCULA(c) (c>='a' && c<='z')
    #define ABC_MAYUSCULA(c) (c>='A' && c<='Z')
    #define ERROR_DEFICCION_NO_VALIDA -30
    #define ABC(c) (c=='_') || (ABC_MAYUSCULA(c) || ABC_MINUSCULA(c))
    #define IF_INIT_STRING(c) (c=='"' && c=='\'')
    #define IF_INIT_COMENT(c) (c=='#')
    #define ERROR_INDEX_STR (-3)
    #define MEMORY_ERROR (-30)
    #define CHAR_TO_NUM(c) (c-'0')
    #define BUFFER 1000
    inline const char* VERSION="V0";//0 porque todavia se esta en desarrollo.
    enum TYPE{ INT,STRING,FUNCTION,VAR,CODES_BLOCKS,ARRAY};
    inline std::string err_msg;
#endif