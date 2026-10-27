#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "3701+";           
const char* password = "ISIC2026@";     
const char* mqtt_server = "broker.hivemq.com"; 

WiFiClient espClient;
PubSubClient client(espClient);

const int sensorPin = 4;
const int ledPin = 8;  
int estadoAnterior = -1; 

void setup() {
  pinMode(sensorPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  conectarWiFi();
  client.setServer(mqtt_server, 1883);
}

void conectarWiFi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32C3-Puerta-";
    clientId += String(random(0xffff), HEX);
    
    if (client.connect(clientId.c_str())) {
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int estadoSensor = digitalRead(sensorPin);

  if (estadoSensor == LOW) { 
    digitalWrite(ledPin, HIGH); 
  } else {
    digitalWrite(ledPin, LOW);
  }

  if (estadoSensor != estadoAnterior) {
    estadoAnterior = estadoSensor;
    
    if (estadoSensor == HIGH) {
      client.publish("casa/puerta/estado", "abierto");
    } else {
      client.publish("casa/puerta/estado", "cerrado");
    }
  }

  delay(150);
}