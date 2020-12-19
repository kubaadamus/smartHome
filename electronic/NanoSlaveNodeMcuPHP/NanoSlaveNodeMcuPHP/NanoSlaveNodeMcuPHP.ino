#define portOneRX 2
#define portOneTX 3
#include <SoftwareSerial.h>
SoftwareSerial portOne(portOneRX, portOneTX); // RX / TX
String serialIn;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
portOne.begin(115200);
Serial.println("Siema");
pinMode(portOneRX,INPUT);
pinMode(portOneTX,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (portOne.available() > 0) {
    while (portOne.available()) {
      delay(10);  //delay to allow byte to arrive in input buffer
      char c = portOne.read();
      serialIn += c;
    }
    Serial.println("Odbieram:");
    Serial.println(serialIn);
    portOne.flush();
    serialIn = "";
  }

}
