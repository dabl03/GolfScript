#define MAIN 1
#include "./view_log.c"

int main(int argc, char const *argv[]){
	char** directory=(char**)malloc(sizeof(char**)*(argc+3));
	char** files=(char**)malloc(sizeof(char**)*(argc+3));
	char* extend[]={
		"log",
		"o",
		"exe",
		(char*)NULL
	};
	StackPath* all_file;
	uint i=0;
	uint i_f=0;
	for(uint x=1; x<argc; x++){
		if (is_directory(argv[x]))
			directory[i++]=(char*)argv[x];
		else if(is_file(argv[x]))
			files[i_f++]=(char*)argv[x];
	}
	for(uint x=0; x<i; x++){
		all_file=lista_directorio(directory[x], extend);
		if (all_file==NULL)
			continue;
		uint y=0;
		while (all_file!=NULL){
			char* path=Pop(&all_file);
			remove(path);
			free(path);
		}
	}
	for (uint x=0; x<i_f; x++)
		remove(files[x]);
	free(directory);
	free(files);
	return 0;
}