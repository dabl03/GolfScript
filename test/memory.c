#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*Advertencia: Habrá una advertencia con la bibliotecas ya compiladas al intentar liberarlas. */
struct Stack{
  void* memory;
  size_t size;
  char* file;
  unsigned int line;
  char* func;
  struct Stack* next;
};
struct HeadStack{
  struct Stack* top;
};
struct Stack* memory=NULL;
void viewStack(){
  struct Stack* now=memory;
  while (now!=NULL){
    printf(
      "======================================================\n"
      "Dirección %p:\n"
      "    Canitadad asignada: %ld .\n"
      "    Se asigno en la linea: %d .\n"
      "    Se asigno en la función: \"%s\".\n"
      "    Se asigno en el archivo: \"%s\".\n"
      "======================================================\n"
      "",now->memory,now->size,now->line,now->func,now->file
    );
    now=now->next;
  }
}
void* test_malloc(size_t size, const char *file, int line, const char *func){
  struct Stack* stack=(struct Stack*)malloc(sizeof(struct Stack));
  stack->memory=malloc(size);
  stack->size=size;
  stack->file=file;
  stack->line=line;
  stack->func=func;
  if (stack->memory==NULL){
    printf("Error: No se ha podido asignar %d de memoria.\nEsta es la memoria reservada:\n",size);
    viewStack();
    exit(-0xfff);
  }
  stack->next=memory;
  memory=stack;
  return stack->memory;
}
void* test_realloc(void* ptr,size_t size, const char *file, int line, const char *func){
  struct Stack* now=memory;
  while(now!=NULL){
    if (now->memory==ptr){
      now->memory=realloc(ptr,size);
      now->size=size;
      now->file=file;
      now->line=line;
      now->func=func;
      return now->memory;
    }
    now=now->next;
  }
  return test_malloc(size, file, line, func);
}
void test_free(void* ptr, const char *file, int line, const char *func){
  if (memory==NULL)
    return;
  struct Stack* now=memory,
              * item_previous=now;
  bool isModific=false;
  if (now->memory==ptr){
    //Para alterar la variable memory
    memory=now->next;
    free(now);
    free(ptr);
    return;
  }
  while (now!=NULL){
    if (now->memory==ptr){
      item_previous->next=now->next;
      free(now);
      free(ptr);
      return;
    }
    item_previous=now;
    now=now->next;
  }
  printf(
    "Error:\n"
    "  ptr=%p no se ha asignado ubicado en:"
    "  Archivo: \"%s\".\n"
    "  Función: \"%s\".\n"
    "  Linea: %d .\n"
    "", ptr, file, func, line
  );
  free(ptr);
}
// Para probar este archivo...
// gcc -o memory.exe memory.c "-D MAIN=1"/*
#ifdef MAIN
  #include "./include/memory.h"
  #define MAIN main
#endif
int MAIN(void){
  void* a=malloc(sizeof(char)),
      * b=malloc(sizeof(char*)),
      * c=malloc(sizeof(int));
  printf("Las memorias buscada:\n%p    |    %p    |    %p\n",a,b,c);
  viewStack();
  printf("Memoria a eliminar: %p\n",a);
  free(a);
  viewStack();
  free(b);
  free(c);
  viewStack();
  return 0;
}