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
- Cambiar los archivos de test para que cada cosa tenga su funcion test.
- En los test necesitamos cambiarlo para que se puedan compilar por separado (su propio main) o puedan compilarlo como biblioteca.
- NO_MAIN indica que no se va a definir otro main, dandole luz verde al archivo de definir el propio.
- Tambien hacer los test mas silencioso.
- Terminar los operadores.
- Establecer un limite a los nombres de variables en longitud de caracteres.
- Terminar los operadores suma que faltan bloque de codigo y str y array.
- Corregir el problema que las variables no se asignan bien, el posible problema es str_add_str_end o get_name_var
- Succede una violacion de segmento al usar mucho ++++++++++++++ con muchos numeros. Verificar.
/**
 * @TODO: Buscar y Arreglar un error que impide
 *  que \xhex muestre los caracteres hexadecimales. 
*/
char* get_str_escp(char* old_str){
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
- Modificar todos los operadores para que trabajen con la estructura stack.
- Modificar el archivo test y verificar stack.c
- Retornar NULL en add stack para solo eliminar el operando 2 y dejar vivo en el otro array el operando 1.
- Modificar los encabezados y las docs para que cuadren con la estructura Stack_
- Hacer una función que verifique si las operaciones que requieren dos elementos de la pila lo haga, y tambien hacer las operaciones necesarias (revisar el operador add para tener una idea)
- .... NO se que mas decir

```