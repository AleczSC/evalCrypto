#ifndef AESSIMPLIFIED_H
#define AESSIMPLIFIED_H

#include <Arduino.h>
#include <Crypto.h>
#include <AES.h>
#include <string.h>

#define MAX_PLAINTEXT_SIZE 16
#define MAX_CIPHERTEXT_SIZE 16
#define MAX_KEYS_IZE 32

class vectorAES
{
    public:
        uint8_t key[32];
        uint8_t plaintext[16];
        uint8_t ciphertext[16];
};

void encryptBlockAES256(AES256 *block, vectorAES *vector);
void decryptBlockAES256(AES256 *block, vectorAES *vector);
void perfEncryptBlockAES256(AES256 *block, vectorAES *vector);
void perfDecryptBlockAES256(AES256 *block, vectorAES *vector);

#endif