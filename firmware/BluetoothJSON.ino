#include "BluetoothSerial.h"
#include <ArduinoJson.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Clock Req
#include <sys/time.h>


// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
 
BluetoothSerial SerialBT;

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
// You can also use software SPI
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);

int step_count = 0;
int reuse_count = 0;
float gyrox_data[50];
float gyroy_data[50];
float gyroz_data[50];
float accelx_data[50];
float accely_data[50];
float accelz_data[50];


void pedometer(int x,int y,int z) {   // Detect whether we have a step or reuse or nothing
    float magnitude = sqrt(x*x + y*y + z*z); // calculate the magnitude of the acceleration vector
    if (x > 0 && magnitude > 10) {
      step_count++;
      Serial.println("Step added");
    }
    else if (x < -7)
    {
      reuse_count++;
      Serial.println("Washing added");
    }
}

void setupSensor()  // Accelerometer setup
{
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
}

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
 
    while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }
  
  Serial.println("LSM9DS1 data read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  setupSensor();

    // I2C Initialization
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Time Preset
  int set_hour = 9; // Preset Hours
  int set_min = 17; // Preset Minutes
  struct timeval timeset;
  timeset.tv_sec = set_hour*3600 + set_min*60;
  settimeofday(&timeset, NULL);

  if(!SerialBT.begin("EcoBit")){
    Serial.println("An error occurred initializing Bluetooth");
  }
}

String arrayToString(float a[]){
  String myarray = "[";
  for (int i=0; i<50; i++){
    myarray += String(a[i]);
    myarray += ",";
  }
  myarray[myarray.length() - 1] = ']';
  return myarray;
}
 
void loop() {
  step_count = 0;
  reuse_count = 0;

  float gx;
  float gy;
  float gz;

  float xx;
  float yy;
  float zz;

  for (int i = 0; i < 50; i++)    // For loop reads sensor data, fills data array, and decides how much to increment step_count and reuse_count
  {
    lsm.read();  /* ask it to read in the data */ 

    /* Get a new sensor event */ 
    sensors_event_t a, m, g, temp;

    lsm.getEvent(&a, &m, &g, &temp); 
  
    xx = a.acceleration.x;
    yy = a.acceleration.y;
    zz = a.acceleration.z;

    gx = g.gyro.x;
    gy = g.gyro.y;
    gz = g.gyro.z;

    gyrox_data[i] = gx;
    gyroy_data[i] = gy;
    gyroz_data[i] = gz;
    accelx_data[i] = xx;
    accely_data[i] = yy;
    accelz_data[i] = zz;

    pedometer(xx,yy,zz);
    delay(50);
  }

  /* Generate an InsertOne Payload*/
  DynamicJsonDocument payload (1024);
  
  payload["document"]["steps"] = step_count;
  payload["document"]["reuse"] = reuse_count;
  arrayToString(accelx_data).length() > 0 ? payload["document"]["accelx"] = arrayToString(accelx_data) : payload["document"]["accelx"] = "[]";
  arrayToString(accely_data).length() > 0 ? payload["document"]["accely"] = arrayToString(accely_data) : payload["document"]["accely"] = "[]";
  arrayToString(accelz_data).length() > 0 ? payload["document"]["accelz"] = arrayToString(accelz_data) : payload["document"]["accelz"] = "[]";
  arrayToString(gyrox_data).length() > 0 ? payload["document"]["gyrox"] = arrayToString(gyrox_data) : payload["document"]["gyrox"] = "[]";
  arrayToString(gyroy_data).length() > 0 ? payload["document"]["gyroy"] = arrayToString(gyroy_data) : payload["document"]["gyroy"] = "[]";
  arrayToString(gyroz_data).length() > 0 ? payload["document"]["gyroz"] = arrayToString(gyroz_data) : payload["document"]["gyroz"] = "[]";

  String JSONText = "";
  size_t JSONlength = serializeJson(payload, JSONText);

  for (int i = 0; i < JSONText.length(); i++)
    SerialBT.write(JSONText.charAt(i));
  
  SerialBT.write('\n');
 
  // Clock stuff
  time_t rawtime = time(nullptr);
  struct tm* timeinfo = localtime(&rawtime);

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,25);
  display.print(timeinfo->tm_hour);
  display.print(":");
  if( timeinfo->tm_min <10)
  display.print("0");
  display.print(timeinfo->tm_min);

  display.setTextSize(2);
  display.setCursor(90,30);
  display.print(":");
  if( timeinfo->tm_sec <10)
  display.print("0");
  display.print(timeinfo->tm_sec); 
  display.display();
}