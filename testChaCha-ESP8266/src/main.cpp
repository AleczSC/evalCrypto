#include <Arduino.h>
#include <Crypto.h>
#include <ChaCha.h>
#include <string.h>
#include <ChaCha20Simplified.h>

//Declarar valores en el vector
vectorChaCha20 vectorchacha20 = 
{
  .key        =   {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 
                  0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
                  0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
                  0x79, 0x7A, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46},
  .keySize    =   MAX_KEY_SIZE,
  .rounds     =   20,
  .plaintext  =   {0x42, 0x61, 0x69, 0x6C, 0x61, 0x20, 0x63, 0x68, 
                  0x61, 0x63, 0x68, 0x61, 0x63, 0x68, 0x61, 0x21},
  .ciphertext =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  .iv =           {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37},
  .counter    =   {0x39, 0x38, 0x37, 0x36, 0x35, 0x34, 0x33, 0x32},
  .size       =   16
};

ChaCha chacha;
uint8_t plaintext[MAX_PLAINTEXT_SIZE];
uint8_t ciphertext[MAX_CIPHERTEXT_SIZE];

void setup() 
{
  Serial.begin(115200);

  Serial.println("Prueba de protocolo ChaCha.\n");

  Serial.print("Texto original: ");
  for(size_t i = 0; i < MAX_PLAINTEXT_SIZE; i++)
    Serial.write(vectorchacha20.plaintext[i]);
  Serial.println();
  
  encryptCipherChaCha20(&chacha, &vectorchacha20);
  decryptCipherChaCha20(&chacha, &vectorchacha20);
  Serial.println();

  Serial.println("Pruebas de desempeño: ");
  perfSetValuesChaCha20(&chacha, &vectorchacha20);
  perfEncryptChaCha20(&chacha, &vectorchacha20);
  perfDecryptChaCha20(&chacha, &vectorchacha20);

}

void loop() {
  // put your main code here, to run repeatedly:
}
