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
     * {TODO}:{ Recuerda que el array debe ser ejecutado por una nueva pila. }
     * {TODO}: {Usar printf_stack para obtener la cadena para la funcion to_string_value }
     * @brief Función que analiza y ejecuta la instrucciones.
     * 
     * @param lines Lineas de codigo que interpretar. 
     * @param stack stack o pila en español.
     * @param vars Variables declaradas.
     * @return int error_codes.
    */
   /****
    * @todo Tambien falta colocar la biblioteca gmp.
    * @todo  get_str_token hacer que en vez de que se ejecute la funcion linea por linea esta funcion get_str_token ordene todo mas la funcion add*
    * @todo  Al crear una nueva cadena entra en un bucle infinito, revisar porque.
    * @todo  Es muy lento el programa, sobretodo cuando el entero es muy grande, arreglar
    * Se que hay otra cosa por hacer pero no me acuerdo.
    */
    int run(struct Array* lines,struct Array* stack,struct Array* vars){
        struct String codes_blocks={0,0,NULL};//Importante iniciarlo en null, esto nos dirá si es un bloque de codigo.
        U_INT sub_codes_blocks=0,
		tmp_istr=0;
        char* esc_str=NULL,
        *tmp_str=NULL;
        /*Aqui debemos analizar para saber si hay un simbolo diferente, leer el readme o el ejemplo para mas informacion.*/
        for (U_INT i_line=0;i_line<lines->i && !quit;i_line++){
            U_INT end;
            if(lines->value[i_line].type!=STRING){
                return -1;
            }
            const char* l=(char*)lines->value[i_line].value;
            U_INT i_end=strlen(l);
            for(U_INT i=0;i<i_end;i++){
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
						end=(end)?end:i_end;
                        tmp_str=get_sub_str(l,i,end+1);
                        char* scape_=get_str_escp(tmp_str);
                        str_add_str_init_end(&codes_blocks,scape_,0,0);
                        free(tmp_str);
                        free(scape_);
                        i=end;
                        continue;
                    }else if(IF_INIT_COMENT(l[i])){
                        continue;
                    }else if(IF_ENDL(l[i])){//Cambiamos los saltos de lineas por espacio.
                            str_add_char(&codes_blocks,' ');
                        continue;
                    }
                    str_add_char(&codes_blocks,l[i]);
                }else if (IF_INIT_STRING(l[i])){//Llegamos a " o '
                    end=get_end_str(l,i,i_end);
					end=(end)?end:i_end;
                    tmp_str=get_sub_str(l,i,end);//Si la cadena es '' se combierte a ""
                    esc_str=get_str_escp(tmp_str);
                    free(tmp_str);
                    add_array(stack,STRING,esc_str);//Ingresamos 
                    i= end;//Recuerda que si end=0 entonces se toma toda la cadena.
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
                    free(name);
                }else if(IF_ENDL(l[i])){
                    continue;//Por velocidad. Ignoramos los saltos de lineas como el interprete original.
				}else if(l[i]=='['){//Arrays:
                    tmp_str=get_ie_block(l,i,']',&tmp_istr);
                    //Nueva linea y nuevo stack.
                    struct Array line={0,0,0};
    				struct Array* sub_stack=(struct Array*)malloc(sizeof(struct Array));
    				sub_stack->i=0;
    				sub_stack->max=0;
    				sub_stack->value=NULL;
    				//Iniciamos la linea.
    				add_array(&line,STRING,tmp_str);
    				//Ejecutamos y agregamos al stack el array.
                    run(&line,sub_stack,vars);
                    add_array(stack,ARRAY,(void*)sub_stack);
                    delete_array(&line);//Liberamos memoria
                    //Terminamos.
					i=tmp_istr;
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
        }else if(IF_INIT_STRING(search[*i])){

        }else{ // Espacio y otros simbolos.
            name.str[0] = search[*i];
            name.str[1] = '\0';
        }
        *i=i_2;
        return name.str;
    }
    /**Elimina los excesivos espacio dejando solo uno para separar cada palabra.
     * Tambien combertimos los saltos de lineas en espacio y se hace lo mismo que el anterior.
     * Nota: Tomamos en cuenta las cadenas ignorando todo lo que este adentro.
     * @param str Cadena
     * @param init { comienzo de la cadena a revisar }
     * @param end Fin de la cadena. si es 0 se busca el final.
     * @return     Cadena tratada para prevenir los excesos de espacio y los saltos de linea. Retorna cadena dinamica, liberar.
     */
    char* get_str_token(char* str,U_INT init,U_INT end){
        NEW_STRING(out,20);
        unsigned char space=0;
        end=(end)?end:strlen(str);
        U_INT tmp_i=0;
        for(U_INT i=init;i<end;i++){
            if (str[i]=='\n' || str[i]==' ')
                space=TRUE;
            else{
                if (space){
                    cadd_add_leftover(&out,' ');
                    space=FALSE;
                }
                cadd_add_leftover(&out,str[i]);
                if (IF_INIT_STRING(str[i])){
                    tmp_i=get_end_str(str,i,end);
                    tmp_i=(tmp_i)?tmp_i:end;
                    out.count--;//Porque necesitamos que ignore la entre comilla anterior.
                    str_add_str_init_end(&out,str, i,tmp_i);
                    i=tmp_i;
                }else if (IF_INIT_COMENT(str[i])){
                    out.count--;
                    for (;i<end && str[i]!='\n';i++);
                    continue;
                }
            }
        }
        cadd_add_leftover(&out,'\0');
        return (char*)realloc(out.str,out.count);
    }
    /**
     * get init end block. Obtiene un bloque buscando en la cadena, desde init hasta conseguirn end block.
     * Nota: El inicio del bloque se obtiene deacuerdo a init: char inicio=input[init];.
     * No sirve para obtener cadena, en su lugar use get_sub_str o get_end_str.
     * @param  input   Cadena a buscar
     * @param  init    Desde donde empezar
     * @param  end     El caracter a buscar para ver el final.
     * @param  out_end Aqui almacenaremos el indice final para que fuera de la funcion se sepa.
     * @return         retorna el bloque buscado sin el inicio y el final. Recordar liberar cadena.
     */
    char* get_ie_block(const char* input,const U_INT init, const char end, U_INT* out_end){
    	NEW_STRING(out,20);
    	char cinit=input[init];
    	U_INT i=init+1,
    	sub=1;
    	U_INT tmp_i=0,
    	iend=strlen(input);
    	char* tmp_str=NULL;
    	for (;i<iend;i++){

    		if (input[i]==cinit)
    			sub++;
    		else if (input[i]==end){
    			sub--;
    			if (sub==0)
    				break;
    		}
    		switch(input[i]){
    			case '"': /*OR*/ case '\'':
    				tmp_i=get_end_str(input,i,iend);
	    			tmp_str=get_sub_str(input,i,tmp_i);
	    			str_add_str_init_end(&out,tmp_str,0,0);
	    			i=(tmp_i)?tmp_i:iend;
	    			free(tmp_str);
	    			if (i+1<iend AND input[i+1]!=' ')
	    				cadd_add_leftover(&out,' ');
	    			break;
	    		case COMMENT:
	    			for (;input[i]!='\n' AND input[i]!='\0';i++);//Se ignora todo
	    		case '\n':
	    			if (input[i-1]!=' ')
	    				cadd_add_leftover(&out,' ');
	    			break;
	    		default:
	    			cadd_add_leftover(&out,input[i]);
    		}
    		
    	}
    	//Para mas velocidad no llamamos a cadd_add_leftover.
    	if (out.count+sub+1>out.max){//Ya no necesitamos la variable max de la estructura.
    		out.str=(char*)realloc(out.str,out.count+sub+2);
    	}
    	//Si sigue abierto la cerramos.
    	while(sub>0){
    		out.str[out.count++]=end;
    		sub--;
    	}
    	if (out_end!=NULL)
	    	*out_end=i;
    	out.str[out.count++]='\0';//Retornamos la cadena sin el inicio o el fin del bloque.
    	return (char*)realloc(out.str,out.count);
    }
#endif