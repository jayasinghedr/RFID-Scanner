#ifndef WIFI_ACCESS_H
#define WIFI_ACCESS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Login to WiFi
// need to find a way to enter the SSID and password
const char* ssid = "AndroidAP24FB"; 
const char* password = "whxt3128";  

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

#endif