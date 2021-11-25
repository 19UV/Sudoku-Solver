#include "file.h"

const char* read_file(const char* path) {
	FILE* file;

#ifdef _WIN32
	errno_t err = fopen_s(&file, path, "r");
	if(err != 0) {
		return NULL;
	}
#else
	file = fopen(path, "r");
	if(file == NULL) {
		return NULL;
	}
#endif

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	
	char* source = malloc(file_size + 1);
	if(source == NULL) {
		fclose(file);
		
		return NULL;
	}
	
	char* write = source, ch;
	while((ch = fgetc(file)) != EOF) {
		*(write++) = ch;
	}
	*write = 0;
	
	fclose(file);
	
	return (const char*)source;
}