#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#ifdef _WIN32
	#include <windows.h>
	#define stat _stat
	#define S_IFDIR _S_IFDIR
	#define S_IFREG _S_IFREG
#else
	#include <sys/stat.h>
#endif
#ifndef MAX_PATH
	 #define MAX_PATH 100
#endif
#define CHANGE_CASE(c) (isupper(c) ? tolower(c) : toupper(c))
#define uint unsigned int
#define FALSE 0
#ifndef MAIN // Por si no se importa este archivo.
	#define MAIN 1
	#include "../test/memory.c"
	#undef MAIN
#else
	#include "../test/memory.c"
#endif
#include "../test/include/memory.h"
// Codigo modificado de:
// - https://github.com/Nicolastolinii/List-Directories/blob/main/listar.c
/// @todo: Probar en windows.
typedef struct StackPath{
	struct StackPath* next;
	char* path;
} StackPath;
char* extensions[]={
	"log",
	(char*)NULL // Indica el final.
};
void Push(StackPath** stack, char* path){
	StackPath* new_stack=(StackPath*)malloc(sizeof(StackPath));
	if (*stack!=NULL){
		new_stack->path=(*stack)->path;
		new_stack->next=(*stack)->next;
		(*stack)->next=new_stack;
	}else{
		new_stack->next=NULL;
		*stack=new_stack;
	}
	(*stack)->path=path;
}
char* Pop(StackPath** stack){
	if (stack==NULL || *stack==NULL)
		return NULL;
	char* out=(*stack)->path;
	StackPath* next=(*stack)->next;
	free(*stack);
	if ((*stack)->next!=NULL){
		(*stack)=next;
	}else
		(*stack)=NULL;
	return out;
}
void Free_stack(StackPath* stack){
	StackPath* next=stack;
	while(next!=NULL){
		stack=next;
		next=stack->next;
		free(stack->path);
		free(stack);
	}
}
void repeat_chr(uint count, char chr){
	for(;count!=0;count--)
		putchar(chr);
}
unsigned char strcmp_uncase_sensible(const char* str1, const char* str2){
	char* chr_1=(char*)str1;
	char* chr_2=(char*)str2;
	const uint dif=(uint)'z'-'A';
	while (*chr_1!='\0'){
		if (*chr_1!=*chr_2){
			if (CHANGE_CASE(*chr_1)!=*chr_2)
				return FALSE;
		}
		chr_1++;
		if (*chr_2=='\0')
			return FALSE;
		chr_2++;
	}
	return *chr_2=='\0';
}
StackPath* lista_directorio(char *nombre, char** extensiones);
char* get_file(char* path);
// Si otro archivo con main quiere importar este.
// entonces no permitimos que main exista.
#ifndef MAIN
	#define view_log main
#endif
bool is_directory(const char* path){
	#if _WIN32
	return GetFileAttributesA(path) & FILE_ATTRIBUTE_DIRECTORY;
	#else
	struct stat s;
	if (stat(path, &s)==0)
		return s.st_mode&S_IFDIR;
	#endif
	return false;
}
bool is_file(const char* path){
	#if _WIN32
	return GetFileAttributesA(path) & FILE_ATTRIBUTE_ARCHIVE;
	#else
	struct stat s;
	if (stat(path, &s)==0)
		return s.st_mode&S_IFREG;
	#endif
	return false;
}
int view_log(int argc, char const *argv[]){
	char** directory=(char**)malloc(sizeof(char**)*(argc+3));
	char** files=(char**)malloc(sizeof(char**)*(argc+3));
	StackPath* all_file;
	char* out_file;
	uint i=0;
	uint i_f=0;
	uint len_path;
	for(uint x=1; x<argc; x++){
		if (is_directory(argv[x]))
			directory[i++]=(char*)argv[x];
		else if(is_file(argv[x]))
			files[i_f++]=(char*)argv[x];
	}
	for(uint x=0; x<i; x++){
		all_file=lista_directorio(directory[x], extensions);
		if (all_file==NULL)
			continue;
		uint y=0;
		while (all_file!=NULL){
			char* path=Pop(&all_file);
			out_file=get_file(path);
			len_path=strlen(path);
			repeat_chr(80, '=');
			printf("\n%*s%s\n", (len_path<80)?(80 - len_path) / 2:0, "", path);
			repeat_chr(80, '=');
			printf("\n%s", out_file);
			free(out_file);
			free(path);
		}
	}
	for (uint x=0; x<i_f; x++){
		out_file=get_file(files[x]);
		len_path=strlen(files[x]);
		repeat_chr(80, '=');
		printf("\n%*s%s\n", (len_path<80)?(80 - len_path) / 2:0, "", files[x]);
		repeat_chr(80, '=');
		printf("\n%s", out_file);
		free(out_file);
	}
	free(directory);
	free(files);
	return 0;
}

