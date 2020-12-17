// IMPORTY
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "Sensor.h"
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;


// ZMIENNE I OBIEKTY GLOBALNE
String apiPath = "http://www.fitcalc.cba.pl/backend.php";
// DEFAULT STATESY SENSORÓW
int push1;
float photo1;

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WLAN1-76L9H0", "ciuftechniczny");

  //PINY
  pinMode(D7,INPUT);
  pinMode(A0,INPUT);
  // DEFAULT STATESY SENSORÓW
  push1 = digitalRead(D7);
  photo1 = analogRead(A0);
}

void loop() {

  if(checkForChanges(&push1,&photo1)){
 if ((WiFiMulti.run() == WL_CONNECTED) && http.begin(client, apiPath)) { 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // CO ROBIMY?
      String formData = "action=updateSensorData&object={\"sensors\":[";
      formData += "{\"id\":0,\"value\":"+String(digitalRead(D7))+"},"; // push sensor
      formData += "{\"id\":1,\"value\":"+String(analogRead(A0))+"}"; // photoresistor
      formData += "]}";
     updateSensors(formData);
    http.end();
   }
  }
  delay(100);
}

boolean checkForChanges(int*push1,float*photo1){
  if(digitalRead(D7) != *push1 || analogRead(A0)!= *photo1){
    *push1 = digitalRead(D7);
    *photo1 = analogRead(A0);
    return true;
  }else{
    return false;
  }
}

void readSensors(){
    String formData = "action=getSensorData";
    auto httpCode = http.POST(formData);
    String payload = http.getString();
    Serial.println(payload);
}
void updateSensors(String _formData){
    String formData = _formData;
    auto httpCode = http.POST(formData);
    String payload = http.getString();
    Serial.println(payload);
}
