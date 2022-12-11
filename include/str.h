#ifndef STR_H
#define STR_H 1
unsigned int len(const char* str){
	unsigned int i=0;
	while(str[i]!='\0')
		i++;
	return i;
}
/**
 * @brief Funcion que busca la subcadena dentro de otra cadena, si str_1 < str_2 entonces str_1 es la sub-cadena. Si son iguales entonces solo compara las cadenas.
 * @param str_1 char* -- Cadena 1.
 * @param str_2 char* -- Cadena 2.
 * @return bool
*/
bool compare_sub_str(char* str_1,char* str_2){
	unsigned int i_s1=len(str_1);
	unsigned int i_s2=len(str_2);
	int c=0;//Veces que hubo coincidencia.
	char* c1=str_1;
	char* c2=str_2;
	if (i_s1==i_s2){// Como son iguales no tenemos porque buscar una sub-cadena, solo debemos ver si son iguales.
		for (unsigned int i=0;i<i_s1;i++){
			if (c1[i]!=c2[i])
				return false;
		}
		return true;
	}else if(i_s1<i_s2){//Si la cadena 1 es menos que la cadena 2 entonces la cadena 1 es la sub cadena que debemos buscar en la cadena 2.
		c1=str_2;
		c2=str_1;
		c=i_s1;
		i_s1=i_s2;
		i_s2=c;
		c=0;
	}
	for (unsigned int i=0;i<i_s1;i++){
		if (c1[i]==c2[c]){//Vemos si la sub cadena se encuentra dentro de la cadena.
			c++;
			if (c==i_s2)//Si se encuentra.
				return true;
		}else
			c=0;
	}
	return false;//no se encuentra.
}
/**
 * @brief Función que busca un char dentro de una cadena.
 * 
 * @param str  Cadena de donde buscar.
 * @param c Char que vamos a buscar.
 * @return true 
 * @return false 
 */
bool compare_sub_str(const char* str,unsigned const char c){
	for (unsigned int i=0;str[i]!='\0';i++)
		if ((unsigned int)str[i]==c)
			return true;//Se encontró:)
	return false;//No se encontro el char:(
}
#endif