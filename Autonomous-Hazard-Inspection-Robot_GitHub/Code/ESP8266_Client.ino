/*
 * ESP8266 Client for ThingSpeak Telemetry (sanitized)
 */
#include <ESP8266WiFi.h>

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* host     = "api.thingspeak.com";
const char* apiKey   = "YOUR_THINGSPEAK_API_KEY";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println(); Serial.println("WiFi connected!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    Serial.println(data);

    int c1 = data.indexOf(',');
    int c2 = data.indexOf(',', c1 + 1);
    int c3 = data.indexOf(',', c2 + 1);

    int distance_F  = data.substring(0, c1).toInt();
    int sensorValue = data.substring(c1 + 1, c2).toInt();
    float ambientC  = data.substring(c2 + 1, c3).toFloat();
    float objectC   = data.substring(c3 + 1).toFloat();

    WiFiClient client;
    if (client.connect(host, 80)) {
      String url = "/update?api_key="; url += apiKey;
      url += "&field1=" + String(distance_F);
      url += "&field2=" + String(sensorValue);
      url += "&field3=" + String(ambientC);
      url += "&field4=" + String(objectC);

      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");

      delay(10);
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
    }
  }
  delay(5000);
}
