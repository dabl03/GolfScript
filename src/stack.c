#ifndef STACK_C
#define STACK_C 1
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "./include/str.h"
#include "./include/stack.h"
#include "./include/define.h"
/**
 * @brief Ingresa un elemento en el array.
 * 
 * @param arr Array- Recordar que tienes que llenar la estructuras aunque todos los valores sean 0 es necesario por un error que no entiendo.
 * @param type Tipo del elemento, determinado por el enum TYPE
 * @param value Cualquier clase de objeto que este definido en enum TYPE.
 * @return unsigned short 
*/
unsigned short add_array(struct Array* arr,enum TYPE type, void* value){
    if (arr->i==arr->max){//Si se necesita aumentar el espacio.
        arr->value=(struct type_value*)realloc(
            (void*)arr->value,
            sizeof(struct type_value)*(arr->max+=10)
        );
        if (arr->value==NULL)
            return 1;//No hay mas RAN.
    }
    arr->value[arr->i].type=type;
    arr->value[arr->i++].value=value;//lo inicializamos y Apuntamos al siguiente elemento.
    return 0;
}
/**
 * @brief Elimina la ultima posición del array y retorna una estructura si la cantidad total es menor al tamaño maximo que puede obtener entonces se reduce el tamaño que puede obtener.
 * 
 * @param arr array.
 * @return struct type_value* Retornado el elemento sacado del array. Nota: No liberamos la memoria del elemento retornado.
 */
struct type_value* pop_array(struct Array* arr){
    if (arr->i<(arr->max-10)){//Para asegurar de que no se desperdicie demasiada memoria.
        arr->max-=10;
        arr->value=(struct type_value*)realloc((void*)arr->value,sizeof(struct type_value)*arr->max);
    }
    return &arr->value[--arr->i];//Quitamos un elemento de la pila y retornamos ese elemento.
}
/**
 * @brief Libera toda la memoria ocupada por este array y la structura Var que tenga.
 * 
 * @param arr 
 * @return unsigned short 
 */
unsigned short delete_array(struct Array* arr){
    if(arr->value==NULL)
        return 1;
    for (unsigned int i=0;i<arr->i;i++){
        delete_item(arr->value[i].type,arr->value[i].value);
    }
    //Reiniciamos la estructura para que se pueda volver a usar.
    free(arr->value);
    arr->max=0;
    arr->i=0;
    arr->value=NULL;
    return 0;
}
/**
 * Copiamos un array y retornamos uno nuevo con todo sus elementos.
 * @param  arr Array a copiar.
 * @return     Array destino.
 */
struct Array* copy_array(struct Array* arr){
    struct Array* out=(struct Array*)malloc(sizeof(struct Array));
    void* tmp;//Aqui almacenaremos los punteros para despues guardarlos.
    out->i=0;
    out->max=0;
    out->value=NULL;
    for (unsigned int i=0;i<arr->i;i++){
        switch(arr->value[i].type){
            case VAR:///Caracteristicas no disponible.
                perror("Caracteristica no añadio la caracteristica. Dile al desarrollador o añadela.\n"
                "En funcion copy_array->caso VAR.\nEnter para terminar.");
                tmp=malloc(sizeof(int));
                while((*(int*)tmp=getchar())!=EOF && *(int*)tmp!='\0');
                free(tmp);
                exit(-4);
                break;
            case ARRAY:
                tmp=copy_array((struct Array*)arr->value[i].value);
                break;
            case INT:
                tmp=malloc(sizeof(int));
                *(int*)tmp=*(int*)arr->value[i].value;
                break;
            case FLOAT:
                tmp=malloc(sizeof(double));
                *(double*)tmp=*(double*)arr->value[i].value;
                break;
            case LONGINT:
                tmp=malloc(sizeof(mpz_t));
                mpz_init(*(mpz_t*)tmp);
                mpz_set(*(mpz_t*)tmp,*(mpz_t*)arr->value[i].value);
                break;
            case LONGFLOAT:
                tmp=malloc(sizeof(mpf_t));
                mpf_init_set(*(mpf_t*)tmp,*(mpf_t*)arr->value[i].value);
                break;
            case CODES_BLOCKS:
            case STRING:
                tmp=malloc(strlen((char*)arr->value[i].value)+1);
                strcpy((char*)tmp,(char*)arr->value[i].value);
                break;
            default:
                perror("Error interno de la app, me falto un tipo de dato por verificar en la funcion copy_array.");
                printf("\nEl tipo se llama %s\n",get_name_type(arr->value[i].type) );
                exit(-2);
        }
        add_array(out,arr->value[i].type,tmp);
    }
    return out;
}
/**
 * @brief      Agrega un item en una posicion indicada a un array.
 *
 * @param      arr        El array a modificar.
 * @param[in]  is_append  Indica si se agrega(true) o simpremente se modifica la posición del elemento(false)
 * @param[in]  index_set  El indice a modificar. Si es -1 entonces se agrega al final.
 * @param[in]  t_out      Tipo de dato
 * @param      value      El valor(nota no se hace copia por lo que debe ser de memoria dinamica el valor)
 */
