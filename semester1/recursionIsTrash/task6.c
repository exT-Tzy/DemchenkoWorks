#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

int recursive_integers_count_in_file(FILE* fi)
{
	int num;

	if (!fi) return 0;
	if (fscanf(fi, "%d", &num) == 1) return 1 + recursive_integers_count_in_file(fi);

	return 0;
}

int main()
{
	FILE* file;

	file = fopen("example.txt", "r");
	if (!file) return 666;

	printf("%d", recursive_integers_count_in_file(file));

	fclose(file);

	return 0;
}