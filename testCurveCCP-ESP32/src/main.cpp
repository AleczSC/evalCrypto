#include <Arduino.h>
#include <Curve25519Simplified.h>
#include <ChaChaPolySimplified.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ByteConversions.h>

//Credenciales de Router
const char* ssid = "UbeeD963";
const char* password = "5216F9D963";

//Nombre cliente y topics a utilizar
const char* clientName = "ESP32testCurveChaChaPolyMQTT_0";
const char* topicSendName = "ESP32Crypto/CurveChaChaPoly/UserName";
const char* topicWill = "ESP32Crypto/CurveChaChaPoly/dc";
char topicAlice[128];
char topicBob[128];
const char* topicAlicePlaintext = "ESP32testCurveChaChaPolyMQTT_0/plaintext";
const char* topicAliceCiphertext = "ESP32testCurveChaChaPolyMQTT_0/ciphertext";
const char* topicAliceTag = "ESP32testCurveChaChaPolyMQTT_0/tag";
char topicBobPlaintext[128];
char topicBobCiphertext[128];
char topicBobTag[128];

// direccion server mqtt
const char* mqtt_server = "broker.hivemq.com";

//Inicializar EspClient
WiFiClient ESP32_testCurveChaChaPoly;
PubSubClient client(ESP32_testCurveChaChaPoly);

//Declaracion de valores para encriptar
vectorChaChaPoly vectorchachapoly =
{
  .key          = {},
  .keySize      = MAX_KEY_SIZE,
  .plaintext    = {},
  .ciphertext   = {},
  .authdata     = {},
  .iv           = {},
  .tag          = {},
  .authsize     = 12,
  .datasize     = 16,
  .tagsize      = 16,
  .ivsize       = 12
};
ChaChaPoly chachapoly;

//Declaracion de funciones
void setup_wifi();          //Conecta tarjeta ESP32 al router wifi
void callback(String topic, byte* message, unsigned int length);  //Detecta cuando se recibe un mensaje en un topic suscrito
void reconnect();           //reconecta la tarjeta ESP32 al broker MQTT

//Variables a utilizar
String bufferString = "";
char bufferHex[MAX_KEY_SIZE*2 + 1];
char bufferChar[MAX_KEY_SIZE + 1];
unsigned long start, elapsed;

#define RNG_TAG "Aleatoriedad"    //Randomizador
TransistorNoiseSource noise(36);  //Seleccionar pin para entrada de ruido

//Variables Alice y Bob
uint8_t alice_k[32], alice_f[32];
bool alice_publicKeyState, alice_privateKeyState;
uint8_t bob_k[32], bob_f[32];
bool bob_publicKeyState, bob_privateKeyState;
char bobUser[64];


void setup() 
{
  bool calibracion = 0;
  Serial.begin(115200);

  //Iniciar randomizador
  RNG.begin(RNG_TAG);
  RNG.addNoiseSource(noise);
  while(!calibracion)
    calibracion = calibrarRuido(noise);

  alice_publicKeyState = aliceFase1(alice_k, alice_f);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  alice_publicKeyState = 0;
  alice_privateKeyState = 0;
  start = millis();

  strcpy(topicAlice, "ESP32Crypto/");
  strcat(topicAlice, clientName);
}

void loop() 
{

  if(!client.connected())
    reconnect();

  if(!client.loop())
    client.connect(clientName, "", "", topicWill, 2, false, clientName);

  //Enviar solicitud para intercambiar llaves cada 30 segundos
  if(((elapsed - start) > 10000) && (alice_privateKeyState == 0))
  {
    start = elapsed;
    Serial.println("Solicitando intercambio de llaves.");
    client.publish(topicSendName, clientName);
  }

  //Leer monitor y encriptar texto para enviarlo
  if(alice_privateKeyState && (Serial.available() > 0))
  {
    bufferString = Serial.readString();
    strToByte(bufferString, vectorchachapoly.plaintext, sizeof(bufferString), MAX_PLAINTEXT_SIZE);
    byteToCharArray(vectorchachapoly.plaintext, bufferChar, MAX_PLAINTEXT_SIZE);
    encryptCipherChaChaPoly(&chachapoly, &vectorchachapoly);

    //Se envia primero la tag
    byteToHexChar(vectorchachapoly.tag, bufferHex, vectorchachapoly.tagsize, vectorchachapoly.tagsize*2);
    client.publish(topicAliceTag, bufferHex);

    //Se envia el texto encriptado
    byteToHexChar(vectorchachapoly.ciphertext, bufferHex, MAX_CIPHERTEXT_SIZE, MAX_CIPHERTEXT_SIZE*2);
    //client.publish(topicAlicePlaintext, bufferChar);
    client.publish(topicAliceCiphertext, bufferHex);
  }

 elapsed = millis();
}

