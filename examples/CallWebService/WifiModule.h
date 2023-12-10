#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#endif

void SetUpWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID_NAME, SSID_PASSWORD);
  Serial.println("Connecting to Internet ...");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    attempts++;
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());

  } else {
    Serial.println('\n');
    Serial.println('I could not connect to the wifi network after 10 attempts \n');
  }

  delay(500);
}

void CheckInternetConexion() {
  if (WiFi.status() != WL_CONNECTED) {
    SetUpWifi();
  }
}