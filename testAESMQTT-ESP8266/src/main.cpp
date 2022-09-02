#include <Arduino.h>
#include <AesSimplified.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ByteConversions.h>

//Credenciales de Router
const char* ssid = "UbeeD963";
const char* password = "5216F9D963";

//Nombre cliente y topics a utilizar
const char* clientName = "ESP8266testAesMQTT_0";
const char* topicPlaintextESP8266 = "ESP8266Crypto/AES256/plaintext";
const char* topicCiphertextESP8266 = "ESP8266Crypto/AES256/ciphertext";
const char* topicPlaintextESP32 = "ESP32Crypto/AES256/plaintext";
const char* topicCiphertextESP32 = "ESP32Crypto/AES256/ciphertext";

// direccion server mqtt
const char* mqtt_server = "broker.hivemq.com";

//Inicializar EspClient
WiFiClient ESP8266_testAES;
PubSubClient client(ESP8266_testAES);

//Declarar valores para encriptar
  vectorAES vectorAes256 = 
  {
    .key = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 
            0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70,
            0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
            0x79, 0x7A, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46}
  };
  AES256 aes256;

//Declaracion de funciones
void setup_wifi();          //Conecta tarjeta ESP8266 al router wifi
void callback(String topic, byte* message, unsigned int length);  //Detecta cuando se recibe un mensaje en un topic suscrito
void reconnect();           //reconecta la tarjeta ESP8266 al broker MQTT

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
    strToByte(bufferString, vectorAes256.plaintext, sizeof(bufferString), MAX_PLAINTEXT_SIZE);
    byteToCharArray(vectorAes256.plaintext, bufferChar, MAX_PLAINTEXT_SIZE);
    encryptBlockAES256(&aes256, &vectorAes256);
    byteToHexChar(vectorAes256.ciphertext, bufferHex, MAX_CIPHERTEXT_SIZE, MAX_CIPHERTEXT_SIZE*2);

    client.publish(topicPlaintextESP8266, bufferChar);
    client.publish(topicCiphertextESP8266, bufferHex);
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

  //Llegada de un mensaje cifrado
  if (!memcmp(topic.c_str(), topicCiphertextESP32, strlen(topicCiphertextESP32)))
  {
    Serial.print("Mensaje cifrado recibido: ");
    Serial.println(messageTemp);
    hexCharToByte((char*) message, vectorAes256.ciphertext, MAX_PLAINTEXT_SIZE*2, MAX_PLAINTEXT_SIZE);

    decryptBlockAES256(&aes256, &vectorAes256);
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
      client.subscribe(topicCiphertextESP32);
      client.subscribe(topicPlaintextESP32);
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

