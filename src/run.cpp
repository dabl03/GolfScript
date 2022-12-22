#ifndef RUN_CPP
#define RUN_CPP 1
    #include <string>
    #include <cstdlib>
    #include <vector>
    #include <iostream>//No es necesario, solo necesito ver algo rapido.
    //C
    #include <stdlib.h>
    #include "./include/define.h"
    #include "./include/str.h"
    #include "./include/stack.h"
    using namespace std;
    /**
     * @brief Función que analiza y ejecuta la instrucciones.
     * 
     * @param lines Lineas de codigo que interpretar. 
     * @param stack stack o pila en español.
     * @param vars Variables declaradas.
     * @return int error_codes.
    */
    int analyze_and_run(const vector<string>& lines,vector<Var>& stack,vector<Var> vars){
        string codes_blocks="";
        unsigned int sub_codes_blocks=0;
        /*Aqui debemos analizar char a char para saber si hay un simbolo diferente, leer el readme o el ejemplo para mas informacion.*/
        for (string line: lines){
            unsigned int end;
            const char* l=line.c_str();
            unsigned int i_end=line.length();
            for(unsigned int i=0;i<i_end;i++){
                //Primero definimos nuestros signos constantes:
                if(sub_codes_blocks || l[i]=='{'){
                    if(l[i]=='{'){
                        sub_codes_blocks+=1;
                    }else if (l[i]=='}'){
                        sub_codes_blocks-=1;
                        if (!sub_codes_blocks)
                            stack.emplace_back(codes_blocks+'}');
                            codes_blocks="";
                        continue;
                    }else if(IF_INIT_STRING(l[i])){
                        end=get_end_str(l,i,i_end);
                        codes_blocks+=line.substr(i,end);
                        i=end;
                        continue;
                    }else if(IF_INIT_COMENT(l[i])){
                        continue;
                    }
                    codes_blocks+=l[i];
                }else if (IF_INIT_STRING(l[i])){//Llegamos a " o '
                    end=get_end_str(l,i,i_end);
                    stack.emplace_back(STRING,new string( line.substr(i,end) ));//Ingresamos 
                    i=end;
                    continue;
                }else if (IF_INIT_COMENT(l[i])){//Llegamos a un comentario.
                    continue;
                }else if (l[i]==';')
                    stack.pop_back();
                else if(l[i]==':'){//Para asignar una nueva variable.
                    //Si la cadena esta vacia significa que no es variable.
                    string name_vars="";
                    unsigned int i_2=++i;
                    if (is_abc(l[++i])){//Si es un nombre lo modificamos buscamos hasta fin de linea o espacio.
                        for (;i_2<i_end && !is_abc(l[i_2]) && !is_num(l[i_2]);i_2++){
                            name_vars+=l[i_2];
                        }
                    }else if (is_num(l[i])){//Si es un numero.
                        string num="";
                        for(;i_2<i_end && is_num(l[i_2]);i_2++){
                            name_vars+=l[i_2];
                        }
                    }else{//Espacio y otros simbolos.
                        name_vars+=l[i];
                    }
                    i=i_2;//Actualizamos la posición.
                    unsigned int end=0;
                    int i_var=Var::search_var(name_vars,0,vars,&end);
                    if(i_var!=-1){
                        Var* this_var=&vars.at(i_var);
                        string codes_block=this_var->interpret(stack,vars);
                        if (this_var->type==CODES_BLOCKS){
                            //No lo paso de una vez porque no se.
                            vector<string> c;
                            c.emplace_back(codes_block);
                            analyze_and_run(c,stack,vars);
                        }
                        continue;
                    }
                    //No estubo definida antes.
                    vars.emplace_back(name_vars,VAR,(void*)&stack.back());
                }else{
                    //Ahora vemos si existe la variable.
                    unsigned int end=0;
                    int i_var=Var::search_var(line,i,vars,&end);
                    if(i_var!=-1){//Significa que se encontro la variable.
                        Var* v=&vars.at(i_var);
                        if (v->type==CODES_BLOCKS){
                            vector<string> arg;
                            arg.emplace_back(*(string*)v->value).substr(1,arg.back().length()-1);
                            analyze_and_run(arg,stack,vars);
                        }else{
                            v->interpret(stack,vars);
                        }
                        i=end;
                    }else if(is_num(l[i])){
                        string number="";
                        int* v=(int*)malloc(sizeof(int));
                        for(;i<i_end && is_num(l[i]);i++){
                            number+=l[i];
                        }
                        *v=parseInt(number);
                        stack.emplace_back(INT,(void*)v);
                        cout<<" string number: "<<number<<"int parseInt(number): "<<parseInt(number)<<"\n &v: "<<v<<"\n *v: "<<*v<<"\n stack.value:"<<*(int*)stack.back().value<<endl;
                    }
                }
            }
        }
        if (sub_codes_blocks)
            stack.emplace_back(codes_blocks);
        return 0;
    }
#endif