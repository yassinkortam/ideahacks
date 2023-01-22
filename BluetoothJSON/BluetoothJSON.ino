#include "BluetoothSerial.h"
#include <ArduinoJson.h>
 
BluetoothSerial SerialBT;
 
void setup() {
  Serial.begin(115200);
 
  if(!SerialBT.begin("Team49ESP")){
    Serial.println("An error occurred initializing Bluetooth");
  }
}
 
void loop() {

  String a = "123";

    /* Generate an InsertOne Payload*/
  DynamicJsonDocument payload (1024);
  payload["collection"] = "user-stats";
  payload["database"] = "ideahacks";
  payload["dataSource"] = "AtlasCluster";
  payload["document"]["_id"] = "62cc3b7d0cdd2b2c8dee4434";
  payload["document"]["name"] = "Nathan";
  payload["document"]["steps"] = 4;
  payload["document"]["reuse"] = 1000;

  String JSONText;
  size_t JSONlength = serializeJson(payload, JSONText);

  for (int i = 0; i < JSONText.length(); i++)
    SerialBT.write(JSONText.charAt(i));
  
  SerialBT.write('\n');
 
  delay(5000);
}