#include <Arduino.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

#include "secret.h"

// PIN DEFINE
uint8_t RELAY[6] = {15, 4, 16, 17, 18, 19};

const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);

    // Hardware Setup
    for (int i = 0; i < 6; i++) pinMode(RELAY[i], OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    Serial.printf("[STA] Connecting to %s\n", SSID);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    pinMode(LED_BUILTIN, HIGH);

    Serial.printf("Connected to %s\n", SSID);
    Serial.printf("IP Address: %s\n", WiFi.localIP().toString().c_str());

    if (!LittleFS.begin()) {
        Serial.printf("[SERVER] An Error has occurred while mounting LittleFS\n");
        return;
    }

    // HTTP REQUEST
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.printf("[SERVER] Serving index.html\n");

        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    File root = LittleFS.open("/");
    while (File file = root.openNextFile()) {
        String filename = "/" + String(file.name());
        server.on(filename.c_str(), HTTP_GET, [filename](AsyncWebServerRequest *request) {
            Serial.printf("[SERVER] Serving %s\n", filename.c_str());

            String contentType = filename.substring(filename.length() - 3);
            if (contentType == "tml" || contentType == "htm")
                contentType = "text/html";
            else if (contentType == "css")
                contentType = "text/css";
            else if (contentType == ".js")
                contentType = "text/javascript";
            else if (contentType == "son")
                contentType = "application/json";
            else if (contentType == "jpg" || contentType == "peg")
                contentType = "image/jpeg";
            else if (contentType == "png")
                contentType = "image/png";
            else if (contentType == "svg")
                contentType = "image/svg+xml";
            else if (contentType == "ttf")
                contentType = "application/x-font-truetype";
            else if (contentType == "otf")
                contentType = "application/x-font-opentype";
            else
                contentType = "text/plain";
            AsyncWebServerResponse *response = request->beginResponse(LittleFS, filename, contentType);
            response->addHeader("Content-Encoding", "gzip");
            request->send(response);
        });
    }

    // HTTP handlers to control relays
    for (int i = 0; i < 6; i++) {
        char endpoint[16];                      // buffer for endpoint string
        sprintf(endpoint, "/relay/%d", i + 1);  // create endpoint name

        server.on(endpoint, HTTP_GET, [i](AsyncWebServerRequest *request) {
            if (request->hasParam("state")) {
                String state = request->getParam("state")->value();

                if (state == "on") {
                    digitalWrite(RELAY[i], HIGH);  // turn on relay
                    request->send(200, "text/plain", "Relay " + String(i + 1) + " is ON");
                } else if (state == "off") {
                    digitalWrite(RELAY[i], LOW);  // turn off relay
                    request->send(200, "text/plain", "Relay " + String(i + 1) + " is OFF");
                } else {
                    request->send(400, "text/plain", "Invalid state. Use 'on' or 'off'.");
                }
            } else {
                request->send(400, "text/plain", "No 'state' parameter found. Use 'on' or 'off'.");
            }

            Serial.printf("[SERVER] Relay %d state: %s\n", i + 1, request->getParam("state")->value().c_str());
        });
    }

    server.begin();
    Serial.printf("Web server is now running\n");
}

void loop() {
    delay(1000);
}