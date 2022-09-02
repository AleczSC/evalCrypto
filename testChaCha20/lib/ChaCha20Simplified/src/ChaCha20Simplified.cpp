#include "ChaCha20Simplified.h"

void setValuesChaCha20(ChaCha *cipher, vectorChaCha20 *vector)
{
  //Protocolos a seguir de ChaCha, deben ser en este orden
  cipher -> setNumRounds(vector -> rounds);
  cipher -> setKey(vector -> key, vector -> keySize);
  cipher -> setIV(vector -> iv, cipher -> ivSize());
  cipher -> setCounter(vector -> counter, 8);
}

void encryptCipherChaCha20(ChaCha *cipher, vectorChaCha20 *vector)
{
  byte output[MAX_CIPHERTEXT_SIZE];
  size_t posn, len;

    setValuesChaCha20(cipher, vector);
    Serial.print("texto encriptado: \n");

    //Proceso de encriptacion
    for (posn = 0; posn < vector->size; posn++) 
    {
        len = vector->size - posn;
        if (len > 1)
            len = 1;
        cipher->encrypt(output + posn, vector->plaintext + posn, len);
        vector -> ciphertext[posn] = output[posn];
        Serial.write(output[posn]);        //Escribe cifrado en la consola
    }
    Serial.println();
    cipher -> clear();
}

void decryptCipherChaCha20(ChaCha *cipher, vectorChaCha20 *vector)
{
  byte output[MAX_CIPHERTEXT_SIZE];
  size_t posn, len;

  setValuesChaCha20(cipher, vector);
  Serial.print("texto desencriptado: \n");
    for (posn = 0; posn < vector -> size; posn++) 
    {
        len = vector->size - posn;
        if (len > 1)
            len = 1;
        cipher->decrypt(output + posn, vector -> ciphertext + posn, len);
        vector -> plaintext[posn] = output[posn];
        Serial.write(output[posn]);
    }
    Serial.println();

  cipher -> clear();
}

void perfSetValuesChaCha20(ChaCha *cipher, vectorChaCha20 *vector)
{
  unsigned long start;
  unsigned long elapsed;
  int count;
  
  cipher -> setNumRounds(vector -> rounds);
  start = micros();

  for (count = 0; count < 1000; count++)
  {
    cipher -> setKey(vector -> key, vector -> keySize);
    cipher -> setIV(vector -> iv, 8);
  }
  elapsed = micros() - start;

  Serial.print("tiempo de ejecucion protocolo SetKey: ");
  Serial.print(elapsed / 1000.0);
  Serial.println("us por operacion. ");
  Serial.println(1000.0 * 1000000.0 / elapsed);
  Serial.println("operaciones por segundo.");
  Serial.println();

  cipher -> clear();

}

void perfEncryptChaCha20(ChaCha *cipher, vectorChaCha20 *vector)
{
  unsigned long start;
  unsigned long elapsed;
  int count;

  uint8_t buffer[16];


  setValuesChaCha20(cipher, vector);
  start = micros();
  for (count = 0; count < 500; count++)
    cipher -> encrypt(buffer, buffer, sizeof(buffer));

  elapsed = micros() - start;

  Serial.print("tiempo de ejecucion encriptacion: ");
  Serial.print(elapsed/(sizeof(buffer) * 500.0));
  Serial.print("us por byte. ");
  Serial.print((sizeof(buffer) * 500.0 * 1000000.0) / elapsed);
  Serial.print(" bytes por segundo.");
  Serial.println();
}

void perfDecryptChaCha20(ChaCha *cipher, vectorChaCha20 *vector)
{
  unsigned long start;
  unsigned long elapsed;
  int count;

  uint8_t buffer[16];

  setValuesChaCha20(cipher, vector);
  start = micros();
  for (count = 0; count < 500; count++)
    cipher -> encrypt(buffer, buffer, sizeof(buffer));

  elapsed = micros() - start;

  Serial.print("tiempo de ejecucion descifrado: ");
  Serial.print(elapsed/(sizeof(buffer) * 500.0));
  Serial.print("us por byte. ");
  Serial.print((sizeof(buffer) * 500.0 * 1000000.0) / elapsed);
  Serial.print(" bytes por segundo.");
  Serial.println();

  cipher -> clear();
}