void array_set_item(struct Array* arr,bool is_append,int index_set, enum TYPE t_out, void* value){
    struct type_value cp,
    cp_after;
    U_INT i=index_set+1;
    if (!is_append){//Si no es append item entonces modificamos el valor de ese espacio.
        int index=(index_set!=-1)?index_set:arr->i-1;
        delete_item(arr->value[index].type,arr->value[index].value);
        arr->value[index].type=t_out;
        arr->value[index].value=value;
        return;
    }else if (index_set==-1){//Aunque si es -1 significa agregar al final
        add_array(arr,t_out,value);
        return;
    }else if(index_set>arr->i || index_set<0){//No queremos indices negativos.
        //Prevenimos que el indice pasado sea mayor
        //al ultimo objeto+1 para no dejar espacios vacios.
        printf("Advertencia: se intenta acceder a una posición que no se puede."
            "\nIndice pasado: %d  -  Indice maximo a modificar: %d.\n",index_set,arr->i);
        return;
    }
    //Modo Append.
    //Si no hay mas espacio se agregara 10 mas para almacenar todo
    add_array(arr,PCHAR,NULL);
    //Copiamos el primer elemento.
    cp.type=arr->value[index_set].type;
    cp.value=arr->value[index_set].value;
    //Agregamos el elemento modificaco.
    arr->value[index_set].type=t_out;
    arr->value[index_set].value=value;

    for (;i<arr->i;i++){
        //Copiamos el elemento actual para despues agregarlo.
        cp_after.type=arr->value[i].type;
        cp_after.value=arr->value[i].value;
        //Agregamos el elemento anterior.
        arr->value[i].type=cp.type;
        arr->value[i].value=cp.value;
        //Copiamos el elemento para el proximo ciclo.
        cp.type=cp_after.type;
        cp.value=cp_after.value;
    }
}
/**
 * @brief Aqui configuramos y si ya estubo definida pase NULL en name
 ** para indicar que ya existe y hay que liberar su valor.
 * 
 * @param v Variable a configurar.
 * @param name Nombre de la variable.
 * @param tv El tipo y el valor de la variable.
 */
void setValue_tv(struct Var* v,char* name,struct type_value* tv){
    char* str;
    //No estuvo definida antes
    if (name==NULL){
        delete_item(v->type,v->value);
    }else{
        v->name=(char*)malloc(sizeof(name));
        strcpy(v->name,name);
    }
    v->type = tv->type;
    switch(v->type){
        case INT:
            v->value=malloc(sizeof(int));
            *(int*)v->value=*(int*)tv->value;
            break;
        case FLOAT:
            v->value=malloc(sizeof(double));
            *(double*)v->value=*(double*)tv->value;
            break;
        case LONGINT:
            v->value=malloc(sizeof(mpz_t));
			mpz_init(*(mpz_t*)v->value);//Importante.
            mpz_set(*(mpz_t*)v->value,*(mpz_t*)tv->value);
            break;
        case LONGFLOAT:
            v->value=malloc(sizeof(mpf_t));
            mpf_init_set(*(mpf_t*)v->value,*(mpf_t*)tv->value);
            break;
        case ARRAY:
            v->value=copy_array((struct Array*)tv->value);
            break;
        case STRING:
        case CODES_BLOCKS:
            str=(char*)malloc(sizeof(char)*(strlen((char*)tv->value)+1));
            strcpy(str,(char*)tv->value);
            v->value=str;
            break;
        case FUNCTION:break;//No es necesario porque este nunca estará en la pila.
        default:
            printf("Error tipo %s no tratado en la función setValue_tv\n",get_name_type(tv->type) );
            exit(-3);
    }
}
/**
 * @brief Liberamos la memoria reserbada para almacenar una variable.
 * @param v Variable a eliminar.
 */
