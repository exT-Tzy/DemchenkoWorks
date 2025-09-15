#include <stdio.h>
#include <stdlib.h>

#define NUMBERS_TO_WRITE_IN_FILE_COUNT 11

enum
{
	OK = 0,
	NOT_ENOUGH_ARGUMENTS,
	FILE_CANNOT_BE_OPENED,
	FILE_WRITE_ERR,
    FILE_READ_ERR,
    FSEEK_ERR
};

void print_file_info(
    FILE *file, 
    char const *operation) 
{
    printf("After %s:\n", operation);
    printf("  _flags: %d\n", file->_flags);
    printf("  _IO_read_ptr: %p\n", (void*)file->_IO_read_ptr);
    printf("  _IO_read_end: %p\n", (void*)file->_IO_read_end);
    printf("  _IO_read_base: %p\n", (void*)file->_IO_read_base);
    printf("  _IO_write_ptr: %p\n", (void*)file->_IO_write_ptr);
    printf("  _IO_write_end: %p\n", (void*)file->_IO_write_end);
    printf("  _IO_write_base: %p\n", (void*)file->_IO_write_base);
    printf("  _IO_buf_base: %p\n", (void*)file->_IO_buf_base);
    printf("  _IO_buf_end: %p\n", (void*)file->_IO_buf_end);
    printf("  _IO_save_base: %p\n", (void*)file->_IO_save_base);
    printf("  _IO_backup_base: %p\n", (void*)file->_IO_backup_base);
    printf("  _IO_save_end: %p\n", (void*)file->_IO_save_end);
    printf("  _markers: %p\n", (void*)file->_markers);
    printf("  _chain: %p\n", (void*)file->_chain);
    printf("  _fileno: %d\n", file->_fileno);
    printf("  _flags2: %d\n", file->_flags2);
    printf("  _old_offset: %ld\n", file->_old_offset);
    printf("  _cur_column: %d\n", file->_cur_column);
    printf("  _vtable_offset: %d\n", file->_vtable_offset);
    printf("  _shortbuf: %s\n", file->_shortbuf);
    printf("  _lock: %p\n", (void*)file->_lock);
    printf("  _offset: %ld\n", file->_offset);
    printf("  _codecvt: %p\n", (void*)file->_codecvt);
    printf("  _wide_data: %p\n", (void*)file->_wide_data);
    printf("  _freeres_list: %p\n", (void*)file->_freeres_list);
    printf("  _freeres_buf: %p\n", (void*)file->_freeres_buf);
    printf("  __pad5: %ld\n", file->__pad5);
    printf("  _mode: %d\n", file->_mode);
    printf("  _unused2: %p\n", (void*)file->_unused2);
    printf("\n");
}

int main(
	int argc,
	char *argv[])
{
	FILE* fi;
	unsigned char const numbers_to_write_in_file[NUMBERS_TO_WRITE_IN_FILE_COUNT] = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
    unsigned char byte, buf[4];
    int iteration = 0, i;
    size_t bytes_read;

	if (argc != 2)
	{
		printf("Usage: %s <file_path>\n", argv[0]);

		return NOT_ENOUGH_ARGUMENTS;
	}

	fi = fopen(argv[1], "wb");
	if (!fi)
	{
		return FILE_CANNOT_BE_OPENED;
	}

	if (fwrite(numbers_to_write_in_file, sizeof(unsigned char), NUMBERS_TO_WRITE_IN_FILE_COUNT, fi) != NUMBERS_TO_WRITE_IN_FILE_COUNT)
	{
		fclose(fi);

		return FILE_WRITE_ERR;
	}

    fclose(fi);

    fopen(argv[1], "rb");
    if (!fi)
    {
        return FILE_CANNOT_BE_OPENED;
    }

    print_file_info(fi, "opening the file");

    while(fread(&byte, sizeof(unsigned char), 1, fi) == 1)
    {
        printf("Iteration: %d\nRead byte: %u\n", ++iteration, byte);
        print_file_info(fi, "%d iteration of reading the file\n");
    }

    fclose(fi);

    fopen(argv[1], "rb");
    if (!fi)
    {
        return FILE_CANNOT_BE_OPENED;
    }

    if(fseek(fi, 3, SEEK_SET) != 0)
    {
        fclose(fi);

        return FSEEK_ERR;
    }

    print_file_info(fi, "fseek");

	bytes_read = fread(buf, sizeof(unsigned char), 4, fi);
    if (bytes_read != 4)
    {
        fclose(fi);

        return FILE_READ_ERR;
    }

    print_file_info(fi, "fread");

    printf("Buffer contents: ");
    for (i = 0; i < bytes_read; ++i)
    {
        printf("%d ", buf[i]);
    }
    printf("\n");

    fclose(fi);

	return OK;
}