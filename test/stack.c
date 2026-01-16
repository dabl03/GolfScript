#include <stdlib.h>
#include <gmp.h>
#include "header/stack.h"
#include "header/str.h"
#include "include/memory.h"
///@todo: Modificar el archivo tester para que los test sean individuales por cada funcion.
//// Estudiar estrategias para testear una app C/C++

void printf_separator(const char separator,const char* msg){
  for (unsigned short i=0;i<80;i++)
    putchar(separator);
  putchar('\n');
  if (msg!=NULL)
    puts(msg);
}
void printf_stack_test(struct Header_Stack* stack,char* sMsg){
  char* sOut=printf_stack(stack);
  printf("%s%s.\n",sMsg,sOut);
  free(sOut);
}
void toStringValue_test(enum TYPE typData,void* vData){
  char* sOut=to_string_value(typData,vData);
  printf(
    "  to_string_value de (%s) vale %s\n",
    get_name_type(typData),
    sOut
  );
  free(sOut);
}
void toStringValue_test_all(){
  void* v_tmp;
  struct Header_Stack hstc={NULL,NULL};
  printf_separator('=',"Testeamos toStringValue_test:");
  toStringValue_test(STRING,"\"Tres triste tigre, tragaban trigo en un trigal..\"");
  v_tmp=alloca(sizeof(int));
  *(int*)v_tmp=123;
  toStringValue_test(INT,v_tmp);
  v_tmp=alloca(sizeof(int));
  *(int*)v_tmp=-123;
  toStringValue_test(INT,v_tmp);

  v_tmp=alloca(sizeof(double));
  *(double*)v_tmp=1.234;
  toStringValue_test(FLOAT,v_tmp);
  v_tmp=alloca(sizeof(double));
  *(double*)v_tmp=-1.234;
  toStringValue_test(FLOAT,v_tmp);

  v_tmp=alloca(sizeof(mpz_t));
  mpz_init_set_str(*(mpz_t*)v_tmp,"19999999999999999999",10);
  toStringValue_test(LONGINT,v_tmp);
  mpz_clear(*(mpz_t*)v_tmp);
  v_tmp=alloca(sizeof(mpz_t));
  mpz_init_set_str(*(mpz_t*)v_tmp,"-19999999999999999999",10);
  toStringValue_test(LONGINT,v_tmp);
  mpz_clear(*(mpz_t*)v_tmp);

  v_tmp=alloca(sizeof(mpf_t));
  mpf_init_set_str(*(mpf_t*)v_tmp,"523.12345e0",10);
  toStringValue_test(LONGFLOAT,v_tmp);
  mpf_clear(*(mpf_t*)v_tmp);
  v_tmp=alloca(sizeof(mpf_t));
  mpf_init_set_str(*(mpf_t*)v_tmp,"-523.12345e0",10);
  toStringValue_test(LONGFLOAT,v_tmp);
  mpf_clear(*(mpf_t*)v_tmp);

  toStringValue_test(CODES_BLOCKS,"\"Haciendo test pasando STRING a la función to_string_value.\"");
  toStringValue_test(FUNCTION,NULL);

  for (unsigned int i=10,*ptr;i>0;i--){
    ptr=(unsigned int*)malloc(sizeof(unsigned int));
    *ptr=i;
    add_stack(&hstc,INT,ptr);
  }
  toStringValue_test(STACK,&hstc);
  delete_stack(&hstc);
  printf_separator('=',NULL);
}
struct Var* create_var(const char* name, enum TYPE tpy,void* value){
  struct Var* out_var=(struct Var*)malloc(sizeof(struct Var));
  out_var->name=convert_static_str_to_dynamic(name);
  out_var->type=tpy;
  out_var->value=value;
  out_var->i_name=0;
  for (unsigned int i=0;name[i]!='\0';i++)
    out_var->i_name+=name[i];
  return out_var;
}
int main(void){
  struct Header_Stack hstc_test={0,0,NULL},
    *hstc_second=NULL;
  struct type_value* tv_data=NULL;
  char options='\n';
  int iData=0;
  void* v_tmp;
  puts("Probando la capacidad de guardar datos en una pila en la memoria...");
  // Testeamos si se guardan correctamentes los datos....
  for (unsigned int i=0;i<100;i++){
    int* o=(int*)malloc(sizeof(int));
    *o=i;
    if (add_stack(&hstc_test,INT,o))
      printf("El valor \"%d\" no pudo guardarse.\n",*o);
  }
  puts("Guardado de entero completo.\nQuieres ver si los datos estan bien guardados? (y/n)");
  while ((options=getchar())!='n' && options!='y');
  
  for (unsigned int i=0;i<hstc_test.i;i++){
    iData=*(int*)hstc_test.value[i].value;
    if (options=='y'){
      printf("  El dato guardado es: %d.\n",iData);
    }
    if (iData!=i){
      printf("Error: Dato no coinciden: %d==%d=False.\n",iData,i);
      puts("Terminará el test...");
      exit(-1);
    }
  }
  printf_separator('=',"Testeamos \"delete_item\"...");
  tv_data=pop_stack(&hstc_test);
  printf("  ¿Es entero?: %s\n  Valor: %d\n",tv_data->type==INT?"True":"False",*(int*)tv_data->value);
  delete_item(tv_data->type,tv_data->value);
  printf("  ¿Es entero?: %s\n  Valor: %d\n",tv_data->type==INT?"True":"False",*(int*)tv_data->value);
  free(tv_data);
  printf("Dato en la pila: %s=%d\n",
    hstc_test.value[iData].type==INT?"INT":"NOT-DEFINE",
    *(int*)hstc_test.value[iData].value
  );
  printf_separator('-',"Testeamos \"delete_stack\"...");
  delete_stack(&hstc_test);
  puts("¿Hay fugas de memoria?:\n");
  viewStack();
  // Testeamos printf_stack
  printf_separator('-',"Testeamos \"printf_stack\"...");
  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Cadena de Prueba\n")
  );

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Entero sin signo:")
  );
  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=1034;
  add_stack(&hstc_test,INT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Entero con signo:")
  );
  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=-1034;
  add_stack(&hstc_test,INT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Flotante sin signo:")
  );
  v_tmp=malloc(sizeof(double));
  *(double*)v_tmp=3203.234;
  add_stack(&hstc_test,FLOAT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Flotante con signo:")
  );
  v_tmp=malloc(sizeof(double));
  *(double*)v_tmp=-3203.234;
  add_stack(&hstc_test,FLOAT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Gran entero sin signo:")
  );
  v_tmp=malloc(sizeof(mpz_t));
  mpz_init (*(mpz_t*)v_tmp);
  mpz_set_si(*(mpz_t*)v_tmp,10000000);
  add_stack(&hstc_test,LONGINT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Gran entero con signo:")
  );
  v_tmp=malloc(sizeof(mpz_t));
  mpz_init (*(mpz_t*)v_tmp);
  mpz_set_si(*(mpz_t*)v_tmp,-1000000);
  add_stack(&hstc_test,LONGINT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Gran flotante sin signo:")
  );
  v_tmp=malloc(sizeof(mpf_t));
  mpf_init (*(mpf_t*)v_tmp);
  mpf_set_d(*(mpf_t*)v_tmp,3.14159265358979);
  add_stack(&hstc_test,LONGFLOAT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Gran flotante con signo:")
  );
  v_tmp=malloc(sizeof(mpf_t));
  mpf_init (*(mpf_t*)v_tmp);
  mpf_set_d(*(mpf_t*)v_tmp,-3.14159265358979);
  add_stack(&hstc_test,LONGFLOAT,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("Bloque de código:")
  );
  add_stack(
    &hstc_test,
    CODES_BLOCKS,
    convert_static_str_to_dynamic("1-a43c13")
  );
  puts("");
  printf_stack_test(&hstc_test,"Actualmente la pila contiene: ");

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("La pila de números entero:")
  );
  v_tmp=malloc(sizeof(struct Header_Stack));
  ((struct Header_Stack*)v_tmp)->i=0;
  ((struct Header_Stack*)v_tmp)->max=0;
  ((struct Header_Stack*)v_tmp)->value=NULL;
  add_stack(&hstc_test,STACK,v_tmp);

  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("La pila recursiva: < ")
  );
  for (unsigned int i=0;i<100;i++){
    int* o=(int*)malloc(sizeof(int));
    *o=i;
    if (add_stack((struct Header_Stack*)v_tmp,INT,o)){
      printf("El valor \"%d\" no pudo guardarse.\n",*o);
      return -1;
    }
    if (i==10){
      puts("Aprovechamos y usamos la pila anterior para testear \"copy_stack\". \n"
      "Ojo: Para no alargar tanto solo tendrá hasta 11 numeros del bucle actual.");
      hstc_second=copy_stack(&hstc_test);
      add_stack(hstc_second,STACK,copy_stack(hstc_second));
      add_stack(hstc_second,STACK,copy_stack(hstc_second));
    }
  }
  add_stack(
    &hstc_test,
    STRING,
    convert_static_str_to_dynamic("End recursive  Stack: > ")
  );
  printf_stack_test(
    &hstc_test,
    "La pila vale:\n"
  );
  printf_separator('-',NULL);
  printf_stack_test(
    hstc_second,
    "La pila copiada vale: "
  );

  printf_separator('=',"Testeamos \"delete_stack\" con el doble de datos...");
  delete_stack(&hstc_test);
  delete_stack(hstc_second);
  free(hstc_second);
  puts("¿Hay fugas de memoria?:\n");
  viewStack();

  printf("El puntero a la pila 2 tiene valor: %p\n",(struct Header_Stack*)v_tmp);

  puts("¿Hay fugas de memoria?:\n");
  viewStack();

  toStringValue_test_all();

  printf_separator('=',"Testeamos add_var");
  // No testeamos setValue_tv porque add_var lo usa.
  /// iTestear setValue_tv!
  v_tmp=convert_static_str_to_dynamic("Definida");
  add_var(&hstc_test,"variable",STRING,v_tmp);
  free(v_tmp);

  v_tmp=convert_static_str_to_dynamic("Michi");
  add_var(&hstc_test,"Gato",STRING,v_tmp);
  free(v_tmp);

  v_tmp=convert_static_str_to_dynamic("Firulai");
  add_var(&hstc_test,"Perro",STRING,v_tmp);
  free(v_tmp);
  printf_stack_test(&hstc_test,"Las variables agregadas son: ");

  printf_separator('-',"Testeamos search_var_init:");
  printf("¿La variable \"Gato\" existe?: %s.\n",(search_var_init("Gato",0,&hstc_test)!=-1)?"SI":"NO");
  printf("¿La variable \"variable\" existe?: %s.\n",(search_var("variable",&hstc_test)!=-1)?"SI":"NO");
  printf("¿La variable \"Perro\" existe?: %s.\n",(search_var("Perro",&hstc_test)!=-1)?"SI":"NO");
  // delete_var->delete_stack->delete_item ya lo llama.
  delete_stack(&hstc_test);
  puts("¿Hay fugas de memoria?:\n");
  viewStack();

  printf_separator('=',"Testeamos process_data:");
  /// @todo: process_data
  puts("¿Hay fugas de memoria?:\n");
  viewStack();
}