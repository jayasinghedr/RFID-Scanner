#ifndef WIFI_ACCESS_H
#define WIFI_ACCESS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Login to WiFi
// need to find a way to enter the SSID and password
// const char* ssid = "AndroidAP24FB"; 
// const char* password = "whxt3128";  
const char* ssid = "DHANUJA"; 
const char* password = "19990527"; 

String url = "http://217.76.63.83:6060/authenticate-clients";


int httpCode = 0;           // HTTP status code
char jsonOutput[128];       // store JSON string 
char jsonInput[256];
uint32_t chip_id = 0;
String device_id = "12";    //store device ID received from endpoint
String class_id = "";       //store class ID from keypad


void initialise_WiFi () {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
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