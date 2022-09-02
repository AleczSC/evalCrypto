#include "AesSimplified.h"

void encryptBlockAES256(AES256 *block, vectorAES *vector)
{
    block -> setKey(vector -> key, 32);
    block -> encryptBlock(vector -> ciphertext, vector -> plaintext);

    Serial.print("Texto encriptado: ");
    for(size_t i = 0; i < MAX_PLAINTEXT_SIZE; i++)
        Serial.write(vector -> ciphertext[i]);
    Serial.println();

}

void decryptBlockAES256(AES256 *block, vectorAES *vector)
{
    block -> setKey(vector -> key, 32);
    block -> decryptBlock(vector -> plaintext, vector -> ciphertext);

    Serial.print("Texto desencriptado: ");
    for(size_t i = 0; i < MAX_CIPHERTEXT_SIZE; i++)
        Serial.write(vector -> plaintext[i]);
    Serial.println();
}

void perfEncryptBlockAES256(AES256 *block, vectorAES *vector)
{
    unsigned long start, elapsed;
    size_t count;
    uint8_t buffer[MAX_PLAINTEXT_SIZE];

    block -> setKey(vector -> key, 32);
    start = micros();
    for(count = 0; count < 500; count++)
        block -> encryptBlock(buffer, buffer);
    elapsed = micros() - start;

    Serial.print("tiempo de ejecucion encriptacion: ");
    Serial.print(elapsed/(sizeof(buffer) * 500.0));
    Serial.print("us por byte. ");
    Serial.print((sizeof(buffer) * 500.0 * 1000000.0) / elapsed);
    Serial.print(" bytes por segundo.");
    Serial.println();    
}

void perfDecryptBlockAES256(AES256 *block, vectorAES *vector)
{
    unsigned long start, elapsed;
    size_t count;
    uint8_t buffer[MAX_PLAINTEXT_SIZE];

    block -> setKey(vector -> key, 32);
    start = micros();
    for(count = 0; count < 500; count++)
        block -> decryptBlock(buffer, buffer);
    elapsed = micros() - start;

    Serial.print("tiempo de ejecucion descifrado: ");
    Serial.print(elapsed/(sizeof(buffer) * 500.0));
    Serial.print("us por byte. ");
    Serial.print((sizeof(buffer) * 500.0 * 1000000.0) / elapsed);
    Serial.print(" bytes por segundo.");
    Serial.println(); 
}