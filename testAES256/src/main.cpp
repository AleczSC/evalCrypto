#include <Arduino.h>
#include <AesSimplified.h>

vectorAES vectorAes256 = 
{
  . key         = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 
                  0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
                  0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
                  0x79, 0x7A, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46},
  .plaintext    = {0x54, 0x65, 0x78, 0x74, 0x6F, 0x20, 0x64, 0x65, 
                  0x20, 0x70, 0x72, 0x75, 0x65, 0x62, 0x61, 0x2E},
  .ciphertext   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

AES256 aes256;

void setup() 
{
  Serial.begin(115200);
  Serial.print("\n\n");


  Serial.println("Prueba de protocolo AES.\n");
  Serial.print("Texto original: ");
  for(int i = 0; i < MAX_PLAINTEXT_SIZE; i++)
    Serial.write(vectorAes256.plaintext[i]);
  Serial.println();

  encryptBlockAES256(&aes256, &vectorAes256);
  decryptBlockAES256(&aes256, &vectorAes256);
  Serial.println();
  
  //Pruebas de desempeño
  perfEncryptBlockAES256(&aes256, &vectorAes256);
  perfDecryptBlockAES256(&aes256, &vectorAes256);

}

void loop() {
  // put your main code here, to run repeatedly:
}