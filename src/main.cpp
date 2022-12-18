#include <fstream>
#include <iostream>
#include <string>//La clase string la sacamos desde el 5 circulo del infierno.
#include <vector>
#include <regex>
#include "./include/str.h"
#include "./include/define.h"
#include "./include/run.h"
#define ARCHIVO_CON_VARIABLES_DEL_PREDECTERMINADAS "./include/vars.gs"
using namespace std;
int interprete(vector<Var> stack,vector<Var> vars);
void config_all(vector<string> opciones);
int main(int argc, char *argv[])
{
	ios_base::sync_with_stdio(false);//Aumenta la velocidad de la entrada y salida estandar.
	string linea;
	vector<Var> stack,vars;
	if (argc > 1)
	{
		vector<string> params, path_files;

		for (int i = 1; i < argc; i++)
		{
			string opcion = argv[i];
			if (opcion.find('-') == string::npos)
			{
				path_files.emplace_back(opcion.c_str());
				continue;
			}
			params.emplace_back(opcion.substr(1));
		}
        if (!params.empty()){
            config_all(params);
        }
		if (path_files.empty())
		{
            return interprete(stack,vars);
        }else{
			ifstream file(path_files[0].c_str());
            vector<string> lineas;
			if (!file.good())
			{
				cout<<"El archivo no existe, revise la ruta."<<endl;
				exit(EXIT_FAILURE);
			}

			while (getline(file, linea))
				lineas.emplace_back(linea);
			file.close();
			analyze_and_run(lineas,stack,vars);
		}
	}else{
		return interprete(stack,vars);
	}
	return 0;
}
void config_all(vector<string> opciones){
    for(string str : opciones){
        /****
         * @todo Hacer opciones de configuracion para configurarlas aqui.
         * Me gustaria una que esamble y compile el archivo para que sea mas facil analizarlo.
         * Otra -o
         * --help
         * --v o --version
         * --license
        */
    }
}
int interprete(vector<Var> stack,vector<Var> vars){
	int sub = 0;	//para cambiar de >> a .. cuando hay una condición.
	char c_linea[BUFFER];
	vector<string> lineas;
	cout<<"Golfscript Interactive Mode"<<endl;
	
	while (true)
	{
		if (sub) // Esta anidando algo.
		{
			char* space=new char[sub];
			unsigned int i=0;
			for (;i<sub;i++)
				space[i]=' ';
			space[i]='\0';
			cout<<space;
			delete[] space;
		}
		cout<<'>';
		try
		{
			cin.getline(c_linea,BUFFER);
			if (compare_sub_str(c_linea,'{')){//Vemos si hay anidamiento.
				sub += 1;
			}else if (compare_sub_str(c_linea,"}")){//Fin del anidamiento.
				if(sub <= 0){//Espera no hubo nada que desanidar ;(
					cout << "ERROR \nno hay anidamiento que quitar";
					sub = 0;
					continue;
				}
				sub -= 1;
			}

			lineas.emplace_back(string(c_linea));
			if(sub == 0){//Podemos interpretar linea a linea.
				analyze_and_run(lineas,stack,vars);
				lineas.clear();
				string output=Var::printf_stack(stack);
				cout<<"[ "<<output<<']'<<endl;
			}
		}
		catch (const std::exception& e)
		{
			cerr << e.what() << c_linea << endl;
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}