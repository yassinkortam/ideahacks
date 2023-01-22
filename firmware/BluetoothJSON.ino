#include "BluetoothSerial.h"
#include <ArduinoJson.h>
 
BluetoothSerial SerialBT;
 
void setup() {
  Serial.begin(115200);
 
  if(!SerialBT.begin("Team49ESP")){
    Serial.println("An error occurred initializing Bluetooth");
  }
}

String arrayToString(){
  String myarray = "[";
  for (int i=0; i<100; i++){
    myarray += String(random(-10,10));
    myarray += ",";
  }
  myarray[myarray.length() - 1] = "]";
  return myarray;
}
 
void loop() {

  String a = "123";

    /* Generate an InsertOne Payload*/
  DynamicJsonDocument payload (1024);
  
  payload["document"]["steps"] = 1;
  payload["document"]["reuse"] = 1;
  payload["document"]["accel"] = arrayToString();
  payload["document"]["gyro"] = arrayToString();

  String JSONText;
  size_t JSONlength = serializeJson(payload, JSONText);
  JSONText = "START" + JSONTEXT + "END";

  for (int i = 0; i < JSONText.length(); i++)
    SerialBT.write(JSONText.charAt(i));
  
  SerialBT.write('\n');
 
  delay(5000);
}