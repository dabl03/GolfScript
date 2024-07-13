#include <stdlib.h>
#include <gmp.h>
#include "header/stack.h"
#include "include/memory.h"

void printf_stack_test(struct Array* stack,char* sMsg){
  char* sOut=printf_stack(stack);
  printf("%s%s .\n",sMsg,sOut);
  free(sOut);
}
void toStringValue_test(enum TYPE typData,void* vData){
  char* sOut=to_string_value(typData,vData);
  printf("Testeamos to_string_value: %s\n",sOut);
  free(sOut);
}
void toStringValue_test_all(){
  void* v_tmp;
  toStringValue_test(STRING,"\"Haciendo test pasando STRING a la función to_string_value.\"");

  v_tmp=alloca(sizeof(int));
  *(int*)v_tmp=0x234;
  toStringValue_test(INT,v_tmp);

  v_tmp=alloca(sizeof(double));
  *(double*)v_tmp=1.2;
  toStringValue_test(FLOAT,v_tmp);

  v_tmp=alloca(sizeof(mpz_t));
  mpz_init_set_str(*(mpz_t*)v_tmp,"19999999999999999999",10);
  toStringValue_test(LONGINT,v_tmp);

  v_tmp=alloca(sizeof(mpf_t));
  mpf_init_set_str(*(mpf_t*)v_tmp,"523.12345e0",10);
  toStringValue_test(LONGFLOAT,v_tmp);

  toStringValue_test(CODES_BLOCKS,"\"Haciendo test pasando STRING a la función to_string_value.\"");
  toStringValue_test(FUNCTION,NULL);
}
int main(void){
  struct Array arr_testArray={0,0,NULL};
  struct type_value* tv_data=NULL;
  char options='\n';
  char* sOut=NULL;
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
  v_tmp=malloc(sizeof(int));
  *(int*)v_tmp=1034;
  add_array(&arr_testArray,INT,v_tmp);
  v_tmp=malloc(sizeof(double));
  *(double*)v_tmp=3203.234;
  add_array(&arr_testArray,FLOAT,v_tmp);
  v_tmp=malloc(sizeof(char)*5);
  ((char*)v_tmp)[0]='h';
  ((char*)v_tmp)[1]='o';
  ((char*)v_tmp)[2]='l';
  ((char*)v_tmp)[3]='a';
  ((char*)v_tmp)[4]='\0';
  add_array(&arr_testArray,STRING,v_tmp);

  v_tmp=malloc(sizeof(mpz_t));
  mpz_init (*(mpz_t*)v_tmp);
  mpz_set_si(*(mpz_t*)v_tmp,1000);
  add_array(&arr_testArray,LONGINT,v_tmp);
  v_tmp=malloc(sizeof(mpf_t));
  mpf_init (*(mpf_t*)v_tmp);
  // Hacer un visor de decimales.
  // En otras palabras que para los decimales hacer una estructura con la cantidad de decimales y enteros.
  mpf_set_d(*(mpf_t*)v_tmp,3.14159265358979);
  add_array(&arr_testArray,LONGFLOAT,v_tmp);

  v_tmp=malloc(sizeof(char)*9);
  ((char*)v_tmp)[0]='1';
  ((char*)v_tmp)[1]='-';
  ((char*)v_tmp)[2]='a';
  ((char*)v_tmp)[3]='4';
  ((char*)v_tmp)[4]='3';
  ((char*)v_tmp)[5]='c';
  ((char*)v_tmp)[6]='1';
  ((char*)v_tmp)[7]='3';
  ((char*)v_tmp)[8]='\0';
  add_array(&arr_testArray,CODES_BLOCKS,v_tmp);
  printf_stack_test(&arr_testArray,"Actualmente el array contiene: ");

  v_tmp=malloc(sizeof(struct Array));
  ((struct Array*)v_tmp)->i=0;
  ((struct Array*)v_tmp)->max=0;
  ((struct Array*)v_tmp)->value=NULL;
  add_array(&arr_testArray,ARRAY,v_tmp);
  for (unsigned int i=0;i<100;i++){
    int* o=(int*)malloc(sizeof(int));
    *o=i;
    if (add_array((struct Array*)v_tmp,INT,o))
      printf("El valor \"%d\" no pudo guardarse.\n",*o);
  }
  printf_stack_test(&arr_testArray,
    "El array vale:\n---------------------------------\n"
  );
  puts("---------------------------------");

  puts("Testeamos \"delete_array\" con el doble de datos...");
  delete_array(&arr_testArray);
  printf("El puntero al array 2 tiene valor: %p\n",(struct Array*)v_tmp);

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

  toStringValue_test_all();
  ///@todo: Hacer que el testeo anterior guarde todos los tipos de datos.
  // copy_array
  // interpret
  // pop_array
  puts("¿Hay fugas de memoria?:");
  viewStack();
}