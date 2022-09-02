#include <Arduino.h>
#include <ChaChaPolySimplified.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ByteConversions.h>

//Credenciales de Router
const char* ssid = "UbeeD963";
const char* password = "5216F9D963";

//Nombre cliente y topics a utilizar
const char* clientName = "ESP32testChaChaPolyMQTT_0";
const char* topicPlaintextESP32 = "ESP32Crypto/ChaChaPoly/plaintext";
const char* topicCiphertextESP32 = "ESP32Crypto/ChaChaPoly/ciphertext";
const char* topicTagESP32 = "ESP32Crypto/ChaChaPoly/tag";
const char* topicPlaintextESP8266 = "ESP8266Crypto/ChaChaPoly/plaintext";
const char* topicCiphertextESP8266 = "ESP8266Crypto/ChaChaPoly/ciphertext";
const char* topicTagESP8266 = "ESP8266Crypto/ChaChaPoly/tag";

// direccion server mqtt
const char* mqtt_server = "broker.hivemq.com";

//Inicializar EspClient
WiFiClient ESP32_testChaChaPoly;
PubSubClient client(ESP32_testChaChaPoly);

//Declarar valores para encriptar
vectorChaChaPoly vectorchachapoly =
{
  .key          = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 
                  0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
                  0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
                  0x79, 0x7A, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46},
  .keySize      = MAX_KEY_SIZE,
  .plaintext    = {},
  .ciphertext   = {},
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


//Declaracion de funciones
void setup_wifi();          //Conecta tarjeta ESP32 al router wifi
void callback(String topic, byte* message, unsigned int length);  //Detecta cuando se recibe un mensaje en un topic suscrito
void reconnect();           //reconecta la tarjeta ESP32 al broker MQTT

//Variables a utilizar
String bufferString = "";
char bufferHex[MAX_PLAINTEXT_SIZE*2 + 1];
char bufferChar[MAX_PLAINTEXT_SIZE + 1];


void setup() 
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() 
{
  if(!client.connected())
    reconnect();

  if(!client.loop())
    client.connect(clientName);

  if(Serial.available() > 0)
  {
    bufferString = Serial.readString();
    strToByte(bufferString, vectorchachapoly.plaintext, bufferString.length(), MAX_PLAINTEXT_SIZE);
    byteToCharArray(vectorchachapoly.plaintext, bufferChar, MAX_PLAINTEXT_SIZE);
    encryptCipherChaChaPoly(&chachapoly, &vectorchachapoly);

    //Publicar Tag primero
    byteToHexChar(vectorchachapoly.tag, bufferHex, vectorchachapoly.tagsize, vectorchachapoly.tagsize*2);
    client.publish(topicTagESP32, bufferHex);

    //Publicar texto cifrado
    byteToHexChar(vectorchachapoly.ciphertext, bufferHex, MAX_CIPHERTEXT_SIZE, MAX_CIPHERTEXT_SIZE*2);
    client.publish(topicPlaintextESP32, bufferChar);
    client.publish(topicCiphertextESP32, bufferHex);
  }
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
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  //Se recibe tag de usuario externo
  if(!memcmp(topic.c_str(), topicTagESP8266, strlen(topicTagESP8266)))
  {
    Serial.print("Tag recibida: ");
    hexCharToByte((char*) message, vectorchachapoly.tag, vectorchachapoly.tagsize*2, vectorchachapoly.tagsize);
    for (size_t i = 0; i < vectorchachapoly.tagsize; i++)
      Serial.write(vectorchachapoly.tag[i]);
    Serial.println();
    
  }

  //Se recibe mensaje cifrado
  if(!memcmp(topic.c_str(), topicCiphertextESP8266, strlen(topicCiphertextESP8266)))
  {
    Serial.print("Mensaje encriptado recibido.\n");
    hexCharToByte((char*) message, vectorchachapoly.ciphertext, MAX_CIPHERTEXT_SIZE*2, MAX_CIPHERTEXT_SIZE);
    decryptCipherChaChaPoly(&chachapoly, &vectorchachapoly);
  }
}

void reconnect()
{
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) 
    {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe(topicCiphertextESP8266);
      client.subscribe(topicPlaintextESP8266);
      client.subscribe(topicTagESP8266);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

