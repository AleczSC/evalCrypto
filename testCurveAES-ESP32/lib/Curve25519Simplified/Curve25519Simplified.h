#ifndef CURVE25519SIMPLIFIED_H
#define CURVE25519SIMPLIFIED_H

#include <Arduino.h>
#include <Crypto.h>
#include <Curve25519.h>
#include <RNG.h>
#include <TransistorNoiseSource.h>
#include <string.h>

bool calibrarRuido(TransistorNoiseSource noise);
bool aliceFase1(uint8_t *a_k, uint8_t *a_f);
bool aliceFase2(uint8_t *b_k, uint8_t *a_f);
bool bobFase1(uint8_t *b_k, uint8_t *b_f);
bool bobFase2(uint8_t *a_k, uint8_t *b_f); 
void perfFase1();
void perfFase2();


#endif