#ifndef RUN_CPP
#define RUN_CPP 1
    #include <string>
    #include <vector>
    #include "./head/define.h"
    #include "./head/str.h"
    #include "./head/stack.h"
    using namespace std;
    int analizar(const vector<string>& lines,vector<Var>& stack,vector<Var> vars){
        /*Aqui debemos analizar char a char para saber si hay un simbolo diferente, leer el readme o el ejemplo para mas informacion.*/
        for (string line: lines){
            unsigned int end;
            const char* l=line.c_str();
            for(unsigned int i=0;l[i]!='\0';i++){
                //Primero definimos nuestros signos constantes:
                if (IF_INIT_STRING(l[i])){//Llegamos a " o '
                    end=get_end_str(l,i,line.length());
                    stack.push_back( Var( STRING,new string(line.substr(i,end)) ) );//Ingresamos 
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
                        for (;!is_abc(l[i_2]) && !is_num(l[i_2]);i_2++){
                            name_vars+=l[i_2];
                        }
                    }else if (is_num(l[i])){//Si es un numero.
                        string num="";
                        for(;is_num(l[i_2]);i_2++){
                            name_vars+=l[i_2];
                        }
                    }else{//Espacio y otros simbolos.
                        name_vars+=l[i];
                    }
                    i=i_2;//Actualizamos la posición.
                    unsigned int end=0;
                    int i_var=search_var(name_vars.c_str(),0,vars,&end);
                    if(i_var!=-1){
                        Var* this_var=&vars[i_var];
                        string codes_block=this_var->interpret(stack,vars);
                        if (this_var->var.type==CODES_BLOCKS){
                            //No lo paso de una vez porque no se.
                            vector<string> c;
                            c.push_back(codes_block);
                            analizar(c,stack,vars);
                        }
                        continue;
                    }
                    //No estubo definida antes.
                    vars.push_back(Var(name_vars,VAR,(void*)&stack.back()));
                }else{
                    //Ahora vemos si existe la variable.
                    unsigned int end=0;
                    int i_var=search_var(l,i,vars,&end);
                    if(i_var!=-1){//Significa que se encontro la variable.
                        Var* this_var=&vars[i_var];
                        if (this_var->var.type==CODES_BLOCKS){
                            vector<string> arg;
                            arg.push_back((*(string*)this_var->var.value).substr(1,arg.back().length()-1));
                            analizar(arg,stack,vars);
                        }else{
                            this_var->interpret(stack,vars);
                        }
                        i=end;
                    }
                    
                }
            }
        }
        return 0;
    }
    int run(vector<Var> definiciones){
        for (Var defi:definiciones){
            switch(defi.var.type){
                case VAR:
                case STRING:
                case INT:
                case FUNCTION:
                default:
                    return ERROR_DEFICCION_NO_VALIDA;
            }
        }
        return 0;
    }
#endif