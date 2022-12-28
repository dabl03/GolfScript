#ifndef RUN_C
#define RUN_C 1
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "include\define.h"
    #include "include/str.h"
    #include "include/stack.h"
    #include "include/run.h"
    /**
     * @brief Función que analiza y ejecuta la instrucciones.
     * 
     * @param lines Lineas de codigo que interpretar. 
     * @param stack stack o pila en español.
     * @param vars Variables declaradas.
     * @return int error_codes.
    */
   /****
    * @todo Tambien falta colocar la biblioteca gmp.
    */
    int run(struct Array* lines,struct Array* stack,struct Array* vars){
        struct String codes_blocks={2,0,NULL};
        unsigned int init_codes_blocks=0;
        unsigned int sub_codes_blocks=0;
        /*Aqui debemos analizar char a char para saber si hay un simbolo diferente, leer el readme o el ejemplo para mas informacion.*/
        for (unsigned int i_line=0;i_line<lines->i;i_line++){
            unsigned int end;
            if(lines->value[i_line].type!=STRING){
                return -1;
            }
            const char* l=(char*)lines->value[i_line].value;
            unsigned int i_end=strlen(l);
            for(unsigned int i=0;i<i_end;i++){
                //Primero definimos nuestros signos constantes:
                if(sub_codes_blocks || l[i]=='{'){
                    if(l[i]=='{'){
                        sub_codes_blocks+=1;
                        init_codes_blocks=(init_codes_blocks)?init_codes_blocks:i;
                    }else if (l[i]=='}'){
                        sub_codes_blocks-=1;
                        if (!sub_codes_blocks){
                            str_add_str_init_end(&codes_blocks,l,init_codes_blocks,i);
                            add_array(stack,CODES_BLOCKS,(void*)codes_blocks.str);
                            continue;
                        }
                    }else if(IF_INIT_STRING(l[i])){
                        end=get_end_str(l,i,i_end);
                        i=(end)?end:i;
                        continue;
                    }else if(IF_INIT_COMENT(l[i])){
                        continue;
                    }
                }else if (IF_INIT_STRING(l[i])){//Llegamos a " o '
                    end=get_end_str(l,i,i_end);
                    end=(end)?end+1:strlen(l);
                    struct String str={end-i,0,(char*)malloc(sizeof(char)*((end-i)+1))};
                    str_add_str_init_end(&str,l,i,end);
                    add_array(stack,STRING,str.str);//Ingresamos 
                    i=end-1;//Necesitamos retroceder un caracter.
                    continue;
                }else if (IF_INIT_COMENT(l[i])){//Llegamos a un comentario.
                    break;
                }else if (l[i]==';'){
                    if (stack->i){
                        struct type_value* tv=pop_array(stack);
                        free(tv->value);
                    }
                }else if(l[i]==':'){//Para asignar una nueva variable.
                    //Si la cadena esta vacia significa que no es variable.
                    if (!stack->i){
                        printf("ERROR: La pila esta vacia.%c",ENDL);
                        break;
                    }
                    unsigned int end=0;i++;
                    char* name=get_name_var(l,&i,i_end);
                    int i_var=search_var(name,vars);
                    if(i_var!=-1){
                        struct Var* this_var=(struct Var*)vars->value[i_var].value;
                        setValue_tv(this_var,NULL,&stack->value[stack->i-1]);
                        free(name);
                        continue;
                    }
                    //No estubo definida antes.
                    struct Var* this_var=(struct Var*)malloc(sizeof(struct Var));
                    struct type_value* stack_var=&stack->value[stack->i-1];
                    this_var->value=NULL;
                    setValue_tv(this_var,name,stack_var);
                    add_array(vars,VAR,(void*)this_var);
                }else{
                    //Ahora vemos si existe la variable.
                    char* name=get_name_var(l,&i,0);
                    int i_var=search_var(name,vars);
                    if(i_var!=-1){//Significa que se encontro la variable.
                        struct Var* v=vars->value[i_var].value;
                        if (v->type==CODES_BLOCKS){
                            //No lo paso de una vez porque no se.
                            struct Array arr={0,0,NULL};
                            add_array(&arr,STRING,v->value);
                            run(&arr,stack,vars);
                            free(arr.value);
                        }else{
                            interpret(stack,vars,v);
                        }
                    }else if(is_num(name[0])){
                        int* v=(int*)malloc(sizeof(int));
                        *v=parseInt(name);
                        add_array(stack,INT,(void*)v);
                    }
                    free(name);
                }
            }
        }
        if (codes_blocks.str!=NULL){
            add_array(stack,CODES_BLOCKS,codes_blocks.str);
        }
        return 0;
    }
    /**
     * @brief Obtiene el nombre de la variable o un numero y retorna la cadena, ojo hay que liberarla.
     * 
     * @param search Cadena a buscar.
     * @param i indice actual a buscar.
     * @param end Fin de la cadena.
     * @return char* malloc/calloc/readlloc
     */
    char* get_name_var(const char* search,int* i,unsigned int end){
        struct String name = {3, 0, (char *)malloc(sizeof(char) * 3)};
        unsigned int i_2 = *i;
        end=(end)?end:strlen(search);
        if (is_abc(search[*i])){ // Si es un nombre lo modificamos buscamos hasta fin de linea o espacio.
            for (; i_2 < end && (is_abc(search[i_2]) || is_num(search[i_2])); i_2++);
            str_add_str_init_end(&name, search, *i, i_2--);//Necesitamos disminuir para que el bucle vaja para el siguiente caracter.
        }else if (is_num(search[*i])){ // Si es un numero.
            for (; i_2 < end && is_num(search[i_2]); i_2++);
            str_add_str_init_end(&name, search, *i, i_2--);
        }else{ // Espacio y otros simbolos.
            name.str[0] = search[*i];
            name.str[1] = '\0';
        }
        *i=i_2;
        return name.str;
    }
#endif