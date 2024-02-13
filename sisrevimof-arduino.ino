/* Get all possible data from MPU6050
 * Accelerometer values are given as multiple of the gravity [1g = 9.81 m/s²]
 * Gyro values are given in deg/s
 * Angles are given in degrees
 * Note that X and Y are tilt angles and not pitch/roll.
 *
 * License: MIT
 */

#include "Wire.h"
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
long timer = 0;

const int FLEX_THUMB = 1; // Pin connected to voltage divider output
const int FLEX_INDEX = 2; // Pin connected to voltage divider output
const int FLEX_MIDDLE = 3; // Pin connected to voltage divider output
const int FLEX_RING = 6; // Pin connected to voltage divider output
const int FLEX_PINKY = 7; // Pin connected to voltage divider output

//Measurements for flex sensors
//412 - 322 - 326 - 462 - 470 -> 0
//220 - 190 - 170 - 360 - 370 -> 90
const int MAX_THUMB = 412;
const int MIN_THUMB = 220;
const int MAX_INDEX = 322;
const int MIN_INDEX = 190;
const int MAX_MIDDLE = 326;
const int MIN_MIDDLE = 170;
const int MAX_RING = 462;
const int MIN_RING = 360;
const int MAX_PINKY = 470;
const int MIN_PINKY = 370;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  pinMode(FLEX_THUMB, INPUT);
  pinMode(FLEX_INDEX, INPUT);
  delay(1000);
  mpu.calcOffsets(true,true); // gyro and accelero
  Serial.println("Done!\n");
  
}

void loop() {
  String data="";
  mpu.update();
  String magX = "";
  String magY = "";
  String magZ = "";
  String magAzi = "";
  if(millis() - timer > 100){ // print data every second
    int flexADCThumb = analogRead(FLEX_THUMB);
    int flexADCIndex= analogRead(FLEX_INDEX);
    int flexADCMiddle = analogRead(FLEX_MIDDLE);
    int flexADCRing = analogRead(FLEX_RING);
    int flexADCPinky = analogRead(FLEX_PINKY);

    float angleThumb = map(flexADCThumb, MIN_THUMB, MAX_THUMB, 0, 90.0);
    float angleIndex = map(flexADCIndex, MIN_INDEX, MAX_INDEX, 0, 90.0);
    float angleMiddle = map(flexADCMiddle, MIN_MIDDLE, MAX_MIDDLE, 0, 90.0);
    float angleRing = map(flexADCRing, MIN_RING, MAX_RING, 0, 90.0);
    float anglePinky = map(flexADCPinky, MIN_PINKY, MAX_PINKY, 0, 90.0);

    data = "RightThumb=";
    data += String(angleThumb);
    data += "&RightIndex=";
    data += String(angleIndex);
    data += "&RightMiddle=";
    data += String(angleMiddle);
    data += "&RightRing=";
    data += String(angleRing);
    data += "&RightPinky=";
    data += String(anglePinky);
    data += "&RightAngX=";
    data += String(mpu.getAngleX());
    data += "&RightAngY=";
    data += String(mpu.getAngleY());
    data += "&RightAngZ=";
    data += String(mpu.getAngleZ());
  
    data += "&RightAccX=";
    data += String(mpu.getAccX());
    data += "&RightAccY=";
    data += String(mpu.getAccY());
    data += "&RightAccZ=";
    data += String(mpu.getAccZ());
    data += "&RightGyroX=";
    data += String(mpu.getGyroX());
    data += "&RightGyroY=";
    data += String(mpu.getGyroY());
    data += "&RightGyroZ=";
    data += String(mpu.getGyroZ());
    Serial.println(data);
    
    timer = millis();
  }

}
