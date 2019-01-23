#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <ESP8266HTTPClient.h>

#define IP "http://192.168.43.251:3001/api/newreading"

Adafruit_BME280 sensor;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Wire.begin();
  if(!sensor.begin(0x76)) {
    Serial.println("Error: Sensor not found!");
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Pixel_3", "88888888");
}

void loop() {
  StaticJsonBuffer<JSON_OBJECT_SIZE(4)> jbuffer;
  String output = "";
  JsonObject& data = jbuffer.createObject();

  data["name"] = "Konsta Spurtti";
  data["temperature"] = sensor.readTemperature();
  data["pressure"] = sensor.readPressure();
  data["humidity"] = sensor.readHumidity();

  data.prettyPrintTo(Serial);
  Serial.println();
  data.printTo(output);
  Serial.println();

  if (WiFiMulti.run() == WL_CONNECTED){
    HTTPClient http;

    http.begin(IP);
    http.addHeader("content-type", "application/json");

    int httpcode = http.POST(output);
    if (httpcode == HTTP_CODE_OK) {
      Serial.println("Toimii :D");
    } else {
      Serial.println("Ei toimi :(");
      Serial.println(httpcode);
    }
  }
  Serial.println("---------------------------------------------------------------");
  delay(5000);
}