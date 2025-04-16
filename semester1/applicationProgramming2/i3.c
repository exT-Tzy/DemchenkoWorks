#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef enum {
	DEALLOCATE_MEMORY = 0,
	CLOSE_FILE = 1
} Flagtype;

void lozhka(   // Pathetic parody of vilka ¯\_(ツ)_/¯
	void* resource, 
	...) 
{
	va_list arg;
	va_start(arg, resource);

	size_t count_of_pointers_on_dynamic_memory = 0, count_of_files = 0;

	while (resource != NULL)
	{
		Flagtype flag = va_arg(arg, Flagtype);

		if (flag == DEALLOCATE_MEMORY)
		{
			free(resource);
			
			++count_of_pointers_on_dynamic_memory;
		}
		else if (flag == CLOSE_FILE)
		{
			fclose((FILE*)resource);
			
			++count_of_files;
		}
		
		resource = va_arg(arg, void*);
	}

	va_end(arg);

	printf("%zu part(s) of memory deallocated.\n%zu file(s) closed.\n", count_of_pointers_on_dynamic_memory, count_of_files);
}

int main()
{
	char* dynamic_memory_example = (char*)malloc(10 * sizeof(char));

	if (!dynamic_memory_example)
	{
		printf("Memory allocation error!\n");

		return EXIT_FAILURE;
	}

	FILE* file_example = fopen("fileExample.txt", "w");

	if (!file_example)
	{
		printf("Open file error!\n");

		return EXIT_FAILURE;
	}

	lozhka(dynamic_memory_example, DEALLOCATE_MEMORY, file_example, CLOSE_FILE, NULL);

	return EXIT_SUCCESS;
}