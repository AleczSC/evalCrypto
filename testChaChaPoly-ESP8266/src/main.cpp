#include <Arduino.h>
#include <ChaChaPolySimplified.h>

vectorChaChaPoly vectorchachapoly =
{
  .key          = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 
                  0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
                  0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
                  0x79, 0x7A, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46},
  .keySize      = MAX_KEY_SIZE,
  .plaintext    = {0x50, 0x6F, 0x6C, 0x69, 0x65, 0x74, 0x69, 0x6C, 
                  0x65, 0x6E, 0x6F, 0x20, 0x61, 0x7A, 0x75, 0x6C},
  .ciphertext   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  .authdata     = {0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
                  0x49, 0x4A, 0x4B, 0x4C},
  .iv           = {0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54,
                  0x55, 0x56, 0x57, 0x58},
  .tag          = {},
  .authsize     = 12,
  .datasize     = 16,
  .tagsize      = 16,
  .ivsize       = 12
};

ChaChaPoly chachapoly;

void setup() 
{
  Serial.begin(115200);
  Serial.print("\n\n");

  Serial.println("Prueba de protocolo ChaChaPoly.\n");
  //setValuesChaChaPoly(&chachapoly, &vectorchachapoly);
  encryptCipherChaChaPoly(&chachapoly, &vectorchachapoly);
  //setValuesChaChaPoly(&chachapoly, &vectorchachapoly);
  decryptCipherChaChaPoly(&chachapoly, &vectorchachapoly);
  Serial.println();
  
  //Pruebas de desempeño
  perfSetValuesChaChaPoly(&chachapoly, &vectorchachapoly);
  perfEncryptChaChaPoly(&chachapoly, &vectorchachapoly);
  perfDecryptChaChaPoly(&chachapoly, &vectorchachapoly);
  perfTagChaChaPoly(&chachapoly, &vectorchachapoly);

}

void loop() {
  // put your main code here, to run repeatedly:
}