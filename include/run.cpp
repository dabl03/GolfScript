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
                    unsigned int end;
                    int i_var=search_var(l,i+1,vars,&end);
                    if (i_var!=-1){
                        vars[i_var].setValue(stack.back().var.type,stack.back().var.value);
                    }else{
                        /*@todo: buscar hasta conseguir espacio o alguna cosa ya definidas que no sea numero o letras.*/
                    }
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
                            this_var->interpret(stack);
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