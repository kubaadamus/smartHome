#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "REMSTAL";
const char* password = "remstalremstal";
bool ledState = 0;
const int ledPin = 2;
int port = 80;
// Create AsyncWebServer object on port 80
AsyncWebServer server(port);
AsyncWebSocket ws("/ws");
void notifyClients(String message) {
  ws.textAll(message);
}
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;

    if(strcmp((char*)data, "DWD0L") == 0)
    {
      pinMode(D0,OUTPUT);
      digitalWrite(D0,LOW);
      
    }
     if(strcmp((char*)data, "DWD0H") == 0)
    {
      pinMode(D0,OUTPUT);
      digitalWrite(D0,HIGH);
    }
    String message = "Wykonano ";
    message.concat((char*)data);
    notifyClients(message);
   
  }
}
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
         notifyClients("Polaczono!");
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
         notifyClients("Rozlaczono!");
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}
void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}
void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
  Serial.println("port:");
  Serial.println(port);
  initWebSocket();
  server.begin();
  ws.cleanupClients();
}

void loop() {
 // 
}
