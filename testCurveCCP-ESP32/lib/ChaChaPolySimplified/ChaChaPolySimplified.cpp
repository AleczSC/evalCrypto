#include "ChaChaPolySimplified.h"

void setValuesChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  size_t posn, len;

  cipher -> clear();      //Limpiar valores
  //Comienza configuracion, debe ejecutarse en este orden
  cipher -> setKey(vector -> key, 32);
  cipher -> setIV(vector -> iv, vector -> ivsize);

  //Generacion de autenticacion
  Serial.print("Authentication data: ");
  for (posn = 0; posn < vector -> authsize; posn++) 
  {
    len = vector->authsize - posn;
    if (len > 1)
      len = 1;
    cipher->addAuthData(vector -> authdata + posn, len);
    Serial.write(vector -> authdata[posn]);
  }
  Serial.print("\n\n");
}

void encryptCipherChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  byte output[MAX_PLAINTEXT_SIZE];
  size_t posn, len;

  //Inicializar valores para encriptacion
  setValuesChaChaPoly(cipher, vector);

  //Comienza encriptacion
  Serial.print("Texto encriptado: ");
  for (posn = 0; posn < vector -> datasize; posn++) 
  {
    len = vector->datasize - posn;
    if (len > 1)
        len = 1;
    cipher->encrypt(output + posn, vector->plaintext + posn, len);//
    vector -> ciphertext[posn] = output[posn];                    //Guardar palabra encriptada en la estructura
    Serial.write(output[posn]);
  }
  Serial.println();

  getTag(cipher, vector);

}

void decryptCipherChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  byte output[MAX_PLAINTEXT_SIZE];
  size_t posn, len;

  //Inicializar valores para descifrar
  setValuesChaChaPoly(cipher, vector);

  //Proceso de descifrado 
  Serial.print("texto desencriptado: \n");
  for (posn = 0; posn < vector -> datasize; posn++) 
  {
    len = vector -> datasize - posn;
    if (len > 1)
      len = 1;
    cipher->decrypt(output + posn, vector -> ciphertext + posn, len);
    vector -> plaintext[posn] = output[posn];
    Serial.write(output[posn]);
  }
  Serial.println();

  //Comprobar tag
  if (cipher -> checkTag(vector -> tag, sizeof(vector -> tag)))
    Serial.print("Tag comprobada exitosamente. \n\n");
  else
    Serial.print("Error: la Tag no coincide. \n\n");   

    cipher -> clear();
}

void getTag(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  //Generacion de tag
  cipher -> computeTag(vector -> tag, vector -> tagsize);
  //Imprimir tag en la consola
  Serial.print("Tag generada: ");
  for (int i = 0; i < sizeof(vector -> tagsize); i++)
    Serial.write(vector -> tag[i]);
  Serial.println();
}

void perfSetValuesChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  unsigned long start, elapsed;
  int count;
  byte buffer[MAX_PLAINTEXT_SIZE];

  start = micros();
  for (count = 0; count < 1000; count++)
  {
    cipher -> setKey(vector -> key, MAX_KEY_SIZE);
    cipher -> setIV(vector -> iv, vector -> ivsize);
  }
  elapsed = micros() - start;

  Serial.print("Tiempo de ejecucion protocolo setKey: ");
  Serial.print (elapsed/1000.0);
  Serial.println("us por operacion.");
  Serial.print(1000.0 * 1000000.0 / elapsed);
  Serial.println("operaciones por segundo.\n");

  cipher -> setKey(vector -> key, vector -> keySize);
  cipher -> setIV(vector -> iv, vector -> ivsize);
  start = micros();
  for (count = 0; count < 500; count++)
    cipher -> addAuthData(buffer, MAX_PLAINTEXT_SIZE);
  elapsed = micros() - start;

  Serial.print("Tiempo de ejecucion protocolo AddAuthData: ");
  Serial.print(elapsed / (MAX_PLAINTEXT_SIZE * 500.0));
  Serial.println("us por byte");
  Serial.print((MAX_PLAINTEXT_SIZE * 500.0 * 1000000) / elapsed);
  Serial.println("bytes por segundo.");

  cipher -> clear();
}

void perfEncryptChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  unsigned long start, elapsed;
  int count;
  byte buffer[MAX_PLAINTEXT_SIZE];

  cipher -> setKey(vector -> key, vector -> keySize);
  cipher -> setIV(vector -> iv, vector ->ivsize);
  start = micros();
  for (count = 0; count < 500; count++)
    cipher -> encrypt(buffer, buffer, sizeof(buffer));

  elapsed = micros() - start;

  Serial.print("Tiempo de ejecucion encriptacion: ");
  Serial.print(elapsed / (sizeof(buffer) * 500.0));
  Serial.println("us por byte.");
  Serial.print((sizeof(buffer) * 500.0 * 1000000.0) / elapsed);
  Serial.println("bytes por segundo.");

  cipher -> clear();
}

void perfDecryptChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  unsigned long start, elapsed;
  int count;
  byte buffer[MAX_PLAINTEXT_SIZE];

  cipher -> setKey(vector -> key, vector -> keySize);
  cipher -> setIV(vector -> iv, vector ->ivsize);
  start = micros();
  for (count = 0; count < 500; count++)
    cipher -> decrypt(buffer, buffer, sizeof(buffer));

  elapsed = micros() - start;

  Serial.print("Tiempo de ejecucion desencriptado: ");
  Serial.print(elapsed / (sizeof(buffer) * 500.0));
  Serial.println("us por byte.");
  Serial.print((sizeof(buffer) * 500.0 * 1000000.0) / elapsed);
  Serial.println("bytes por segundo.");

  cipher -> clear();
}
void perfTagChaChaPoly(ChaChaPoly *cipher, vectorChaChaPoly *vector)
{
  unsigned long start, elapsed;
  int count;
  byte buffer[16];

  cipher -> setKey(vector -> key, vector -> keySize);
  cipher -> setIV(vector -> iv, vector -> ivsize);

  start = micros();
  for (count = 0; count < 1000; count++)
    cipher -> computeTag(buffer, 16);
  
  elapsed = micros() - start;
  
  Serial.print("Tiempo de ejecucion protocolo computeTag: ");
  Serial.print(elapsed / 1000.0);
  Serial.println("us por operacion.");
  Serial.print((1000.0 * 1000000.0) / elapsed);
  Serial.println("operaciones por segundo.");
}