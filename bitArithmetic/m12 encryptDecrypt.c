#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable: 4996)

void encryptDecrypt(const char* ipFile, const char* opFile, char key) {
    int byte;
    
    FILE* inFile = fopen(ipFile, "rb");
    FILE* outFile = fopen(opFile, "wb");

    if (inFile == NULL || outFile == NULL) {
        printf("\nFile open fault!");
        exit(1);
    }

    while ((byte = fgetc(inFile)) != EOF) {
        fputc(byte ^ key, outFile); 
    }

    fclose(inFile);
    fclose(outFile);
}

int main() {
    char inputFile[100], outputFile[100], key;

    printf("Enter input file name: ");
    scanf("%s", inputFile);
    printf("\nEnter output file name: ");
    scanf("%s", outputFile);
    printf("\nEnter char key: ");
    scanf(" %c", &key); 

    encryptDecrypt(inputFile, outputFile, key);

    printf("\nFile succesfully encrypted/decrypted.\n");

    return 0;
}
