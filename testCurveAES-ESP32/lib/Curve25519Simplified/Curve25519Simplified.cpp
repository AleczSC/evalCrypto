#include "Curve25519Simplified.h"

bool calibrarRuido(TransistorNoiseSource noise)
{
    bool calibrating = 0;
    bool newCalibrating;
    int count = 0;

    //Asegurarse de que esta calibrado antes de ejecutar el codigo
    while(!calibrating)
    {
        newCalibrating = noise.calibrating();
        if (newCalibrating != calibrating) 
        {
            calibrating = newCalibrating;
            if (calibrating)
                Serial.println("calibrating");
        }

    count++;
    if (count >= 15)
    {
        Serial.print("Error: No se pudo calibrar.\n");
        return 0;
    }
  }
  Serial.print("Calibracion exitosa.\n");
  return 1;
}

bool aliceFase1(uint8_t *a_k, uint8_t *a_f)
{
    Serial.print("Generando fase 1 de Alice: ");
    Curve25519::dh1(a_k, a_f);
    Serial.print("Fase 1 de Alice completa.\n");
    return 1;
}


bool aliceFase2(uint8_t *b_k, uint8_t *a_f)
{
    Serial.print("Generando fase 2 de Alice: ");
    if(Curve25519::dh2(b_k, a_f))
    {
        Serial.print("Generacion exitosa.\n");
        return 1;
    }
    else
    {
        Serial.print("Error: No se pudo generar la llave privada.\n");
        return 0;
    }
}

bool bobFase1(uint8_t *b_k, uint8_t *b_f)
{
    Serial.print("Generando fase 1 de Bob: ");
    Curve25519::dh1(b_k, b_f);
    Serial.print("Fase 1 de Bob completa.\n");
    return 1;
}

bool bobFase2(uint8_t *a_k, uint8_t *b_f)
{
    Serial.print("Generando fase 2 de Bob: ");
    if(Curve25519::dh2(a_k, b_f))
    {
        Serial.print("Generacion exitosa.\n");
        return 1;
    }
    else
    {
        Serial.print("Error: No se pudo generar la llave privada.\n");
        return 0;
    }
}

void perfFase1()
{
    unsigned long start, elapsed;
    int count;
    byte buffer_k[32], buffer_f[32];
    
    start = millis();
    for (count = 0; count < 30; count++)
        Curve25519::dh1(buffer_k, buffer_f);
    elapsed = millis() - start;

    Serial.print("Tiempo de ejecucion fase 1: ");
    Serial.print(elapsed / 30.0);
    Serial.println("ms por operacion.");
    Serial.print(30.0 * 1000.0 / elapsed);
    Serial.println(" operaciones por segundo.\n");
}

void perfFase2()
{
    unsigned long start, elapsed;
    int count;
    byte buffer_k[32], buffer_f[32];

    start = millis();
    for(count = 0; count < 30; count++)
        Curve25519::dh2(buffer_k, buffer_f);
    elapsed = millis() - start;

    Serial.print("Tiempo de ejecucion fase 2: ");
    Serial.print(elapsed / 30.0);
    Serial.println("ms por operacion");
    Serial.print(30.0 * 1000.0 / elapsed);
    Serial.println(" operaciones por segundo.\n");
}