#ifndef CHACHAPOLYSIMPLIFIED_H
#define CHACHAPOLYSIMPLIFIED_H

#include <Arduino.h>
#include <Crypto.h>
#include <ChaChaPoly.h>
#include <string.h>

#define MAX_PLAINTEXT_SIZE 16
#define MAX_CIPHERTEXT_SIZE 16
#define MAX_KEY_SIZE 32
#define MAX_TAG_SIZE 16

class vectorChaChaPoly
{
    public:
        uint8_t key[MAX_KEY_SIZE];
        size_t keySize;
        byte plaintext[MAX_PLAINTEXT_SIZE];
        byte ciphertext[MAX_CIPHERTEXT_SIZE];
        uint8_t authdata[16];
        uint8_t iv[16];
        uint8_t tag[16];
        size_t authsize;
        size_t datasize;
        size_t tagsize;
        size_t ivsize;
};

void setValuesChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector);     //Inicializa los valores para encriptar/descifrar
void encryptCipherChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector); //Encripta
void decryptCipherChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector); //Descifra 
void getTag(ChaChaPoly *cipher, vectorChaChaPoly *vector);                  //Obtiene tag

//Funciones de desempeño
void perfSetValuesChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector);
void perfEncryptChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector);
void perfDecryptChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector);
void perfTagChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector);



#endif