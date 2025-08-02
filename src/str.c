#ifndef STR_C
#define STR_C 1
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <gmp.h>
#include <stdbool.h>
#include "./header/define.h"
#include "./header/str.h"

unsigned short search_char(const char* str,unsigned const char c){
  for (unsigned int i=0;str[i]!='\0';i++)
    if ((unsigned int)str[i]==c)
      return 1;//Se encontró:)
  return 0;//No se encontro el char:(
}

unsigned short is_abc(unsigned const char c){
    return ABC(c);
}

unsigned short is_num(unsigned const char c){
  return (c>='0' && c<='9');
}

U_INT get_end_str(const char* str, U_INT i_end){
  bool is_scape=false;
  unsigned const char CHAR_END=str[i++];// Is " or ' and no with \
  i_end=(!i_end)?strlen(str):i_end;
  for( U_INT i=0; i<i_end; i++ ){
    if (str[i]==type AND !is_scape)
      return i;
    is_scape=(str[i]=='\\' && !is_scape);
  }
  return 0;// Not found
}

int parseInt(const char* str){
    const bool IS_NEGATIVE=(str[0]=='-');
    int output=0;
    for (unsigned int i=IS_NEGATIVE;str[i]!='\0';i++){
        if (is_num(str[i])){
            output=(output*10)+CHAR_TO_NUM(str[i]);
        }
    }
    return (IS_NEGATIVE)?-output:output;
}

long int parseLongInt(const char* str){
    const bool IS_NEGATIVE=(str[0]=='-');
    long int output=0;
    for (int i=IS_NEGATIVE;str[i]!='\0';i++){
        if (is_num(str[i])){
            output=(output*10)+CHAR_TO_NUM(str[i]);
        }
    }
    return (IS_NEGATIVE)?-output:output;
}

void str_add_str_end(struct String* str_d, const char* str_copy, unsigned int end){
  end=(end)?end:strlen(str_copy);
  if ( str_d->count+end+1>=str_d->max ){
    str_d->str=(char*)realloc(str_d->str,sizeof(char)*(str_d->max+=end+1));
  }
  char* chr_now=(char*)&str_copy;
  for (unsigned int i=0; i<end ; i++){
    str_d->str[str_d->count++]=*chr_now;
    chr_now++;
  }
  str_d->str[str_d->count]='\0';
}

void str_add_char(struct String* str_d,const char c){
  if (str_d->count+1>=str_d->max){
    str_d->str=(char*)realloc(
      str_d->str,
      sizeof(char)*(str_d->max+=( (str_d->max)?1:2) )
    );
  }
  str_d->str[str_d->count++]=c;
  str_d->str[str_d->count]='\0';
}

void init_str(struct String* str,unsigned int buffer){
  str->str=(char*)malloc(sizeof(char)*buffer);
  str->count=0;
  str->max=buffer;
}

char scape_char(const char c){
  switch (c)
  {
  case 'n':
    return '\n';
  case 'b':
    return '\b';
  case 'r':
    return '\r';
  case 'a':
    return '\a';
  case 'e':
    return '\e';
  case 't':
    return '\t';
  case 'v':
    return '\v';
  case '\\':
    return '\\';
  case 'f':
    return '\f';
  /*case '\''://En golfscript esto no se usa
    return '\'';*/
  case '"':
    return '"';
  case '0':
    return '\0';
  case '?':
    return '\?';
  case 'x':
    return -2;
  default:
    return -1;
  }
}

char nscape_char(char c){
  switch(c){
    case '\n':
      return 'n';
    case '\a':
      return 'a';
    case '\e':
      return 'e';
    case '\b':
      return 'b';
    case '\f':
      return 'f';
    case '\r':
      return 'r';
    case '\t':
      return 't';
    case '\v':
      return 'v';
    case '\\':
      return '\\';
    /*case '\''://En golfscript esto no se usa
      return'\'';*/
    case '"':
      return '"';
    case '\?':
      return '\?';
    case '\0':
      return '0';
    default:
      return -1;

  }
}

const char* get_name_type(enum TYPE t){
  switch(t){
    case INT:
      return "(INT)";
        case LONGINT:
      return "(LONG INT)";
    case FLOAT:
      return "(FLOAT)";
    case LONGFLOAT:
      return "(LONG FLOAT)";
    case STRING:
      return "(STRING)";
    case PCHAR:
      return "no se pchar";
    case CHAR:
      return "CHAR";
    case VALUE_TYPE:
      return "VALUE_TYPE";
        /*Usado por el interprete:*/
        case FUNCTION:
      return "(NATIVE FUNCTION)";
    case VAR:
      return "(VAR)";
    case CODES_BLOCKS:
      return "(CODES BLOCKS)";
    case ARRAY:
      return "(ARRAY)";
    default:
      return NULL;
  }
}

