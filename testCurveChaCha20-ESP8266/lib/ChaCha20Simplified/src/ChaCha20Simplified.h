#ifndef CHACHA20SIMPLIFIED_H
#define CHACHA20SIMPLIFIED_H

#include <Arduino.h>
#include <Crypto.h>
#include <ChaCha.h>
#include <string.h>


#define MAX_PLAINTEXT_SIZE 16
#define MAX_CIPHERTEXT_SIZE 16
#define MAX_KEY_SIZE 32

class vectorChaCha20
{
    public:
        uint8_t key[MAX_KEY_SIZE];
        size_t keySize;
        uint8_t rounds;
        byte plaintext[MAX_PLAINTEXT_SIZE];
        byte ciphertext[MAX_CIPHERTEXT_SIZE];
        byte iv[8];
        byte counter[8];
        size_t size;
};

void setValuesChaCha20(ChaCha *cipher, vectorChaCha20 *vector);
void encryptCipherChaCha20(ChaCha *cipher, vectorChaCha20 *vector);
void decryptCipherChaCha20(ChaCha *cipher, vectorChaCha20 *vector);
void perfSetValuesChaCha20(ChaCha *cipher, vectorChaCha20 *vector);
void perfEncryptChaCha20(ChaCha *cipher, vectorChaCha20 *vector);
void perfDecryptChaCha20(ChaCha *cipher, vectorChaCha20 *vector);


#endif