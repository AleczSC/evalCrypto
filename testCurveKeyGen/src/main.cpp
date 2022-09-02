#include <Arduino.h>
#include <Crypto.h>
#include <Curve25519.h>
#include <RNG.h>
#include <TransistorNoiseSource.h>
#include <string.h>
#include <Curve25519Simplified.h>
#include <ByteConversions.h>

//Variables para el randomizador
#define RNG_TAG "Aleatoriedad"
TransistorNoiseSource noise(36);  //Seleccionar pin para entrada de ruido

//Variables Alice y Bob
uint8_t alice_k[32], alice_f[32];
bool alice_publicKeyState, alice_privateKeyState;
uint8_t bob_k[32], bob_f[32];
bool bob_publicKeyState, bob_privateKeyState;

char bufferHex[65];

void setup() 
{
  bool calibracion = 0;
  Serial.begin(115200);

  //Iniciar randomizador
  RNG.begin(RNG_TAG);
  RNG.addNoiseSource(noise);
  while(!calibracion)
    calibracion = calibrarRuido(noise);

  //Generacion de llave publica
  alice_publicKeyState = aliceFase1(alice_k, alice_f);
  bob_publicKeyState = bobFase1(bob_k, bob_f);

  byteToHexChar(alice_k, bufferHex, 32, 64);
  Serial.print("Llave publica Alice: ");
  Serial.println(bufferHex);
  byteToHexChar(bob_k, bufferHex, 32, 64);
  Serial.print("Llave publica Bob: ");
  Serial.println(bufferHex);

  //Generacion llave privada
  alice_privateKeyState = aliceFase2(bob_k, alice_f);
  bob_privateKeyState = bobFase2(alice_k, bob_f);

  byteToHexChar(bob_k, bufferHex, 32, 64);
  Serial.print("Llave privada Alice: ");
  Serial.println(bufferHex);
  byteToHexChar(alice_k, bufferHex, 32, 64);
  Serial.print("Llave privada Bob: ");
  Serial.println(bufferHex);

  //Comparar llaves privadas
    if(memcmp(alice_k, bob_k, 32) == 0)
      Serial.println("Los secretos coinciden.");
    else
      Serial.println("Los secretos no coinciden.");
    Serial.println();

  //Pruebas de desempeño
  perfFase1();
  perfFase2();

}

void loop() {
  // put your main code here, to run repeatedly:
}