void setup_wifi()     //NO MODIFICAR ESTA FUNCION
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(String topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (size_t i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //Llega solicitud de intercambio
  if(!memcmp(topic.c_str(), topicSendName, strlen(topicSendName)))
  {
    //No se ha detectado al otro usuario
    if(memcmp(message, bobUser, length) && (memcmp(message, clientName, length)))
    {
      Serial.print("Solicitud de intercambio de llaves recibida.\n");
      Serial.print("Usuario: ");
      Serial.println(messageTemp);

      memcpy(bobUser, message, length);
      client.publish(topicSendName, clientName);

      strcpy(topicBob, "ESP32Crypto/");
      strcat(topicBob, bobUser);
      client.subscribe(topicBob);
      Serial.print("Suscrito a: ");
      Serial.println(topicBob);
      
      byteToHexChar(alice_k, bufferHex, MAX_KEY_SIZE, MAX_KEY_SIZE*2);
      client.publish(topicAlice, bufferHex);
    }

    //Si ya se conoce el nombre del otro usuario
    if(!memcmp(bobUser, message, length))
    {
      byteToHexChar(alice_k, bufferHex, MAX_KEY_SIZE, MAX_KEY_SIZE*2);
      client.publish(topicAlice, bufferHex);  
    }
  }

  //Llega llave publica de bob
  if((!memcmp(topic.c_str(), topicBob, strlen(topic.c_str()))) && !alice_privateKeyState)
  {
      Serial.print("Recibida llave publica de usuario ");
      Serial.println(bobUser);

      hexCharToByte((char*) message, bob_k, MAX_KEY_SIZE*2, MAX_KEY_SIZE);
      alice_privateKeyState = aliceFase2(bob_k, alice_f);

      Serial.print("Llave privada generada: ");
      byteToHexChar(bob_k, bufferHex, MAX_KEY_SIZE, MAX_KEY_SIZE*2);
      Serial.println(bufferHex);
      memcpy(vectorchachapoly.key, bob_k, MAX_KEY_SIZE);

      //Valores para el iv y authData se toman de la misma llave
      for(size_t i = 0; i < vectorchachapoly.ivsize ; i++)
      {
        vectorchachapoly.iv[i] = vectorchachapoly.key[i*2];
        vectorchachapoly.authdata[i] = vectorchachapoly.key[i*2+1];
      }

      strcpy(topicBobPlaintext, bobUser);
      strcat(topicBobPlaintext, "/");
      strcat(topicBobPlaintext, "plaintext");

      strcpy(topicBobCiphertext, bobUser);
      strcat(topicBobCiphertext, "/");
      strcat(topicBobCiphertext, "ciphertext");

      strcpy(topicBobTag, bobUser);
      strcat(topicBobTag, "/");
      strcat(topicBobTag, "tag");
      //client.subscribe(topicBobPlaintext);
      client.subscribe(topicBobCiphertext);
      client.subscribe(topicBobTag);

      Serial.print("El sistema esta listo para encriptar y descifrar.\n\n");
  }

  //Si se desconecta usuario
  if(!memcmp(topic.c_str(), topicWill, strlen(topicWill)))
  {
    if(!memcmp(bobUser, message, length))
    {
      Serial.print("Desconexion usuario ");
      Serial.println(messageTemp);
    
      alice_privateKeyState = 0;
      alice_publicKeyState = 0;
      topicBob[0] = '\0';
      bobUser[0] = '\0';
      client.unsubscribe(topicBob);
      client.unsubscribe(topicBobCiphertext);
      client.unsubscribe(topicBobPlaintext);
      client.unsubscribe(topicBobTag);

      alice_publicKeyState = aliceFase1(alice_k, alice_f);
      Serial.print("Reanudando solicitudes de llave publica.\n\n");
    }
  }

  //Si se recibe mensaje encriptado
  if (!memcmp(topic.c_str(), topicBobCiphertext, strlen(topic.c_str())) && alice_privateKeyState)
  {
    Serial.println("Mensaje cifrado recibido.");
    hexCharToByte((char*) message, vectorchachapoly.ciphertext, MAX_CIPHERTEXT_SIZE*2, MAX_CIPHERTEXT_SIZE);
    decryptCipherChaChaPoly(&chachapoly, &vectorchachapoly);
  }

  //Si se recibe tag
  if(!memcmp(topic.c_str(), topicBobTag, strlen(topic.c_str())) && alice_privateKeyState)
    {
      hexCharToByte((char*) message, vectorchachapoly.tag, vectorchachapoly.tagsize*2, vectorchachapoly.tagsize);
      Serial.println("Tag registrada en memoria.");
    }
}


void reconnect()
{
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName, "", "", topicWill, 2, false, clientName))
    {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      client.subscribe(topicSendName);
      client.subscribe(topicWill);
    
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
