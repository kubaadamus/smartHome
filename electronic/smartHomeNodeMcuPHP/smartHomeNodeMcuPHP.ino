// IMPORTY
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;

// ZMIENNE I OBIEKTY GLOBALNE
String apiPath = "http://www.fitcalc.cba.pl/backend.php";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WLAN1-76L9H0", "ciuftechniczny");
}
void loop() {
  getRequests();
  updateSensorData();
  delay(2000);
}
void httpRequest(ESP8266WiFiMulti &WM,WiFiClient &cli,HTTPClient &ht,String formData){
     if ((WM.run() == WL_CONNECTED) && http.begin(cli, apiPath)) { 
      ht.addHeader("Content-Type", "application/x-www-form-urlencoded");
      auto httpCode = ht.POST(formData);
      String payload = ht.getString();
      processRequest(payload);
      ht.end();
   }
}

void getRequests(){
    httpRequest(WiFiMulti,client,http,"action=getRequests&object={\"forArduino\":\"1\"}");
}

void updateSensorData(){
        String sensorsData ="";
        // PO KOLEI ODCZYTUJ I DODAWAJ OBIEKT JSONOWY
        // A0
        pinMode(A0,INPUT);
        int A0_val = analogRead(A0);
        sensorsData += "{\"sensorName\":\"photo1\",\"value\":\""+(String)A0_val+"\"},";
        delay(100);
        // D1
        pinMode(D1,INPUT);
        int D1_val = digitalRead(D1);
        sensorsData += "{\"sensorName\":\"push1\",\"value\":\""+(String)D1_val+"\"},";
        delay(100);
        // D2
        pinMode(D2,INPUT);
        int D2_val = digitalRead(D2);
        sensorsData += "{\"sensorName\":\"push2\",\"value\":\""+(String)D2_val+"\"},";
        delay(100);
        // D3
        pinMode(D3,INPUT);
        int D3_val = digitalRead(D3);
        sensorsData += "{\"sensorName\":\"push3\",\"value\":\""+(String)D3_val+"\"}";
        delay(100);

        // Zmontuj w calosc i wyslij
        httpRequest(WiFiMulti,client,http,"action=updateSensorData&object={\"sensors\":["+sensorsData+"]}");
}

void processRequest(String payload){
      Serial.println("Z serwera:");
      Serial.println(payload);
        delay(100);
      String requestID = getValue(payload,'"',5);
      String request = getValue(payload,'"',9);
      Serial.println(requestID);
      delay(100);
      Serial.println(request);
      delay(100);
      // TUTAJ OBSŁUGA REQUESTÓW
      if(request=="led1:on"){
        pinMode(D0,OUTPUT);
        digitalWrite(D0,HIGH);
        delay(100);
       httpRequest(WiFiMulti,client,http,"action=updateRequest&object={\"id\":\""+requestID+"\",\"status\":\"completed\",\"response\":\"led1:on\",\"inputName\":\"led1\",\"inputValue\":\"1\"}");
      }
      if(request=="led1:off"){
        pinMode(D0,OUTPUT);
        digitalWrite(D0,LOW);
        delay(100);
       httpRequest(WiFiMulti,client,http,"action=updateRequest&object={\"id\":\""+requestID+"\",\"status\":\"completed\",\"response\":\"led1:off\",\"inputName\":\"led1\",\"inputValue\":\"0\"}");
      }
}
 String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
