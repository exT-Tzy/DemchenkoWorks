#ifndef ENCODER_H
#define ENCODER_H

enum encode_mode
{
    encrypt,
    decrypt
};

class encoder final
{
private:
    
    unsigned char* _key;
    
    size_t _key_size;

    inline void swap(unsigned char& a, unsigned char& b);

    void KSA(unsigned char* S); // Key-Scheduling Algorithm

    unsigned char PRGA(unsigned char* S, int& i, int& j); // Pseudo-Random Generation Algorithm

    encoder(); // Using constructor without parameters is banned for users

public:
   
    void mutator(unsigned char const* key, size_t size);

    void encode(char const* in_file_path, char const* out_file_path, encode_mode mode = encrypt);

    encoder(unsigned char const* key, size_t size);

    encoder(encoder const& src);

    encoder& operator=(encoder const& src);

    ~encoder() noexcept;
};

#endif // ENCODER_H