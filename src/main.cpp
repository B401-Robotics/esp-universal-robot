#include <Arduino.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

// PIN DEFINE
uint8_t RELAY[4] = {32, 33, 27, 25};

// NETWORK DEFINE
#define DNS_PORT 53
#define SSID "ESP32-UNIVERSAL-ROBOT"

const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

DNSServer dnsServer;
AsyncWebServer server(80);
// AsyncWebSocket ws("/ws");

void setup() {
    Serial.begin(115200);

    // Hardware Setup
    for (int i = 0; i < 4; i++) pinMode(RELAY[i], OUTPUT);

    WiFi.disconnect();    // added to start with the wifi off, avoid crashing
    WiFi.mode(WIFI_OFF);  // added to start with the wifi off, avoid crashing
    WiFi.mode(WIFI_AP);

    WiFi.softAP(SSID);

    WiFi.softAPConfig(apIP, apIP, gateway);
    dnsServer.start(DNS_PORT, "*", apIP);
    Serial.printf("[AP] WiFi AP is now running\n");
    Serial.printf("[AP] IP address: %s\n", WiFi.softAPIP().toString().c_str());

    if (!LittleFS.begin()) {
        Serial.printf("[AP] An Error has occurred while mounting LittleFS\n");
        return;
    }

    // HTTP REQUEST
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.printf("[AP] Serving index.html\n");

        // Load your JSON data
        DynamicJsonDocument jsonDoc(1024);
        jsonDoc["key1"] = "value1";
        jsonDoc["key2"] = 123;

        // Convert the JSON data to a string
        String jsonData;
        serializeJson(jsonDoc, jsonData);

        // Combine the JSON data with the HTML template
        String combinedData = "<script>var jsonData = " + jsonData + ";</script>";

        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html", "text/html", combinedData);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    // HTTP POST
    server.on(
        "/relay", HTTP_POST, [](AsyncWebServerRequest *request) {
            if (request->hasParam("relayNumber", true)) {
                uint8_t relayNumber = request->getParam("relayNumber", true)->value().toInt();
                uint8_t relayState = request->getParam("relayState", true)->value().toInt();

                digitalWrite(RELAY[relayNumber], relayState);
            }

            request->send(200, "application/json", "{\"type\":\"message\",\"message\":\"OK\"}");
        });

    File root = LittleFS.open("/");
    while (File file = root.openNextFile()) {
        String filename = "/" + String(file.name());
        server.on(filename.c_str(), HTTP_GET, [filename](AsyncWebServerRequest *request) {
            Serial.printf("[AP] Serving %s\n", filename.c_str());

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

    // Captive portal to keep the client
    server.on("*", HTTP_GET, [](AsyncWebServerRequest *request) { request->redirect("http://" + apIP.toString()); });
    server.begin();

    Serial.printf("[AP] Webserver is now running\n");
}

void loop() {
    dnsServer.processNextRequest();
    delay(1000);
}