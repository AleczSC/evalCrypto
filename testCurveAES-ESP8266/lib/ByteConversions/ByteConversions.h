#ifndef BYTECONVERSIONS_H
#define BYTECONVERSIONS_H

#include <Arduino.h>
#include <String.h>

void byteToCharArray(byte *input, char *output, size_t length);
void byteToHexChar(byte *input, char *output, size_t lenIn, size_t lenOut);

void charArrayToByte(char *input, byte *output, size_t length);
void hexCharToByte(char *input, byte*output, size_t lenIn, size_t lenOut);
void strToByte(String input, byte *output, size_t lenIn, size_t lenOut);
void hexStrToByte(String input, byte *output, size_t lenIn, size_t lenOut);


#endif