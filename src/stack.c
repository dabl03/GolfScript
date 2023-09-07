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
 * @param arr Array
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
        switch(arr->value[i].type){
            case VAR:
                delete_var((struct Var*)arr->value[i].value);
                free(arr->value[i].value);
                break;
            case FUNCTION:break;//Las funciones no son necesario liberar.
            case ARRAY:
                delete_array((struct Array*)arr->value[i].value);
            break;
            case LONGINT:
            case LONGFLOAT:
                mpz_clear(*(mpz_ptr*)arr->value[i].value);
                break;
            //No hay forma diferente para liberar.
            case INT:
            case CODES_BLOCKS:
            case STRING:
            case FLOAT:
            default:
                free(arr->value[i].value);
        }
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
    void* tmp;
    for (unsigned int i=0;i<arr->i;i++){
        switch(arr->value[i].type){
            case VAR:
                break;
            case ARRAY:
                break;
            case LONGINT:
                
                break;
            case LONGFLOAT:///Caracteristica no disponible.
                break;
            case INT:
                tmp=malloc(sizeof(int));
                *(int*)tmp=*(int*)arr->value[i].value;
                add_array(out,INT,tmp);
                break;
            case CODES_BLOCKS:
            case STRING:
                tmp=malloc(strlen((char*)arr->value[i].value)+1);
                strcpy((char*)tmp,(char*)arr->value[i].value);
                add_array(out,(arr->value[i].type==STRING)?STRING:CODES_BLOCKS,tmp);
                break;
            case FLOAT:
                tmp=malloc(sizeof(double));
                *(double*)tmp=*(double*)arr->value[i].value;
                add_array(out,INT,tmp);
                break;
            default:
                perror("Error interno de la app, me falto un tipo de dato por verificar en la funcion copy_array.");
                printf("\nEl tipo se llama %s\n",get_name_type(arr->value[i].type) );
                exit(-2);
        }
    }
    return NULL;
}
/**
 * @brief Aqui configuramos y si ya estubo definida(tv->value!=NULL)
 * la liberamos para redefinir el valor de la variable.
 * 
 * @param v Variable a configurar.
 * @param name Nombre de la variable.
 * @param tv El tipo y el valor de la variable.
 */
void setValue_tv(struct Var* v,char* name,struct type_value* tv){
    name=(name!=NULL)?name:v->name;
    //Por seguridad no podemos usar el nombre original.(No liberar una cadena que esa parte del ejecutable.)
	char* str=(char*)malloc(strlen(name)+1);
    strcpy(str,name);
    if (v->value != NULL){
        strcpy(str,name);
        delete_var(v);
    }
    v->name=name;
    switch(tv->type){
        case INT:
            v->value=malloc(sizeof(int));
            *(int*)v->value=*(int*)tv->value;
            break;
        case LONGINT:
            v->value=malloc(sizeof(mpz_t));
			mpz_init(*(mpz_t*)v->value);//Importante.
            mpz_set(*(mpz_t*)v->value,*(mpz_t*)tv->value);
            break;
        case ARRAY://TODO ver como copiarlo----------------------------------------------------------------
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
    v->type = tv->type;
    v->name=(name)?name:v->name;
}
/**
 * @brief Liberamos la memoria reserbada para almacenar una variable.
 * @param v Variable a eliminar.
 */
void delete_var(struct Var* v){
    struct Array sub_arr={1,1,(struct type_value*)malloc(sizeof(struct type_value))};
    sub_arr.value->type=v->type;
    sub_arr.value->value=v->value;
    free(v->name);
    delete_array(&sub_arr);
    v->value=NULL;
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
    case FUNCTION: //Usamos funciones dentro del mismo programa para esta.
        ((unsigned short (*)(struct Array* stack,struct Array* vars,char* extend))v->value)(stack,var,"");
        return NULL;
    case CODES_BLOCKS://Interpretamos el bloque de código.
        return (char*)v->value;
    case STRING:
        value=malloc(sizeof(char)*(strlen((char*)v->value)+1));
        strcpy((char*)value,(char*)v->value);
		break;
    case ARRAY:////////////////////////////////////////////////////////////////TODO Ver como hacerlo
        break;
	case LONGINT://Copiamos el entero largo a otro.
		value=malloc(sizeof(mpz_t));
		mpz_init(*(mpz_t*)value);//Importante.
		mpz_set(*(mpz_t*)value, *(mpz_t*)v->value);
		break;
        /*default:
            printf("Error tipo %s no tratado en la función setValue_tv\n",get_name_type(tv->type) );
            exit(-3);*/
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
    output[0]='\0';
    unsigned int len=1;
    for(unsigned int i=0;i<stack->i;i++){
        switch(stack->value[i].type){
            case INT:
                len+=8;
                output=(char*)realloc(output,sizeof(char)*len);
                sprintf(output,"%s%d ",output,*(int*)stack->value[i].value);
                break;
            case ARRAY:
                a_out=printf_stack((struct Array*)stack->value[i].value);
                len+=strlen(a_out)+6;
                output=(char*)realloc(output,len);
                sprintf(output,"%s[ %s ] ",output,a_out);
                free(a_out);
                break;
            case CODES_BLOCKS://Recuerda todo despues de esto es string.
                len+=strlen((char*)stack->value[i].value)+1;
                output=(char*)realloc(output,sizeof(char)*len);
                sprintf(output,"%s%s ",output,(char*)stack->value[i].value);
                break;
            case STRING:
                a_out=get_str_nescp((char*)stack->value[i].value);
                len+=strlen(a_out)+3;
                output=(char*)realloc(output,sizeof(char)*len);
                sprintf(output,"%s\"%s\" ",output,a_out);
                free(a_out);
				break;
			case LONGINT:
				a_out=mpz_get_str(NULL,0,*(mpz_t*)stack->value[i].value);
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
            out=(char*)malloc(CLIMIT_INT+1);
            sprintf(out,"%d",*(int*)value);
            break;
        case LONGINT:
            out=mpz_get_str(NULL,0,*(mpz_t*)value);
            break;
        case FLOAT:
            out=(char*)malloc(CLIMIT_FLOAT+1);
            sprintf(out, "%.*lf", CLIMIT_FLOAT-1,*(double*)value);
            break;
        //case LONGFLOAT:break;/**@Nota: para gmp double.*///Esta en comentario porque quiero que el compilador me diga donde esta.
        case CODES_BLOCKS:
        case STRING:
            out=(char*)malloc(strlen((char*)value)+1);
            strcpy(out,(char*)value);
            break;
        case ARRAY:
            /**{TODO}:{Recuerda de llamar a esta funcion por cada elemento del array.}*/
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