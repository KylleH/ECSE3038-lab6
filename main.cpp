#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"
#include <Wire.h>
#include<stdio.h>



void setup() {
  Serial.begin(9600);
	pinMode(led_pin, OUTPUT);
  pinMode(temp_sensor, INPUT);
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(ssid, password);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // Establish a connection to the server
    http.begin(endpoint);
    http.addHeader("api-key",api_key);
    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    // Serialise JSON object into a string to be sent to the API
    StaticJsonDocument<64> doc;
    char httpRequestData[64]; // Allocate memory for JSON data

    doc["temp"] = getTemp();

		
		// convert JSON document, doc, to string and copies it into httpRequestData
    serializeJson(doc, httpRequestData);

    // Send HTTP POST request
    int httpResponseCode = http.PUT(httpRequestData);
    String http_response;

    // check reuslt of POST request. negative response code means server wasn't reached
    if (httpResponseCode>0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      Serial.print("HTTP Response from server: ");
      http_response = http.getString();
      Serial.println(http_response);

      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http_response);

      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      const char* variable_name = doc["key_name"];
      int another_variable = doc["key_attribute"];
      bool last_variable = doc["last_key"];
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
   
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
      
    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }

  httpget();
}
`

String getTemp() {
    const float BETA = 3950; // should match the Beta Coefficient of the thermistor
    int analogValue = analogRead(temp_sensor);
  float celsius = 1 / (log(1 / (4096. / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;
    char buffer[10];
  snprintf(buffer, sizeof(buffer), "%.2f", celsius); // Convert float to string with 2 decimal places
    String cels = buffer;
    return cels;
}


void httpget(){
  HTTPClient http;
  http.begin(endpoint1.c_str());
  http.addHeader("api-key",api_key);
  int httpResponseCode = http.GET();
  
   if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
String responseBody = http.getString();

JsonDocument doc;

DeserializationError error = deserializeJson(doc, responseBody);

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  return;
}

  bool light = doc["light"];
  Serial.print("Light status: ");
  digitalWrite(led_pin, light ? HIGH : LOW);



}
else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
