// Cuando termines stack.c empezar a testear este archivo.
#include <string.h>
#include <gmp.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include "../src/header/define.h"
#include "../src/header/str.h"
/**
 * Agregar un mensaje del caso que ha fallado.
 */
/**
 * Testeamos todas las funciones del archivo "str.h".
 * Aquí se define una función por cada función del
 * Archivo, solo que se le agrega el sufijo "_test".
 * Si la función retorna true la función no paso las pruebas.
 */
FUNC_TEST(exist_char_test);
  return (!exist_char("const char* str",'a'))+(exist_char("Arroz",'u'))+(!exist_char("Comida",'i'));
}
FUNC_TEST(is_abc_test);
  const unsigned int ABC_LETTER='z'-'a';
  unsigned short count=0;
  for (unsigned short i=0;i<100;i++){
    count+=is_abc((char)i);
  }
  return !(count==(ABC_LETTER*2));
}
FUNC_TEST(is_num_test);
  const unsigned int NUMBER_LETTER='9'-'0';
  unsigned short count=0;
  for (unsigned short i=0;i<100;i++){
    count+=is_num((char)i);
  }
  return !(count==NUMBER_LETTER);
}
FUNC_TEST(get_end_str_test);
  const char** STRINGS_TO_TEST={
    "The plans is a green\", violet and yellow with pink",
    "The sea is blue\" and red",
    "The mountain\" is a short sea",
    "The \\\"house\\\" is a beautiful",
    "The \\\"cup\\\" is a glass\" and spoon\""
  };
  const char* TEXT_TO_VERIFY={
    "The plans is a green\"",
    "The sea is a blue\"",
    "The mauntain\" is a short sea",
    "The \\\"house\\\" is a beautiful",
    "The \\\"cup\\\" is a glass\"\""
  };
  for (unsigned short i=0; i<sizeof(STRINGS_TO_TEST); i++){
    if (strcmp(TEXT_FOR_TEST[i],TEXT_TO_VERIFY[i])){
      return true;
    }
  }
  return false;
}

FUNC_TEST(parseInt_test);
  const char* STRING_CASE[ ]={
    "123",
    "456",
    "789",
    "2212222222",
    "-123",
    "-2212222222"
  };
  const int INTEGER_CASE[ ]={
    123,
    456,
    789,
    2212222222,
    -123,
    -2212222222
  };
  for (unsigned short i=0; i<sizeof(STRING_CASE); i++){
    if (parseInt(STRING_CASE[i])!=INTEGER_CASE[i]){
      return true;
    }
  }
  return false;
}
FUNC_TEST(parseLongInt_test);
  const char* STRING_CASE[ ]={
    "123",
    "456",
    "789",
    "2212222222",
    "-123",
    "-2212222222",
    "0",
    "1",
    "01"
  };
  const int INTEGER_CASE[ ]={
    123,
    456,
    789,
    2212222222,
    -123,
    -2212222222,
    0,
    1,
    1
  };
  for (unsigned short i=0; i<sizeof(STRING_CASE); i++){
    if (parseInt(STRING_CASE[i])!=INTEGER_CASE[i]){
      return true;
    }
  }
  return false;
}

/*@brief Función que concatena dos cadena usando la estructura de String.*/
FUNC_TEST(str_add_str_init_end_test);return true;}

/*@brief Agrega un char al final de la cadena.*/
FUNC_TEST(str_add_char_test);return true;}

/*@brief Solo iniciamos la estructura String.*/
FUNC_TEST(init_str_test);return true;}

/*Escapa un caracter.*/
FUNC_TEST(scape_char_test);return true;}

/*Transforma los caracteres especiales en su equivalente sin \.*/
FUNC_TEST(nscape_char_test);return true;}

/*Funcion para identificar los tipos de datos.*/
FUNC_TEST(get_name_type_test);return true;}

/*Tratar la cadena para mostrar todos los caracteres especiales sin escapar al momento de hacer printf.*/
FUNC_TEST(get_str_nescp_test);return true;}

/*Tratar la cadena para escapar todos los caracteres especiales.*/
FUNC_TEST(get_str_escp_test);return true;}

/*Funcion para verificar si la cadena necesita mas memoria para agregarle un char.
 * Nota leftover porque reserva mas memoria que la que necesita.*/
FUNC_TEST(cadd_leftover_test);return true;}

/*Funcion que retorna una cadena hechas con comillas dobles.
 * Si fue comillas simple lo que hace es escapar todas las comillas
 * dobles para que esto: '"hola' pase a esto: "\"hola"*/
FUNC_TEST(get_sub_str_test);return true;}

FUNC_TEST(itoa_test);return true;}

/*We take advantage of the fact that we know the end of the string
 * so we don't have to copy and add the string
 * with the desired format.*/
FUNC_TEST(append_sprintf_test);return true;}

/*We take advantage of knowing the end of the string*/
FUNC_TEST(append_strcpy_test);return true;}

/**Copies a string and returns one stored in dynamic memory.*/
FUNC_TEST(convert_static_str_to_dynamic_test);return true;}

bool test_all_test(){
  EXECUTE_TEST(exist_char_test);
  EXECUTE_TEST(is_abc_test);
  EXECUTE_TEST(is_num_test);
  EXECUTE_TEST(get_end_str_test);
  EXECUTE_TEST(parseInt_test);
  EXECUTE_TEST(parseLongInt_test);
  return false;
}
#ifndef NO_MAIN
#define NO_MAIN
int main(void){
  return test_all();
}
#endif