void delete_var(struct Var* v){
    delete_item(v->type,v->value);
    free(v->name);
    v->value=NULL;
}
/**
 * Elimina un item.
 * @param t     Tipo de dato del item
 * @param value Valor
 */
void delete_item(enum TYPE t, void* value){
    switch(t){
        case VAR:
            delete_var((struct Var*)value);
            free(value);
            break;
        case FUNCTION:break;//Las funciones no son necesario liberar.
        case ARRAY:
            delete_array((struct Array*)value);
            free(value);
        break;
        case LONGINT:
            mpz_clear(*(mpz_t*)value);
            free(value);
            break;
        case LONGFLOAT:
            mpf_clear(*(mpf_t*)value);
        //No hay forma diferente para liberar.
        case INT:
        case CODES_BLOCKS:
        case STRING:
        case FLOAT:
            free(value);
            break;
        default:
            perror("Error: Se intenta liberar un tipo de dato no tratado. \nFuncion \"delete_item\" ");
            printf("Type: %s",get_name_type(t));
            exit(-111111);
    }
}
/**
 * @brief Función que ingresa el valor en la pila, o ejecuta una función en especifico.
 *
 * @param stack pila.
 * @param vars variables.
 * @return string -- Retorna una cadena vacia si
 */
char* interpret(struct Array* stack,struct Array* var,struct Var* v){
    void* value;
    switch (v->type){
    case INT:
        value=malloc(sizeof(int));
        *(int*)value=*(int*)v->value;
        break;
    case FLOAT:
        value=malloc(sizeof(double));
        *(double*)value=*(double*)v->value;
        break;
    case FUNCTION: //Usamos funciones dentro del mismo programa para esta.
        ((unsigned short (*)(struct Array* stack,struct Array* vars,char* extend))v->value)(stack,var,"");
        return NULL;
    case CODES_BLOCKS://Interpretamos el bloque de código.
        return (char*)v->value;
    case STRING:
        value=malloc(sizeof(char)*(strlen((char*)v->value)+1));
        strcpy((char*)value,(char*)v->value);
		break;
    case ARRAY:
        value=copy_array((struct Array*)v->value);
        break;
	case LONGINT://Copiamos el entero largo a otro.
		value=malloc(sizeof(mpz_t));
		mpz_init(*(mpz_t*)value);//Importante.
		mpz_set(*(mpz_t*)value, *(mpz_t*)v->value);
		break;
    case LONGFLOAT:
        value=malloc(sizeof(mpf_t));
        mpf_init_set(*(mpf_t*)value, *(mpf_t*)v->value);
        break;
    default:
        printf("Error tipo %s no tratado en la función setValue_tv\n",get_name_type(v->type) );
        exit(-3);
    }
    add_array(stack,v->type,value);
    return NULL;
}
/**
 * @brief Enseñamos todo el contenido de la pila en [ elemento_1 elemento_2 elemento_3 ]
 * @param stack pila a mostrar.
 * @return char* output Recuerda liberar memoria.
*/
char* printf_stack(struct Array* stack){
    char* output=(char*)malloc(sizeof(char)*2);
	char* a_out=NULL;//Cuando necesitemos una cadena secundarias.
    void* tmp=NULL;//Para un uso general.
    output[0]='\0';
    unsigned int len=1;
    for(unsigned int i=0;i<stack->i;i++){
        switch(stack->value[i].type){
            case INT:
                tmp=alloca(50);
                itoa(*(int*)stack->value[i].value,(char*)tmp,10);
                len+=strlen((char*)tmp)+1;
                output=(char*)realloc(output,len);
                sprintf(output,"%s%s ",output,(char*)tmp);
                break;
            case FLOAT:
                tmp=alloca(60);
                sprintf((char*)tmp,"%f",*(double*)stack->value[i].value);
                len+=strlen((char*)tmp)+1;
                output=(char*)realloc(output,len);
                sprintf(output,"%s%s ",output,(char*)tmp);
                break;
            case ARRAY:
                a_out=printf_stack((struct Array*)stack->value[i].value);
                len+=strlen(a_out)+6;
                output=(char*)realloc(output,len);
                sprintf(output,"%s[ %s] ",output,a_out);
                free(a_out);
                break;
            case CODES_BLOCKS://Recuerda todo despues de esto es string.
                len+=strlen((char*)stack->value[i].value)+7;
                output=(char*)realloc(output,sizeof(char)*len);
                sprintf(output,"%s{%s} ",output,(char*)stack->value[i].value);
                break;
            case STRING:
                a_out=get_str_nescp((char*)stack->value[i].value);
                len+=strlen(a_out)+3;
                output=(char*)realloc(output,sizeof(char)*len);
                sprintf(output,"%s\"%s\" ",output,a_out);
                free(a_out);
				break;
			case LONGINT:
            case LONGFLOAT:
				a_out=(stack->value[i].type==LONGINT)?
                mpz_get_str(NULL,0,*(mpz_t*)stack->value[i].value)
                :mpf_get_str(NULL, NULL, 10, 0, *(mpf_t*)stack->value[i].value);
				output=(char*)realloc(output,len+=strlen(a_out)+1);
				sprintf(output,"%s%s ",output,a_out);
				free(a_out);
				break;
        }
    }
    return output;
}
/**
 * @brief Función que busca la variable dentro de una cadena.
 *
 * @param name Nombre a buscar.
 * @param init_str_1 Donde se inicia en la cadena nombre.
 * @param var //Vector para ver las variables.
 * @return int Para saber si se encontró o no. si lo encontró se retorna el indice, sino -1
 */
