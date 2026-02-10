## Falta por hacer:
- CREAR ARCHIVOS PARA TESTEAR...
- Hacer que long int y long float se muestre con notacion cientifica.
- Terminar los archivos para testear.
- Agregar decimales.
- Permitir ejecutar otro programas y pasarle un stack como parametros.
- Corregir los errores ortográficos que haya pasado por alto.
- Agregar una función para que se importe archivos externos.
- Arreglar el operador + para que tenga esta capacidad: [ 1 2 3 [+ + +]] -> [6]
- Cambiar los archivos de test para que cada cosa tenga su funcion test.
- En los test necesitamos cambiarlo para que se puedan compilar por separado (su propio main) o puedan compilarlo como biblioteca.
- NO_MAIN indica que no se va a definir otro main, dandole luz verde al archivo de definir el propio.
- Tambien hacer los test mas silencioso.
- Establecer un limite a los nombres de variables en longitud de caracteres.
- Terminar los operadores suma que faltan bloque de codigo y str y array.
- Succede una violacion de segmento al usa mas + de los numeros que hay con muchos numeros. Verificar.
- Arreglar stack porque no capturo bien un array con todos los elementos.
- Arreglar str_add_stack
- Arreglar la funcion base del operador suma que genera errores graves.
- Solucionar: 3"aaa" no se colocan en la pila.
- Solucionar, parece que hay un problema cuando hay mas de un elemento "string" en la entrada del usuario..
- Ver bug "]", succede cuando no hay 
- Ver si crear un tipo operador en init_gvars
- Completar opt_setFloat

=======
- Arreglar el operador + para que tenga esta capacidad: [ 1 2 3 [+ + +]] -> [[6]]
- 

- Terminar los operadores.

>>>>>>> main

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
- Agregar un Debug mode que ensena la linea+ la pila en ese momento. 

## Recordatorio:
- Verificar si father se guarda bien en todos los archivos (debe ser Header_stack).
- Modificar el archivo test y verificar stack.c
- Ver si enlazar la lista del principio al fin y usar indices negativos.
- Si es asi para los bucles hacer: stc_now!=h_stack->stack->previous
```
