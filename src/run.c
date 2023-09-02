#ifndef RUN_C
#define RUN_C 1
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <limits.h>
    #include <gmp.h>
    #include "include/define.h"
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
        struct String codes_blocks={0,0,NULL};//Importante iniciarlo en null, esto nos dirá si es un bloque de codigo.
        unsigned int sub_codes_blocks=0;
        char* esc_str=NULL;
        /*Aqui debemos analizar para saber si hay un simbolo diferente, leer el readme o el ejemplo para mas informacion.*/
        for (unsigned int i_line=0;i_line<lines->i && !quit;i_line++){
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
                    }else if (l[i]=='}'){
                        sub_codes_blocks-=1;
                        if (!sub_codes_blocks){
                            str_add_char(&codes_blocks,'}');//Nota esto no debe ser asi porque cuando el bucle termina se pone diferentes caracteres. A juro hay que appenarlos.
                            add_array(stack,CODES_BLOCKS,(void*)codes_blocks.str);
                            sub_codes_blocks=0;
                            codes_blocks.str=NULL;//Ahora la referencia está dentro de la pila.
                            continue;
                        }
                    }else if(IF_INIT_STRING(l[i])){
                        end=get_end_str(l,i,i_end);
                        str_add_str_init_end(&codes_blocks,l,i,end+1);
                        i=(end)?end:i;
                        continue;
                    }else if(IF_INIT_COMENT(l[i])){
                        continue;
                    }else if(IF_ENDL(l[i])){//Cambiamos los saltos de lineas por espacio.
                        if (i)
                            str_add_char(&codes_blocks,' ');
                        continue;
                    }
                    str_add_char(&codes_blocks,l[i]);
                }else if (IF_INIT_STRING(l[i])){//Llegamos a " o '
                    end=get_end_str(l,i,i_end);
                    end=(end)?end+1:strlen(l);
                    struct String str={end-i,0,(char*)malloc(sizeof(char)*((end-i)+1))};
                    str_add_str_init_end(&str,l,i+1,end-1);
                    esc_str=get_str_escp(str.str);
                    add_array(stack,STRING,esc_str);//Ingresamos 
                    i=end-1;//Necesitamos retroceder un caracter.
                    free(str.str);
                    continue;
                }else if (IF_INIT_COMENT(l[i])){//Llegamos a un comentario.
                    break;
                }else if (l[i]==';'){
                    if (stack->i){
                        struct type_value* tv=pop_array(stack);
                        free(tv->value);
                    }else{
                        puts("Warnign: La pila esta vacia.");
                    }
                }else if(l[i]==':'){//Para asignar una nueva variable.
                    //Si la cadena esta vacia significa que no es variable.
                    if (!stack->i){
                        printf("ERROR: La pila esta vacia.%s",ENDL);
                        break;
                    }
                    if(IF_ENDL(l[i+1]))//Por velocidad. Innoramos los saltos de lineas como el interprete original.
                        continue;
                    i++;
                    char* name=get_name_var(l,&i,i_end);
                    int i_var=search_var(name,vars);
                    if(i_var!=-1){//Consiguio, se reemplaza.
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
                }else if(IF_ENDL(l[i])){
                    continue;//Por velocidad. Innoramos los saltos de lineas como el interprete original.
				}else if(l[i]=='['){//Arrays:
					perror("Caracteristica que todavia esta en construccion.\n");
					exit(0);
				}else{
                    //Ahora vemos si existe la variable.
                    char* name=get_name_var(l,&i,0);
                    int i_var=search_var(name,vars);
                    if(i_var!=-1){//Significa que se encontro la variable.
                        struct Var* v=vars->value[i_var].value;
                        if (v->type==CODES_BLOCKS){
                            //No lo paso de una vez porque no se.
                            unsigned int len=strlen((char*)v->value);
                            char* code=(char*)malloc(sizeof(char)*(len+1));
                            struct Array arr={0,0,NULL};
                            strncpy(code,&((char*)v->value)[1],len-2);
                            code[len-2]='\0';
                            add_array(&arr,STRING,code);
                            run(&arr,stack,vars);
                            free(arr.value);
                            free(code);
                        }else{
                            interpret(stack,vars,v);
                        }
                    }else if(is_num(name[0])){
                        unsigned int len=strlen(name)-1;
                        if (len<=CLIMIT_INT){
                            int* v=(int*)malloc(sizeof(int));
                            *v=parseInt(name);
                            add_array(stack,INT,(void*)v);
                        }else{
                            mpz_t* n=(mpz_t*)malloc(sizeof(mpz_t));
                            mpz_init_set_str(*n,name,0);
                            add_array(stack,LONGINT,n);
                        }
                    }
                    free(name);
                }
            }
        }
        if (codes_blocks.str!=NULL){//Si quieres lo reemplazas con if (sub_codes_blocks){
            str_add_char(&codes_blocks,'}');
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
    char* get_name_var(const char* search,unsigned int* i,unsigned int end){
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