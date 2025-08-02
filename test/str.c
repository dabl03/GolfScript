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
 * Si la función retorna true la función
 * No paso las pruebas.
 */
bool search_char_test(const char* str,unsigned const char c);
bool is_abc_test(){
  const unsigned int ABC_LETTER='z'-'a';
  unsigned short count=0;
  for (unsigned short i=0;i<100;i++){
    count+=is_abc((char)i);
  }
  return !(count==(ABC_LETTER*2));
}
bool is_num(){
  const unsigned int NUMBER_LETTER='9'-'0';
  unsigned short count=0;
  for (unsigned short i=0;i<100;i++){
    count+=is_num((char)i);
  }
  return !(count==NUMBER_LETTER);
}
bool get_end_str(){
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

bool parseInt(){
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
bool parseLongInt(){
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
void str_add_str_init_end(struct String* str_d, const char* str_copy, const unsigned int init, unsigned int end);

/*@brief Agrega un char al final de la cadena.*/
void str_add_char(struct String* str_d,const char c);

/*@brief Solo iniciamos la estructura String.*/
void init_str(struct String* str,unsigned int buffer);

/*Escapa un caracter.*/
char scape_char(const char c);

/*Transforma los caracteres especiales en su equivalente sin \.*/
char nscape_char(char c);

/*Funcion para identificar los tipos de datos.*/
const char* get_name_type(enum TYPE t);

/*Tratar la cadena para mostrar todos los caracteres especiales sin escapar al momento de hacer printf.*/
char* get_str_nescp(char* old_str);

/*Tratar la cadena para escapar todos los caracteres especiales.*/
char* get_str_escp(char* old_str);

/*Funcion para verificar si la cadena necesita mas memoria para agregarle un char.
 * Nota leftover porque reserva mas memoria que la que necesita.*/
void cadd_leftover(struct String* str_,const char c);

/*Funcion que retorna una cadena hechas con comillas dobles.
 * Si fue comillas simple lo que hace es escapar todas las comillas
 * dobles para que esto: '"hola' pase a esto: "\"hola"*/
char* get_sub_str(const char* str,U_INT init, U_INT end);

/*C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.
 * @param value int. El entero a convertir.
 * @param result char*. Donde se guardara.
 * @param base int. Que reprecentación queremos.
*/
char* itoa(int value, char* result, int base);

/*We take advantage of the fact that we know the end of the string
 * so we don't have to copy and add the string
 * with the desired format.*/
unsigned int append_sprintf(char** out,const unsigned int len_out,const unsigned int len_add_format,const char* format,const char* str_add);

/*We take advantage of knowing the end of the string*/
unsigned int append_strcpy(char** str_out,const unsigned int len,const char* str_io);

/**Copies a string and returns one stored in dynamic memory.*/
char* convert_static_str_to_dynamic(const char* str_io);

bool test_all(){
  if (is_abc_test())
  return false;
}
#ifndef NO_MAIN
#define NO_MAIN
int main(void){
  return test_all();
}
#endif
