#define MAIN 1
#include "./view_log.c"

int main(int argc, char const *argv[]){
	char** directory=(char**)malloc(sizeof(char**)*(argc+3));
	char** files=(char**)malloc(sizeof(char**)*(argc+3));
	struct stat s;
	StackPath* all_file;
	char* out_file;
	uint i=0;
	uint i_f=0;
	uint len_path;
	for(uint x=1; x<argc; x++){
		if (stat(argv[x], &s)==0){
			if (s.st_mode&S_IFDIR){
				directory[i++]=(char*)argv[x];
			}else if(s.st_mode&S_IFREG){
				files[i_f++]=(char*)argv[x];
			}
		}
	}
	for(uint x=0; x<i; x++){
		all_file=lista_directorio(directory[x]);
		if (all_file==NULL)
			continue;
		uint y=0;
		while (all_file!=NULL){
			char* path=Pop(&all_file);
			out_file=get_file(path);
			len_path=strlen(path);
			if (strlen(out_file)<5){
			// No creo que un log de gcc
			//tenga menos de 5 caracteres.
				remove(path);
			}
			free(out_file);
			free(path);
		}
	}
	for (uint x=0; x<i_f; x++){
		out_file=get_file(files[x]);
		len_path=strlen(files[x]);
		if (strlen(out_file)<5)
			remove(files[x]);
		free(out_file);
	}
	free(directory);
	free(files);
	return 0;
}