int search_var_init(const char* name, unsigned const int init_str_1, struct Array* var){
    for (unsigned int i=0;i<var->i;i++){
        struct Var* v_2=(struct Var*)var->value[i].value;//Variable actual.
        if (!strcmp(&name[init_str_1],v_2->name))//Es igual y retornamos.
            return i;
    }
    //No hay coincidencia, por lo que retorno -1.
    return -1;
}
/**
 * @brief Creamos dinamicamente una variable(Nota: debes liberar name y la variable actual).
 * 
 * @param vars el array donde se guardará la variable.
 * @param name Nombre se creará una copia dinamica por lo que debes liberarla despues.
 * @param t 
 * @param value Se inserta el valor sin crear copia dinamica.
 */
void add_var(struct Array* vars,char* name,enum TYPE t,void* value){
    struct Var* this=(struct Var*)malloc(sizeof(struct Var));
    this->name=(char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(this->name,name);
    this->type=t;
    this->value=value;
    add_array(vars,VAR,this);
}
/**
 * Combierte a cadena el elemento.
 * @param  t     Especifica el tipo del elemento
 * @param  value Elemento
 * @return       Cadena dinamica. Recuerda liberar.
 */
char* to_string_value(enum TYPE t,void* value){
    char* out=NULL,
    *tmp=NULL;
    switch(t){
        case INT:
            tmp=(char*)alloca(60);
            itoa(*(int*)value,(char*)tmp,10);
            out=(char*)malloc(strlen((char*)tmp)+1);
            sprintf(out,"%s",tmp);
            break;
        case FLOAT:
            out=(char*)malloc(CLIMIT_FLOAT+1);
            sprintf(out, "%.*lf", CLIMIT_FLOAT-1,*(double*)value);
            break;
        case LONGINT:
            out=mpz_get_str(NULL,0,*(mpz_t*)value);
            break;
        case LONGFLOAT:
            out=mpf_get_str(NULL, NULL, 10, 0, *(mpf_t*)value);
        break;
        case CODES_BLOCKS:
        case STRING:
            out=(char*)malloc(strlen((char*)value)+1);
            strcpy(out,(char*)value);
            break;
        case ARRAY:
            out=printf_stack((struct Array*)value);
            break;
        case FUNCTION:
            tmp="(Native-Function)";
            out=(char*)malloc(strlen((char*)tmp)+1);
            strcpy(out,tmp);
            break;
        default:
            return NULL;
    }
    return out;
}
#endif