#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "encoder.h"

int main()
{
	try
	{
		unsigned char key[] = "hdfhlijdsflhi";

		encoder enc(key, sizeof(key) / sizeof(char));

		enc.encode("example.txt", "encrypted.bin");

		enc.encode("encrypted.bin", "decrypted.txt");
	}
	catch(std::bad_alloc)
	{
		std::cerr << "Memory allocation fault!" << std::endl;
	}
	catch(std::runtime_error const& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (std::invalid_argument const& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}