#include <Arduino.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>

// PIN DEFINE
uint8_t RELAY[6] = {16, 17, 18, 19, 21, 2};

// NETWORK DEFINE
#define DNS_PORT 53
#define SSID "ESP32-UNIVERSAL-ROBOT"

const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");
AsyncWebSocketClient *wsClient;

// initial device state
AsyncWebSocketClient *clients[16];

void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                    void *arg, uint8_t *data, size_t len) {
    DynamicJsonDocument json(1024);
    JsonArray dataJson = json.createNestedArray("data");
    String ack;

    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());

            wsClient = client;

            // send current state
            json["type"] = "ack";
            // send current state as int
            for (int i = 0; i < 6; i++) dataJson.add(digitalRead(RELAY[i]));

            // ACK with current state
            serializeJson(json, ack);
            client->text(ack);
            // store connected client
            for (int i = 0; i < 16; ++i) {
                if (clients[i] == NULL) {
                    clients[i] = client;
                    break;
                }
            }
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());

            wsClient = nullptr;
            // remove client from storage
            for (int i = 0; i < 16; ++i)
                if (clients[i] == client) clients[i] = NULL;
            break;
        case WS_EVT_DATA:
            if (deserializeJson(json, (char *)data, len)) {
                Serial.printf("WebSocket client #%u sent invalid JSON!\n", client->id());
                return;
            }

            if (json["type"] == "control") {
                Serial.printf("WebSocket client #%u sent control JSON!\n", client->id());

                // set relay state
                for (int i = 0; i < 6; ++i) {
                    digitalWrite(RELAY[i], json["data"][i]);
                }

                json["type"] = "ack";

                serializeJson(json, ack);
                serializeJson(json, Serial);
                Serial.println();
                client->text(ack);
                for (int i = 0; i < 16; ++i) {
                    if (clients[i] != NULL && clients[i] != client) clients[i]->text(ack);
                }
            } else {
                Serial.printf("WebSocket client #%u sent invalid JSON!\n", client->id());
                return;
            }

            break;
        default:
            break;
    }
}
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

    // WebSocket
    websocket.onEvent(wsEventHandler);
    server.addHandler(&websocket);

    // HTTP REQUEST
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.printf("[AP] Serving index.html\n");

        AsyncWebServerResponse *response = request->beginResponse(LittleFS, "/index.html", "text/html");
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
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