#include <stdlib.h>
#include <gmp.h>
#include "header/stack.h"
#include "header/str.h"
#include "include/memory.h"
/***
  * @todo: Ver en cada ejecución si hay fuga de memoria y localizar donde se inicializó y porque no murió para ver como solucionarlo.
*/
char* static_string_to_dinamic_string(const char* str_io){
  char* out=(char*)malloc( SIZE_CHAR(strlen(str_io)+1) );
  strcpy(out,str_io);
  return out;
}
void printf_stack_test(struct Array* stack,char* sMsg){
  char* sOut=printf_stack(stack);
  printf("%s%s .\n",sMsg,sOut);
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
  struct Array arr={0,0,NULL};
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
    add_array(&arr,INT,ptr);
  }
  toStringValue_test(ARRAY,&arr);
  delete_array(&arr);
  printf("=============================================\n");
}
int main(void){
  struct Array arr_testArray={0,0,NULL},
    *arrSecond=NULL;
  struct type_value* tv_data=NULL;
  char options='\n';
  int iData=0;
  void* v_tmp;
  puts("Probando la capacidad de guardar un array en la memoria...");
  // Testeamos si se guardan correctamentes los datos....
  for (unsigned int i=0;i<100;i++){
    int* o=(int*)malloc(sizeof(int));
    *o=i;
    if (add_array(&arr_testArray,INT,o))
      printf("El valor \"%d\" no pudo guardarse.\n",*o);
  }
  puts("Guardado de entero completo.\nQuieres ver si los datos estan bien guardados? (y/n)");
  while ((options=getchar())!='n' && options!='y');
  
  for (unsigned int i=0;i<arr_testArray.i;i++){
    iData=*(int*)arr_testArray.value[i].value;
    if (options=='y'){
      printf("  El dato guardado es: %d.\n",iData);
    }
    if (iData!=i){
      printf("Error: Dato no coinciden: %d==%d=False.\n",iData,i);
      puts("Terminará el test...");
      exit(-1);
    }
  }
  puts("Testeamos \"delete_item\"...");
  iData=arr_testArray.i-1;
  tv_data=pop_array(&arr_testArray);
  printf("  ¿Es entero?: %s\n  Valor: %d\n",tv_data->type==INT?"True":"False",*(int*)tv_data->value);
  delete_item(tv_data->type,tv_data->value);
  printf("  ¿Es entero?: %s\n  Valor: %d\n",tv_data->type==INT?"True":"False",*(int*)tv_data->value);
  printf("Dato en el array: %s=%d\n",
    arr_testArray.value[iData].type==INT?"INT":"NOT-DEFINE",
    *(int*)arr_testArray.value[iData].value
  );
  puts("Testeamos \"delete_array\"...");
  delete_array(&arr_testArray);

  // Testeamos printf_stack
  puts("Testeamos \"printf_stack\"...");
  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Cadena de Prueba\n")//Agregar los caracteres de escapes disponible
  );

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Entero sin signo:")
  );
  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=1034;
  add_array(&arr_testArray,INT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Entero con signo:")
  );
  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=-1034;
  add_array(&arr_testArray,INT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Flotante sin signo:")
  );
  v_tmp=malloc(sizeof(double));
  *(double*)v_tmp=3203.234;
  add_array(&arr_testArray,FLOAT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Flotante con signo:")
  );
  v_tmp=malloc(sizeof(double));
  *(double*)v_tmp=-3203.234;
  add_array(&arr_testArray,FLOAT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Gran entero sin signo:")
  );
  v_tmp=malloc(sizeof(mpz_t));
  mpz_init (*(mpz_t*)v_tmp);
  mpz_set_si(*(mpz_t*)v_tmp,10000000);
  add_array(&arr_testArray,LONGINT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Gran entero con signo:")
  );
  v_tmp=malloc(sizeof(mpz_t));
  mpz_init (*(mpz_t*)v_tmp);
  mpz_set_si(*(mpz_t*)v_tmp,-1000000);
  add_array(&arr_testArray,LONGINT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Gran flotante sin signo:")
  );
  v_tmp=malloc(sizeof(mpf_t));
  mpf_init (*(mpf_t*)v_tmp);
  mpf_set_d(*(mpf_t*)v_tmp,3.14159265358979);
  add_array(&arr_testArray,LONGFLOAT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Gran flotante con signo:")
  );
  v_tmp=malloc(sizeof(mpf_t));
  mpf_init (*(mpf_t*)v_tmp);
  mpf_set_d(*(mpf_t*)v_tmp,-3.14159265358979);
  add_array(&arr_testArray,LONGFLOAT,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Bloque de código:")
  );
  add_array(
    &arr_testArray,
    CODES_BLOCKS,
    static_string_to_dinamic_string("1-a43c13")
  );
  puts("");
  printf_stack_test(&arr_testArray,"Actualmente el array contiene: ");

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Array de número entero:")
  );
  v_tmp=malloc(sizeof(struct Array));
  ((struct Array*)v_tmp)->i=0;
  ((struct Array*)v_tmp)->max=0;
  ((struct Array*)v_tmp)->value=NULL;
  add_array(&arr_testArray,ARRAY,v_tmp);

  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("Array recursivo: < ")
  );
  for (unsigned int i=0;i<100;i++){
    int* o=(int*)malloc(sizeof(int));
    *o=i;
    if (add_array((struct Array*)v_tmp,INT,o)){
      printf("El valor \"%d\" no pudo guardarse.\n",*o);
      return -1;
    }
    if (i==10){
      /// @todo : Fuga de memoria:
      /// Aquí al eliminar arrays copiados hay fugas de memoriassss..........................................
      puts("Aprovechamos y usamos el array anterior para testear \"copy_array\". \n"
      "Ojo: Para no alargar tanto solo tendrá hasta 11 numeros del bucle actual.");
      arrSecond=copy_array(&arr_testArray);////////////////////////////Hay una fuga de memoria aquí.
      add_array(arrSecond,ARRAY,copy_array(arrSecond));
      add_array(arrSecond,ARRAY,copy_array(arrSecond));
    }
  }
  add_array(
    &arr_testArray,
    STRING,
    static_string_to_dinamic_string("End Array recursivo: > ")
  );
  printf_stack_test(
    &arr_testArray,
    "El array vale:\n---------------------------------\n"
  );
  printf_stack_test(
    arrSecond,
    "-----------------------\nEl array copiado vale:\n---------------------------------\n"
  );
  puts("---------------------------------");
  
  puts("Testeamos \"delete_array\" con el doble de datos...");
  delete_array(&arr_testArray);
  //Hay fuga de memoria:
  delete_array(arrSecond);
  free(arrSecond);
  puts("¿Hay fugas de memoria?:\n");
  viewStack();

  printf("El puntero al array 2 tiene valor: %p\n",(struct Array*)v_tmp);

  puts("========================================\nTesteamos array_set_item:");
  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=10;
  add_array(&arr_testArray,INT,v_tmp);

  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=20;
  array_set_item(&arr_testArray,true,-1,INT,v_tmp);

  printf_stack_test(&arr_testArray,
    "El array vale:\n---------------------------------\n"
  );
  puts("---------------------------------");

  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=30;
  array_set_item(&arr_testArray,true,-1,INT,v_tmp);

  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=40;
  array_set_item(&arr_testArray,false,0,INT,v_tmp);
  printf_stack_test(&arr_testArray,
    "El array ahora vale:\n---------------------------------\n"
  );
  puts("---------------------------------");
  delete_array(&arr_testArray);
  printf("=============================================\nTesteamos to_string_value:\n");
  toStringValue_test_all();
  puts("===============================================");
  
  ///@todo: Hacer que el testeo anterior guarde todos los tipos de datos.
  // interpret
  // pop_array
  // add_var
  // setValue_tv
  // delete_var
  // search_var_init
  // search_var
  puts("¿Hay fugas de memoria?:\n");
  viewStack();
}