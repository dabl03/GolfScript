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