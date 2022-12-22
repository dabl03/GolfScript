#ifndef RUN_CPP
#define RUN_CPP 1
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "include\define.h"
    #include "include/str.h"
    #include "include/stack.h"
    /**
     * @brief Función que analiza y ejecuta la instrucciones.
     * 
     * @param lines Lineas de codigo que interpretar. 
     * @param stack stack o pila en español.
     * @param vars Variables declaradas.
     * @return int error_codes.
    */
   /****
    * @todo Solo falta pasar esta y el archivo main.c a codigo c.
    * @todo Tambien falta colocar la biblioteca BigInt.
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
                    end=(end)?end:strlen(l);
                    struct String str={end-i,0,(char*)malloc(sizeof(char)*((end-i)+1))};
                    str_add_str_init_end(&str,l,i,end);
                    puts(str.str);
                    add_array(stack,STRING,str.str);//Ingresamos 
                    i=end;
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
                    struct String name={2,0,(char*)malloc(sizeof(char)*2)};
                    unsigned int i_2=++i;
                    if (is_abc(l[i])){//Si es un nombre lo modificamos buscamos hasta fin de linea o espacio.
                        for (;i_2<i_end && !is_abc(l[i_2]) && !is_num(l[i_2]);i_2++);
                        str_add_str_init_end(&name,l,i,i_2);
                    }else if (is_num(l[i])){//Si es un numero.
                        for(;i_2<i_end && is_num(l[i_2]);i_2++);
                        str_add_str_init_end(&name,l,i,i_2);
                    }else{//Espacio y otros simbolos.
                        name.str[0]=l[i];
                        name.str[1]='\0';
                    }
                    i=i_2-1;//Actualizamos la posición.
                    unsigned int end=0;
                    int i_var=search_var(name.str,vars,&end);
                    if(i_var!=-1){
                        struct Var* this_var=(struct Var*)vars->value[i_var].value;
                        struct Var* stack_var=(struct Var*)stack->value[stack->i-1].value;
                        setValue(this_var,stack_var->type,stack_var->value);
                        continue;
                    }
                    //No estubo definida antes.
                    struct Var* this_var=(struct Var*)malloc(sizeof(struct Var));
                    struct Var* stack_var=((struct Var*)(stack->value[stack->i-1].value));
                    this_var->value=NULL;
                    setValue(this_var,stack_var->type,stack_var->value);
                    stack_var->name=name.str;
                    add_array(vars,VAR,(void*)stack_var);
                }else{
                    //Ahora vemos si existe la variable.
                    unsigned int end=0;
                    int i_var=search_var_init_end(l,i,vars,&end);
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
                        i=end;
                    }else if(is_num(l[i])){
                        struct String name={2,0,(char*)malloc(sizeof(char)*2)};
                        int* v=(int*)malloc(sizeof(int));
                        unsigned int i_2;
                        for(;i_2<i_end && is_num(l[i_2]);i_2++);
                        str_add_str_init_end(&name,l,i,--i_2);
                        i=i_2;
                        *v=parseInt(name.str);
                        free(name.str);
                        add_array(stack,INT,(void*)v);
                    }
                }
            }
        }
        if (codes_blocks.str!=NULL){
            add_array(stack,CODES_BLOCKS,codes_blocks.str);
        }
        return 0;
    }
#endif