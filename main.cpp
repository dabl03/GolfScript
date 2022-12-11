#include <fstream>
#include <iostream>
#include <string>//La clase string la sacamos desde el 5 circulo del infierno.
#include <vector>
#include <regex>
#include "include/str.h"
using namespace std;
int interprete();
void config_all(vector<string> opciones);
int main(int argc, char *argv[])
{
	ios_base::sync_with_stdio(false);//Aumenta la velocidad de la entrada y salida estandar.
	string linea;
	if (argc > 1)
	{
		vector<string> params, path_files;

		for (int i = 1; i < argc; i++)
		{
			string opcion = argv[i];
			if (opcion.find('-') == string::npos)
			{
				path_files.push_back(opcion.c_str());
				continue;
			}
			params.push_back(opcion.substr(1));
		}
        if (params.empty()){
            config_all(params);
        }
		if (path_files.empty())
		{
            return interprete();
        }else{
			ifstream file(path_files[0].c_str());
            vector<string> lineas;
			if (!file.good())
			{
				cout<<"El archivo no existe, revise la ruta."<<endl;
				exit(EXIT_FAILURE);
			}

			while (getline(file, linea))
				lineas.push_back(linea);
			file.close();
		}
	}
	interprete();
	return 0;
}
void config_all(vector<string> opciones){
    for(auto str : opciones){
        cout<<str;
    }
}
int interprete(){

}