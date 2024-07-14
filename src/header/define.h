#ifndef DEFINE_H
#define DEFINE_H 1
	#include <string.h>
	#include <math.h> //isinf para ver si se deborda un double.
	#include <stdint.h>//int64_t
	#include <stdlib.h>//Alloca function
	#include <stdbool.h>
	// Get alloca function
	#if defined(_MSC_VER) || defined(__MINGW32__)
		#include <malloc.h>     
	#else
		#include <alloca.h>     // alloca
	#endif
	#ifdef TEST_
		#include "../../test/include/memory.h"
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
	#define NOT !
	#define MAX_INT_DIG 9
	#define PRINTF_MEMORY_ERROR(x) printf("Error: No se puede asignar memoria en la función \"%s\".\n    Variable no creada: \"%s\".\n",__FUNCTION__,x)
	extern int CLIMIT_INT;//Para ver el limite del entero.
	extern int CLIMIT_FLOAT;//Para ver el limite del flotante.
	extern int quit;//Para saber si terminó la app.
	extern const char* VERSION;//0 porque todavia se esta en desarrollo.
	extern const char* AUTHOR;
	extern const char* LICENSE_URL;
	extern const char* LICENSE;
	/**
	 * @brief Se usa para identificar los tipos 
	 * de datos que se va a nanejar | 
	 * para anotación Húngara usar "typ"
	*/
	enum TYPE{
		NONE,
		INT, // Entero de 32bits
		LONGINT, // Entero de 64bits
		FLOAT, // double
		LONGFLOAT, // Big float of gmp
		STRING, // string
		PCHAR,// Elominar y cambiar por None
		CHAR, // char* // Eliminar.
		VALUE_TYPE,

		FUNCTION, // Función estandar del programa. Nota: No debe ser liberado.
		VAR, // Variable o estructura Var
		CODES_BLOCKS, // string que representa un código golfscript
		ARRAY // Array de datos
	};
#endif