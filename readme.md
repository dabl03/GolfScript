El lenguaje se basa en la pila, con solo poner un numero o una cadena se almacena en la pila,
    >1
    output: [1]
    >1
    output:[1 1]
    >a#No hay cambio porque a no esta definido.
    output:[1 1]
    pero si pones alguna letra o caracter entonces lo interpreta como variable o funcion:
    >puts#imprime lo que hay en la pila y saca el elemento de la pila, 
    tambien se puede modificar, es usado por el interprete para imprimir lo que hay en la pila,
    por lo que no debes modificarlo o sino se deja de mostrar la pila.
    Los espacios son una variable que se puede cambiar:
    >1: #Nota que antes del comentario hay un espacio.
    Para sacar datos de la pila se debe usar ";"
    Las variables se declara con valor:nombre
    Realmente todo es una variable, solo ":" es una constante.
    [ #Al hacer esto el contenido completo en la pila se almacena en un array:
    >1 1 1 1
    [1 1 1 1]
    >[
    [[1 1 1 1]]
    >1
    [[1 1 1 1] 1 ]
    Tambien se puede cambiar:(
    { cuando se deja esto abierto el compilador determina un tiempo para que le introduscas algo a dentro.
    Todas las operaciones aritmetica se basan en la pila. + agarra dos elemento dest=dest+ele2 de la pila
    y elimina el segundo. Los demas signos son iguales.
    Recuerda "+" puede funcionar con matrices: [1 2 3] [4 5 6] + # [1 2 3 4 5 6]
    Para declarar una funcion se debe:
    { - 1 * - }:Operacion_loca;#resta 2 elemento quitando un elemento, añade 1...El lenguaje se basa en la pila, con solo poner un numero o una cadena se almacena en la pila,
    >1
    output: [1]
    >1
    output:[1 1]
    >a#No hay cambio porque a no esta definido.
    output:[1 1]
    pero si pones alguna letra o caracter entonces lo interpreta como variable o funcion:
    >puts#imprime lo que hay en la pila y saca el elemento de la pila, 
    tambien se puede modificar, es usado por el interprete para imprimir lo que hay en la pila,
    por lo que no debes modificarlo o sino se deja de mostrar la pila.
    Los espacios son una variable que se puede cambiar:
    >1: #Nota que antes del comentario hay un espacio.
    Para sacar datos de la pila se debe usar ";"
    Las variables se declara con valor:nombre
    Realmente todo es una variable, solo ":" es una constante.
    [ #Al hacer esto el contenido completo en la pila se almacena en un array:
    >1 1 1 1
    [1 1 1 1]
    >[
    [[1 1 1 1]]
    >1
    [[1 1 1 1] 1 ]
    Tambien se puede cambiar:(
    { cuando se deja esto abierto el compilador determina un tiempo para que le introduscas algo a dentro.
    Todas las operaciones aritmetica se basan en la pila. + agarra dos elemento dest=dest+ele2 de la pila
    y elimina el segundo. Los demas signos son iguales.
    Recuerda "+" puede funcionar con matrices: [1 2 3] [4 5 6] + # [1 2 3 4 5 6]
    Para declarar una funcion se debe:
    { - 1 * - }:Operacion_loca;#resta 2 elemento quitando un elemento, añade 1...

Hacer:
    Hacer dos archivo str.cpp y str.h, el h tendra todo los protoripo y sera lo que se incluya, el cpp sera solo la inclusion por compilador.