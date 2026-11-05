#include <WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

#define DHTPIN 17
#define DHTTYPE DHT22

const char* ssid = "TECNO CAMON 40 Pro 5G";
const char* password = "santrifiwa";
const char* mqtt_server = "broker.hivemq.com"; // IP Host Mosquitto
const char* topic = "Rasyad";

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32_DHT22_Client")) {
      Serial.println("Terhubung ke Mosquitto Broker");
    } else {
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature();

  if (!isnan(humidity) && !isnan(tempC)) {
    String payload = "{\"temperature\":" + String(tempC, 1) + ",\"humidity\":" + String(humidity, 1) + "}";
    client.publish(topic, payload.c_str());
    Serial.print("Terkirim ke ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(payload);
  }

  delay(2000);
}