char* get_file(char* path){
	FILE* file=fopen(path, "r");
	long int size_file=0;
	char* out;
	//No existe.
	if (file==NULL){
		printf("Error: El archivo \"%s\" no existe, revise la ruta.\n", path);
		return NULL;
	}
	//Movemos al final del archivo para ver donde termina su contenido.
	fseek(file, 0L, SEEK_END);
		size_file=ftell(file);
	fseek(file, 0L, SEEK_SET);//Regreso al inicio.

	out=(char*)malloc(size_file+2);
	//Leemos todo de una.
	fread(out,sizeof(char),size_file,file);
	out[size_file]='\0'; //Seguridad.
	fclose(file);
	return out;
}
#define LISTAR_DIRECTORY(path, out, extens) \
	StackPath* sub_path=lista_directorio(path, extens); \
	while (sub_path!=NULL) \
		Push(&out, Pop(&sub_path)); \
	free(path);

//// @todo: hacer un strcmp que no le pare a mayuscula y a minusculas.
#define GET_LOG_FILE(path, out, ptr_ext) \
	int extension_found = 0; \
	if (ptr_ext != NULL){ \
		ptr_ext += 1; \
		for (size_t i = 0; extensiones[i]!=NULL; i++){ \
			if (strcmp_uncase_sensible(extensiones[i], ptr_ext) == 1){ \
				Push(&out, path); \
				extension_found = 1; \
				break; \
			} \
		} \
	} \
	if (!extension_found)\
		free(path); /* No nos interesa mostrar este archivo.*/
StackPath* lista_directorio(char *nombre, char** extensiones){
	StackPath* stack_out=NULL;
#ifdef _WIN32
	char search_path[MAX_PATH];
	snprintf(search_path, sizeof(search_path), "%s\\*", nombre);

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(search_path, &findFileData);

	if (hFind == INVALID_HANDLE_VALUE){
		fprintf(stderr, "No se puede abrir el archivo o directorio %s.\n", nombre);
		return NULL;
	}

	do{
		if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0)
			continue;

		size_t size_path = strlen(nombre)+strlen(findFileData.cFileName) + 2;
		char *path = (char *)malloc(sizeof(char)*(size_path + 1));
		sprintf(path, "%s\\%s", nombre, findFileData.cFileName);

		char* ptr=strchr(findFileData.cFileName,'.');
		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
			LISTAR_DIRECTORY(path, stack_out, extensiones);
		}else{
			GET_LOG_FILE(path, stack_out, ptr);
		}
	} while (FindNextFile(hFind, &findFileData) != 0);
	FindClose(hFind);
#else
	DIR *folder = opendir(nombre);
	if (folder == NULL){
		fprintf(stderr, "No se puede abrir el directorio %s.\n", nombre);
		return NULL;
	}
	struct dirent *directorio = readdir(folder);
	while (directorio!= NULL){
		if (
			strcmp(directorio->d_name, ".") == 0 ||
			strcmp(directorio->d_name, "..") == 0
		){
			directorio = readdir(folder);
			continue;
		}
		size_t size_path = strlen(nombre)+strlen(directorio->d_name) + 2;
		char *path = (char *)malloc(size_path);
		sprintf(path,"%s/%s", nombre, directorio->d_name);
		char* ptr=strchr(directorio->d_name,'.');
		if (directorio->d_type == DT_DIR){
			LISTAR_DIRECTORY(path, stack_out, extensiones);
		}else{
			GET_LOG_FILE(path, stack_out, ptr);
		}
		directorio = readdir(folder);
	}
	closedir(folder);
#endif
	return stack_out;
}
#undef LISTAR_DIRECTORY
#undef GET_LOG_FILE