char* get_str_nescp(char* old_str){
  U_INT len=strlen(old_str),
  i_nstr=0;
  char* new_str=(char*)malloc(len);//Tamaño base.
  char chr;
  for (char* c=old_str;*c!='\0';c++){
    if (i_nstr+3>=len){//Necesitamos mas memoria para esta cadena.
      new_str=(char*)realloc(new_str,len=i_nstr+20);
    }
    chr=nscape_char(*c);
    if (chr==-1)//No es un caracter especial.
      new_str[i_nstr++]=*c;
    else{
      new_str[i_nstr++]='\\';
      if (*(c+1)=='x'){
        new_str[i_nstr++]='x';
        c++;
        continue;
      }
      new_str[i_nstr++]=chr;
    }
  }
  new_str[i_nstr]='\0';
  new_str=(char*)realloc(new_str,i_nstr+1);
  return new_str;
}
/**
 * @TODO: Buscar y Arreglar un error que impide
 *  que \xhex muestre los caracteres hexadecimales. 
*/
char* get_str_escp(char* old_str){
  U_INT len=strlen(old_str),
  i_nstr=0;
  char* new_str=(char*)malloc(len);//Tamaño base.
  char chr=0;
  bool is_scape=false;
  for (char* c=old_str;*c!='\0';c++){
    if (i_nstr+3>=len){
      new_str=(char*)realloc(new_str,len=i_nstr+20);
    }
    if (*c=='\\' AND NOT is_scape){
      is_scape=true;
      continue;
    }else if (is_scape){
      chr=scape_char(*c);
      if (chr==-1){
        new_str[i_nstr++]=*c;
      }else{
        if (chr==-2 AND (*(c+1)!='\0' AND *(c+2)!='\0')){//Convertimos a hexadecimal.
          char* tmp=(char*)alloca(3);
          tmp[0]=*(++c);
          tmp[1]=*(++c);
          tmp[2]='\0';
          U_INT otro=(int) strtol(tmp, NULL, 16);
          //Si el entero sobrepasa los limites entonces hacemos mas hexadecimales.
          if (otro>CHAR_MAX){
            new_str[i_nstr++]=CHAR_MAX;
            chr=(char)otro-CHAR_MAX;
          }else
            chr=(char)otro;
        }
        new_str[i_nstr++]=chr;
      }
      is_scape=false;
      continue;
    }else{
      if (*c=='"')//Como no tiene un scape entonces la cadena se mostrara sin ""
        continue;
      new_str[i_nstr++]=*c;
    }
  }
  new_str[i_nstr]='\0';
  new_str=(char*)realloc(new_str,i_nstr+1);
  return new_str;
}

void cadd_leftover(struct String* str_,const char c){
  if (str_->count+3>=str_->max){
    str_->str=(char*)realloc(str_->str,str_->max+=20);
  }
  str_->str[str_->count++]=c;
}

char* get_sub_str(const char* str,U_INT init, U_INT end){
  NEW_STRING(out,20);
  U_INT end_c=(end)?end:strlen(str);
  char type=str[init++];
  bool is_scape=false;
  out.str[out.count++]='"';
  for (int i=init;i<end_c;i++){
    if (str[i]==type AND !is_scape)//Si es termino y no es un escape.
      break;//Conseguimos el final de la cadena por lo que terminamos.
    if (type=='\'' && str[i]=='"')
      cadd_leftover(&out,'\\');
    cadd_leftover(&out,str[i]);
    is_scape=(str[i]=='\\' && !is_scape);
  }
  cadd_leftover(&out,'"');
  cadd_leftover(&out,'\0');
  return (char*)realloc(out.str,out.count);
}

char* itoa(int value, char* result, int base) {
  // check that the base if valid
  if (base < 2 || base > 36) { *result = '\0'; return result; }
  char* ptr = result, *ptr1 = result, tmp_char;
  int tmp_value;
  do {
    tmp_value = value;
    value /= base;
    *ptr++ ="zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
  } while ( value );
  // Apply negative sign
  if (tmp_value < 0) *ptr++ = '-';
  *ptr-- = '\0';
  while(ptr1 < ptr){
    tmp_char = *ptr;
    *ptr--= *ptr1;
    *ptr1++ = tmp_char;
  }
  return result;
}
unsigned int append_sprintf(
  char** out,
  const unsigned int len_out,
  const unsigned int len_add_format,
  const char* format,
  const char* str_add
){
  unsigned int len_str_add=len_out+strlen(str_add)+len_add_format;

  *out=(char*)realloc(*out,SIZE_CHAR(len_str_add));
  if (*out==NULL){
    PRINTF_MEMORY_ERROR("Error: Memoria no suficiente.");
    return 0;
  }
  // Agregamos al final de la cadena.
  sprintf((*out)+SIZE_CHAR(len_out-1),format,str_add);
  return len_str_add;
}
unsigned int append_strcpy(char** str_out,const unsigned int len,const char* str_io){
  const unsigned int size_io=strlen(str_io);
  *str_out=(char*)realloc(*str_out,len+size_io);
  strncpy(*str_out+SIZE_CHAR(len-1),str_io,size_io+1);
  return len+size_io;

}
char* convert_static_str_to_dynamic(const char* str_io){
  char* out=(char*)malloc( SIZE_CHAR(strlen(str_io)+1) );
  strcpy(out,str_io);
  return out;
}
#endif
