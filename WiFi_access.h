#ifndef WIFI_ACCESS_H
#define WIFI_ACCESS_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"

String url = "http://217.76.63.83:6060/authenticate-clients";


int httpCode = 0;           // HTTP status code
char jsonOutput[128];       // store JSON string 
char jsonInput[256];
uint32_t chip_id = 0;
String device_id = "12";    //store device ID received from endpoint
String class_id = "";       //store class ID from keypad

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";

//Variables to save values from HTML form
String ssid;
String pass;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";

IPAddress localIP;

// Set your Gateway IP address
IPAddress localGateway;

int wait_count = 0;

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Read File from SPIFFS
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}

// Write file to SPIFFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- file write failed");
  }
}

bool initWiFi() {

    if(ssid==""){
      Serial.println("Undefined SSID address.");
      return false;
    }

    WiFi.begin(ssid, pass);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
        wait_count++;
        if (wait_count > 10) {
          wait_count = 0;
          return 0;
        }       
    }
    return true;
}

int send_class_id () {
    HTTPClient client;

    client.begin(url);
    client.addHeader("Content-Type", "application/json");

    const size_t CAPACITY = JSON_OBJECT_SIZE(1);
    StaticJsonDocument<CAPACITY> doc;

    JsonObject object = doc.to<JsonObject>();
    object["client_id"] = class_id.toInt();

    serializeJson(doc, jsonOutput);

    httpCode = client.POST(String(jsonOutput));

    if (httpCode == 200) {
        Serial.print("Status code: ");
        Serial.println(httpCode);

        String payload = client.getString();
        
        payload.toCharArray(jsonInput, 500);

        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, jsonInput);

        if (error) {
            Serial.print("Failed to parse JSON: ");
            Serial.println(error.c_str());
            return 0;
        }

        const char* message = doc["message"];
        int client_id = doc["client_id"];

        Serial.print("message: ");
        Serial.println(message);
        Serial.print("client_id: ");
        Serial.println(client_id);

        client.end();
        return 1;
    }
    else if (httpCode == 422) {
        Serial.print("Status code: ");
        Serial.println(httpCode);

        String payload = client.getString();
        Serial.println(payload);
        
        client.end();
        return 0;
    }
    else {
        Serial.println("Error on HTTP request");
        client.end();
        return 0;
    }
}

#endif