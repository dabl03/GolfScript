## Falta por hacer:
- Terminar los archivos para testear.
- Hacer una estructura string o modificar la ya existente para que solo tenga char* y U_INT para hacer el programa mas eficiente.
- Agregar decimales.
- Usar la estructura stack para la pila, en lugar de Array. Para aumentar la velocidad de operaciones.
- Para depurar agregar un mensaje de velocidad por operaciones.
- Permitir ejecutar otro programas y pasarle un array como parametros.
- Corregir los errores ortográficos que haya pasado por alto.
- Agregar una función para que se importe archivos externos.
- Arreglar el operador + para que tenga esta capacidad: [ 1 2 3 [+ + +]] -> [6]

- Terminar los operadores.
- Establecer un limite a los nombres de variables en longitud de caracteres.

## Operadores faltantes:
  - `.` cualquier Duplicar en pila
  - `\` cualquiera, cualquier Intercambiar los dos elementos principales.
  - `@` Mueve el stack[-3] elemento de la pila como si lo estuviera appeneando. a_stack_3=stack[-3] Ahora stack[-3]=stack[-2] y stack[-2]=stack[-1] y stack[-1]=a_stack_3;
  - `!` cualquier Negación: empuja 1 si el argumento es 0 o cadena vacía / matriz / bloque, 0 en caso contrario.
  - ` combierte a adena, o escapa la cadena. (Ejemplo: 1 `  es lo mismo que "1", [1 2 3 4] ` "[1 2 3 4]", "hola" ` "\"hola\"").
  - `~` Esteoperador hace:
  - - Int  En sentido no bit a bit no
  - - cadena o bloque  Evaluar
  - - arreglo  Volcar los elementos en la pila
  - `,` Este operador hace:
  - - INT   Genera una lista desde 0 hasta el int pasado(pila)
  - - Lista   Devuelve la longitud de la lista
  - - lista, bloque  Filtro: selecciona los elementos que devuelven true cuando se les aplica el bloque.
  - - - INVESTIGAR PORQUE NO ENTIENDO
  - @todo En la funcion help:
    Agregar la caracteristica de ver los operadores ver la parte de help para tener mas informacion.
}
Para trabajar con flotante usa la varible globa climit_float

## Recordatorio:
- Verificar si father se guarda bien en todos los archivos (debe ser Header_stack).
- Modificar todos los operadores para que trabajen con la estructura stack.
- Modificar el archivo test y verificar stack.c
- Retornar NULL en add stack para solo eliminar el operando 2 y dejar vivo en el otro array el operando 1.
- Modificar los encabezados y las docs para que cuadren con la estructura Stack_
- Hacer una función que verifique si las operaciones que requieren dos elementos de la pila lo haga, y tambien hacer las operaciones necesarias (revisar el operador add para tener una idea)
- .... NO se que mas decir

## Arreglar stack.c:
```
void set_item(
  struct Header_Stack* header,
  struct Stack_* next,
  struct Stack_* previous,
  struct Stack_* new_item,
  bool is_append
) {
  if (is_append){
    // Alteramos el nuevo elemento.
    new_item->next=previous;
    new_item->previous=previous->previous;
    // Alteramos los vecinos.
    previous->previous=new_item;
    if (new_item->previous->next!=NULL)
      new_item->previous->next=new_item;
    else
      header->stack=previous;
  }else{

  }
}
bool stack_setItem(
  struct Header_Stack* stc_out,
  const bool b_isAppend,
  const int i_indexSet,
  const enum TYPE typ_data,
  void* v_data
){
  struct Stack_* new_item=(struct Stack_*)malloc(sizeof(struct Stack_));
  struct Stack_* next=stc_out->stack;
  struct Stack_* previous_stack=NULL;
  U_INT i=i_indexSet;
  new_item->item.type=typ_data;
  new_item->item.value=v_data;
  if (i_indexSet>=0){
    if (i==0){
      //La cima.
      if (b_isAppend){
        add_stack(stc_out,typ_data,v_data);
      }else{
        delete_item(stc_out->stack->item.type,stc_out->stack->item.value);
        stc_out->stack->item.type=typ_data;
        stc_out->stack->item.value=v_data;
      }
      free(new_item);
      return false;
    }
    for (;next!=NULL;i--){
      if (i==0){
        if (b_isAppend){

          new_item->next=next;
        }else{
          new_item->next=next->next;
          new_item->previous=next-previous_stack;
          delete_item(next->item.type,next->item.value);
          free(next);
        }
        return false;
      }
      previous_stack=next;
      next=next->next;
    }
  }else{
    // modificar el final-i_indexSet.
    previous_stack=stc_out->stack;
    do{
      i++;
      next=previous_stack;
      previous_stack=previous_stack->previous;
      if (i==0){//LLegamos al destino
        if (b_isAppend){
          // Modificamos el actual.
          new_item->next=previous_stack;
          new_item->previous=previous_stack->previous;
          // Modificamos los vecinos.
          previous_stack->previous=new_item;
          if (new_item->previous->next!=NULL){
            new_item->previous->next=new_item;
          }else{
            //Alteramos stc_out
            stc_out->stack=new_item;
          }
        }else{
          delete_item(previous_stack->item.type,previous_stack->item.value);
          // Modificamos el actual.
          new_item->previous=previous_stack->previous;
          new_item->next=previous_stack->next;
          // Modificamos los vecinos:
          new_item->next->previous=new_item;
          if (new_item->previous->next!=NULL)//Si no es el suelo:
            new_item->previous->next=new_item;
          else{
            //Alteramos stc_out
            stc_out->stack=new_item;
          }
          free(previous_stack);
        }
        return false;
      }
    }while(next->next!=NULL);
  }
  return true;
}
```