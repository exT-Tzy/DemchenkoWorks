#include <iostream>
#include <fstream>
#include <cstring>
#include <stdexcept>

#include "encoder.h"

inline void encoder::swap(
    unsigned char& a,
    unsigned char& b)
{
    unsigned char temp = a;
    a = b;
    b = temp;
}

void encoder::KSA(unsigned char* S)  // Key-Scheduling Algorithm
{
    if (!S) throw std::invalid_argument("S argument is nullptr!");

    int j = 0;
    
    for (int i = 0; i < 256; ++i)
    {
        j = (j + S[i] + _key[i % _key_size]) % 256;
       
        swap(S[i], S[j]);
    }
}

unsigned char encoder::PRGA(  // Pseudo-Random Generation Algorithm
    unsigned char* S, 
    int& i, int& j)
{
    if (!S) throw std::invalid_argument("S argument is nullptr!");

    i = (i + 1) % 256;
    j = (j + S[i]) % 256;
   
    swap(S[i], S[j]);
    
    return S[(S[i] + S[j]) % 256];
}

void encoder::mutator(
    unsigned char const* key,
    size_t size)
{
    if (!key) throw std::invalid_argument("Key argument is nullptr!");
    if (size == 0) throw std::invalid_argument("Key size cannot be zero!");

    unsigned char* new_key = new unsigned char[size];
   
    memcpy(new_key, key, size);

    delete[] _key;
    
    _key = new_key;
    _key_size = size;
}

void encoder::encode(
    char const* in_file_path, 
    char const* out_file_path, 
    encode_mode mode)
{
    std::ifstream in_file(in_file_path, std::ios::binary);
    if (!in_file) throw std::runtime_error("Failed to open input file!");

    std::ofstream out_file(out_file_path, std::ios::binary);
    if (!out_file) throw std::runtime_error("Failed to open output file!");

    unsigned char S[256];
    for (int i = 0; i < 256; ++i)
    {
        S[i] = i;
    }

    KSA(S);

    int i = 0, j = 0;
    unsigned char input_byte;
    
    while (in_file.read(reinterpret_cast<char*>(&input_byte), sizeof(char)))
    {
        unsigned char key_byte = PRGA(S, i, j);
        unsigned char output_byte = input_byte ^ key_byte;
        
        out_file.write(reinterpret_cast<char*>(&output_byte), sizeof(char));
    }

    in_file.close();
    out_file.close();
}

encoder::encoder(
    unsigned char const* key, 
    size_t size)
{
    if (size == 0) throw std::invalid_argument("Key size cannot be zero!");
    if (!key) throw std::invalid_argument("Key argument is nullptr!");

    _key_size = size;
    _key = new unsigned char[size];
    memcpy(_key, key, size);
}

encoder::encoder(encoder const& src)
{
    _key_size = src._key_size;
    _key = new unsigned char[_key_size];
    memcpy(_key, src._key, _key_size);
}

encoder& encoder::operator=(encoder const& src)
{
    if (this == &src) return *this;

    unsigned char* new_key = new unsigned char[src._key_size];
    memcpy(new_key, src._key, src._key_size);

    delete[] _key;
    
    _key = new_key;
    _key_size = src._key_size;

    return *this;
}

encoder::encoder() {};  // Using constructor without parameters is banned for users

encoder::~encoder() noexcept
{
    delete[] _key;
}