#include <Arduino.h>
#include <String.h>
#include <ByteConversions.h>

#define MAX_PLAINTEXT_SIZE 16

byte bufferByte[MAX_PLAINTEXT_SIZE];
char bufferChar[MAX_PLAINTEXT_SIZE + 1];
char bufferCharHex[MAX_PLAINTEXT_SIZE * 2 + 1];

byte plaintext[MAX_PLAINTEXT_SIZE] = {0x4c, 0x61, 0x64, 0x69, 0x65, 0x73, 0x20, 0x61,
                                     0x6e, 0x64, 0x20, 0x47, 0x65, 0x6e, 0x74, 0x6c};

void printBytes(byte *input, size_t length);

void setup() 
{
  Serial.begin(115200);

  Serial.print("Texto original: ");
  printBytes(plaintext, MAX_PLAINTEXT_SIZE);

  //byte a char
  byteToCharArray(plaintext, bufferChar, MAX_PLAINTEXT_SIZE);
  Serial.print("Convertido a char: ");
  Serial.println(bufferChar);

  //byte a hexChar
  byteToHexChar(plaintext, bufferCharHex, MAX_PLAINTEXT_SIZE, MAX_PLAINTEXT_SIZE * 2);
  Serial.print("Convertido a hex char: ");
  Serial.println(bufferCharHex);

  //char a String
  String strByte = String(bufferChar);
  Serial.print("Convertido a String: ");
  Serial.println(strByte);
  
  //hexChar a hexString
  String strHex = String(bufferCharHex);
  Serial.print("String Hex: ");
  Serial.println(strHex);

  //char a byte
  charArrayToByte(bufferChar, bufferByte, MAX_PLAINTEXT_SIZE);
  Serial.print("Char convertido a byte: ");
  printBytes(bufferByte, MAX_PLAINTEXT_SIZE);

  //hexChar a byte
  hexCharToByte(bufferCharHex, bufferByte, MAX_PLAINTEXT_SIZE * 2, MAX_PLAINTEXT_SIZE);
  Serial.print("Hex char convertido a byte: ");
  printBytes(bufferByte, MAX_PLAINTEXT_SIZE);

  //String a byte
  strToByte(strByte, bufferByte, sizeof(strByte), MAX_PLAINTEXT_SIZE);
  Serial.print("String convertido a byte: ");
  printBytes(bufferByte, MAX_PLAINTEXT_SIZE);

  //hex String a byte
  hexStrToByte(strHex, bufferByte, MAX_PLAINTEXT_SIZE * 2, MAX_PLAINTEXT_SIZE);
  Serial.print("String hex convertido a byte: ");
  printBytes(bufferByte, MAX_PLAINTEXT_SIZE);

}

void loop() {
  // put your main code here, to run repeatedly:
}

void printBytes(byte *input, size_t length)
{
  for (int i = 0; i < length; i++)
    Serial.write(input[i]);
  Serial.println();
}