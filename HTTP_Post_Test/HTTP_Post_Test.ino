#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// const uint16_t PixelCount = 50; // this example assumes 4 pixels, making it smaller will cause a failure
// const uint8_t PixelPin = 27;  // make sure to set this to the correct pin, ignored for Esp8266

// NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

/************ Useful Setup Constants *****************/


const char* AtlasAPIEndpoint = "https://data.mongodb-api.com/app/data-wpsdz/endpoint/data/v1/action/insertOne";
const char* AtlasAPIKey = "fZTsW8QtgNa6R5ZxgcgA9tEDvVF9dsHPYw6Hfq64eziOPnH72MD4HDb4JdCOR3HI";
const char* ssid = "IEEE";
const char* password = "Ilovesolder";

IPAddress staticIP(192, 168, 1, 245);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);

/* This one Works with Atlas OK*/

const char* rootCACertificate = \
                                "-----BEGIN CERTIFICATE-----\n" \
                                "MIIF6TCCA9GgAwIBAgIQBeTcO5Q4qzuFl8umoZhQ4zANBgkqhkiG9w0BAQwFADCB\n" \
                                "iDELMAkGA1UEBhMCVVMxEzARBgNVBAgTCk5ldyBKZXJzZXkxFDASBgNVBAcTC0pl\n" \
                                "cnNleSBDaXR5MR4wHAYDVQQKExVUaGUgVVNFUlRSVVNUIE5ldHdvcmsxLjAsBgNV\n" \
                                "BAMTJVVTRVJUcnVzdCBSU0EgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMTQw\n" \
                                "OTEyMDAwMDAwWhcNMjQwOTExMjM1OTU5WjBfMQswCQYDVQQGEwJGUjEOMAwGA1UE\n" \
                                "CBMFUGFyaXMxDjAMBgNVBAcTBVBhcmlzMQ4wDAYDVQQKEwVHYW5kaTEgMB4GA1UE\n" \
                                "AxMXR2FuZGkgU3RhbmRhcmQgU1NMIENBIDIwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
                                "DwAwggEKAoIBAQCUBC2meZV0/9UAPPWu2JSxKXzAjwsLibmCg5duNyj1ohrP0pIL\n" \
                                "m6jTh5RzhBCf3DXLwi2SrCG5yzv8QMHBgyHwv/j2nPqcghDA0I5O5Q1MsJFckLSk\n" \
                                "QFEW2uSEEi0FXKEfFxkkUap66uEHG4aNAXLy59SDIzme4OFMH2sio7QQZrDtgpbX\n" \
                                "bmq08j+1QvzdirWrui0dOnWbMdw+naxb00ENbLAb9Tr1eeohovj0M1JLJC0epJmx\n" \
                                "bUi8uBL+cnB89/sCdfSN3tbawKAyGlLfOGsuRTg/PwSWAP2h9KK71RfWJ3wbWFmV\n" \
                                "XooS/ZyrgT5SKEhRhWvzkbKGPym1bgNi7tYFAgMBAAGjggF1MIIBcTAfBgNVHSME\n" \
                                "GDAWgBRTeb9aqitKz1SA4dibwJ3ysgNmyzAdBgNVHQ4EFgQUs5Cn2MmvTs1hPJ98\n" \
                                "rV1/Qf1pMOowDgYDVR0PAQH/BAQDAgGGMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYD\n" \
                                "VR0lBBYwFAYIKwYBBQUHAwEGCCsGAQUFBwMCMCIGA1UdIAQbMBkwDQYLKwYBBAGy\n" \
                                "MQECAhowCAYGZ4EMAQIBMFAGA1UdHwRJMEcwRaBDoEGGP2h0dHA6Ly9jcmwudXNl\n" \
                                "cnRydXN0LmNvbS9VU0VSVHJ1c3RSU0FDZXJ0aWZpY2F0aW9uQXV0aG9yaXR5LmNy\n" \
                                "bDB2BggrBgEFBQcBAQRqMGgwPwYIKwYBBQUHMAKGM2h0dHA6Ly9jcnQudXNlcnRy\n" \
                                "dXN0LmNvbS9VU0VSVHJ1c3RSU0FBZGRUcnVzdENBLmNydDAlBggrBgEFBQcwAYYZ\n" \
                                "aHR0cDovL29jc3AudXNlcnRydXN0LmNvbTANBgkqhkiG9w0BAQwFAAOCAgEAWGf9\n" \
                                "crJq13xhlhl+2UNG0SZ9yFP6ZrBrLafTqlb3OojQO3LJUP33WbKqaPWMcwO7lWUX\n" \
                                "zi8c3ZgTopHJ7qFAbjyY1lzzsiI8Le4bpOHeICQW8owRc5E69vrOJAKHypPstLbI\n" \
                                "FhfFcvwnQPYT/pOmnVHvPCvYd1ebjGU6NSU2t7WKY28HJ5OxYI2A25bUeo8tqxyI\n" \
                                "yW5+1mUfr13KFj8oRtygNeX56eXVlogMT8a3d2dIhCe2H7Bo26y/d7CQuKLJHDJd\n" \
                                "ArolQ4FCR7vY4Y8MDEZf7kYzawMUgtN+zY+vkNaOJH1AQrRqahfGlZfh8jjNp+20\n" \
                                "J0CT33KpuMZmYzc4ZCIwojvxuch7yPspOqsactIGEk72gtQjbz7Dk+XYtsDe3CMW\n" \
                                "1hMwt6CaDixVBgBwAc/qOR2A24j3pSC4W/0xJmmPLQphgzpHphNULB7j7UTKvGof\n" \
                                "KA5R2d4On3XNDgOVyvnFqSot/kGkoUeuDcL5OWYzSlvhhChZbH2UF3bkRYKtcCD9\n" \
                                "0m9jqNf6oDP6N8v3smWe2lBvP+Sn845dWDKXcCMu5/3EFZucJ48y7RetWIExKREa\n" \
                                "m9T8bJUox04FB6b9HbwZ4ui3uRGKLXASUoWNjDNKD/yZkuBjcNqllEdjB+dYxzFf\n" \
                                "BT02Vf6Dsuimrdfp5gJ0iHRc2jTbkNJtUQoj1iM=\n" \
                                "-----END CERTIFICATE-----\n";





