#ifndef MEMORY_H
#define MEMORY_H

typedef enum {
	_MEM = 0,
	_FILE = 1,
	_END = 2
} ptrtype;

void lozhka(   // Pathetic parody of vilka
	ptrtype flag,
	void* ptr,
	...);

int _realloc(
	void** ptr,
	size_t size);

#endif //MEMORY_H