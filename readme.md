# GolfScript
[![logo de licencia MIT](https://user-images.githubusercontent.com/66857879/186940932-591760e2-d06c-49e5-b822-08e6ecef3462.png)](https://raw.githubusercontent.com/dabl03/GolfScript/main/licence)

<p>
    <!--Definición sacada de: http://www.golfscript.com/golfscript/  -->
    GolfScript es un lenguaje de programación esotérico orientado a la pila destinado a resolver problemas ( agujeros ) con la menor cantidad de pulsaciones de teclas posible . También pretende ser simple y fácil de escribir.
</p>
<p>
    El código corto se logra mediante el uso de símbolos únicos para representar operaciones de alto nivel (como mapa, unión, tamaño de matriz, etc.). Estar basado en pilas permite que las funciones y otras manipulaciones funcionen sin necesidad de variables explícitas; sin embargo, las variables siguen existiendo y son útiles para manipulaciones de pilas más complicadas.
</p>
<p>
    Para aprender GolfScript, comience con el tutorial y descargue la implementación. Una vez que se sienta cómodo con él, es posible que desee verificar exactamente cómo se analiza el código y ver todas las funciones integradas.
</p>
<p>
    <a href="http://www.golfscript.com/golfscript/">Para mas información y tutoriales sobre GolfScript</a>
</p>

## Sintaxis:

<p>
    GolfScript se basa en el manejo de pila(Esto hay que saberlo si o si), por lo que todas las operaciones se basan en sacar y meter algo dentro de la pila. 
</p>

Meter algo en la pila:

```Bash
> 1
[ 1 ]
> "hola"
[ 1 "hola" ]
```
Eliminar algo de la pila:
```Bash
> 1 2 3 4 5 #Primero introcucimos algo para despues sacarlo.
[ 1 2 3 4 5 ]
> ;
[ 1 2 3 4 ]
> ;;
[ 1 2 ]
> ;;;
[ ]
Error se intenta sacar mas elemento del que hay en la pila.
```
Operaciones aritmeticas simples:
```Bash
> 2 2 3 4 4 6 4 3#Ingresamos los numeros que vamos a calcular.
[ 2 2 3 4 4 6 4 3 ]
> 1 +
[ 2 2 3 4 4 6 4 4 ]
> +
[ 2 2 3 4 4 6 8 ]
> 2 - -
[ 2 2 3 4 4 0 ]
> *
[ 2 2 3 4 0 ]
> 1 + /
[ 2 2 3 4 ]
```
Creando variables:
```Bash
> 1:var_1
[ 1 ]
> 1:var_2;#Notar que pusimos punto y coma para que el valor de la variable no se instrodusca en la pila
[ 1 ]
> var_1#Pasamos var_1 a la pila
[ 1 1 ]
> { + "Feliz dia" puts }:+;#Redefinimos el operador suma.
[ 1 1 ]
> +
"Feliz día"
[ 2 ]
> { "Hola " }: ;#Redifinimos el espacio(' ')
[ 2 ]
>     ;
[ 2 "hola " "hola " "hola " "hola " ]
```
Creando funciones:
```Bash
> { 1 + 4 2 3 - -} : function #Asignamos la funcion a una variable.
[ { 1 + 4 2 3 - -} ]
> { 1 2 4 4 6 7 }: llenar_pila;
[ { 1 + 4 2 3 - -} ]
> llenar_pila#Llamamos a nuestra función.
[ { 1 + 4 2 3 - -} 1 2 4 4 6 7 ]
> function
[ { 1 + 4 2 3 - -} 1 2 4 4 6 8 3 ]
```
## Dependencias:
- <a href="https://gmplib.org/manual/Headers-and-Libraries">GMP</a>: Biblioteca para hacer operaciones con numeros realmente largos.
- GCC: Para compilarlo.
- make: Para automatizar el compilado.
## Como compilarlo:
Si quiere compilar en linux debes cambiar el valor de la variable `SYSTEM_OS` del archivo `makefile` y listo.
<br/>
Ejecuta el archivo makefile y listo, sino debes hacer:
```Bash
gcc -Wall ./src/main.c {Pon aqui todos los archivos ./src/operators/*.c} {Pon aqui todos los archivos ./src/*.c} -o ./gsp -lgmp
```
Y listo:)

## Creditos:
-  Interprete basado en interprete GolfScript de: <a href="https://github.com/darrenks/golfscript">Darren Smith</a>. 