// WiFiClient client;
const int pollInterval = 200;    // time between each check for new pattern
unsigned long previousMillis = 0;   // last time image was sent
char lastid[25] = "0";

void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
    Serial.println("Configuration failed.");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));

}

/* Simple loop to take a shot every 2000 milliseconds*/

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= pollInterval) {
    getLightDefinition();
    previousMillis = currentMillis;
  }
}

void getLightDefinition() {

  WiFiClientSecure *client = new WiFiClientSecure;
  if (client) {
    client -> setCACert(rootCACertificate);
    {
      //Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;

      if (https.begin(*client, AtlasAPIEndpoint)) {  // HTTPS

        /* Headers Required for Data API*/
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Access-Control-Request-Headers", "*");
        https.addHeader("api-key", AtlasAPIKey);

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
        Serial.println(JSONText);
        int httpCode = https.sendRequest("POST", JSONText);


        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] POST... code: %d\n", httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);
            DynamicJsonDocument description(32687);
            DeserializationError error = deserializeJson(description, payload);
            // Test if parsing succeeds.
            // if (error) {
            //   Serial.print(F("deserializeJson() failed: "));
            //   Serial.println(error.f_str());
            //   delete client; 
            //   return;
            // }
          //   if(description["documents"].size() == 0) { Serial.println("No Change"); delete client; return;}
            
          //   JsonVariant lights = description["documents"][0]["state"];
          //   if(! lights.is<JsonArray>()) {
          //     Serial.println("state is not an array");
          //     return;
          //   }
          
          //   setLights(lights.as<JsonArray>());
          //  // strncpy(lastid,description["documents"][0]["_id"],24); 
          }
        } else {
          Serial.printf("[HTTPS] POST ... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      } 
      else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }

      // End extra scoping block
    }

  //   delete client;
  } else {
    Serial.println("Unable to create client");
  }

}
