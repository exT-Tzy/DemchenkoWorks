#include "memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void lozhka(   // Pathetic parody of vilka
	ptrtype flag,
	void *ptr,
	...)
{
	va_list args;
	ptrtype temp;

	if (flag == _MEM)
	{
		free(ptr);
	}
	else if (flag == _FILE)
	{
		fclose((FILE*)ptr);
	}

	va_start(args, ptr);

	while (1)
	{
		if ((temp = va_arg(args, ptrtype)) == _MEM)
		{
			free(va_arg(args, void*));
		}
		else if (temp == _FILE)
		{
			fclose(va_arg(args, FILE*));
		}
		else if (temp == _END)
		{
			break;
		}
	}

	va_end(args);
}

int _realloc(
	void** ptr,
	size_t size)
{
	void* for_realloc = NULL;
	
	for_realloc = realloc(*ptr, size);

	if (!for_realloc) return EXIT_FAILURE;

	*ptr = for_realloc;

	return EXIT_SUCCESS;
}