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

const int FLEX_THUMB = 12; // Pin connected to voltage divider output
const int FLEX_INDEX = 2; // Pin connected to voltage divider output
// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 3.3; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
//const float STRAIGHT_RESISTANCE = 37300.0; // resistance when straight
//const float BEND_RESISTANCE = 90000.0; // resistance at 90 deg
const float STRAIGHT_RESISTANCE = 270000.0; // resistance when straight
const float BEND_RESISTANCE = 430000.0; // resistance at 90 deg

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
    //float flexVThumb = flexADCThumb * VCC / 1023.0;
    float flexVThumb = flexADCThumb * VCC / 4095.0;
    float flexRThumb = R_DIV * (VCC / flexVThumb - 1.0);
    //Serial.println("Resistance: " + String(flexRThumb) + " ohms");
  
    // Use the calculated resistance to estimate the sensor's
    // bend angleThumb:
    float angleThumb = map(flexRThumb, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                      0, 90.0);
  
    // Read the ADC, and calculate voltage and resistance from it
    int flexADCIndex = analogRead(FLEX_INDEX);
    //float flexVIndex = flexADCIndex * VCC / 1023.0;
    float flexVIndex = flexADCIndex * VCC / 4095.0;
    float flexRIndex = R_DIV * (VCC / flexVIndex - 1.0);
    //Serial.println("Resistance: " + String(flexRIndex) + " ohms");
  
    // Use the calculated resistance to estimate the sensor's
    // bend angleIndex:
    float angleIndex = map(flexRIndex, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                      0, 90.0);
    data = "RightThumb=";
    data += String(angleThumb);
    data += "&RightIndex=";
    data += String(angleIndex);
    data += "&RightMiddle=0.00";
    //data += analogRead(A1);
    data += "&RightRing=0.00";
    //data += analogRead(A1);
    data += "&RightPinky=0.00";
    //data += analogRead(A1);
    data += "&RightAngX=";
    data += String(mpu.getAngleX());
    data += "&RightAngY=";
    data += String(mpu.getAngleY());
    data += "&RightAngZ=";
    data += String(mpu.getAngleZ());
    /*data += "&RightMagX=";
    data += String(magX);
    data += "&RightMagY=";
    data += String(magY);
    data += "&RightMagZ=";
    data += String(magZ);
    data += "&RightAziZ=";
    data += String(magAzi);
    Serial.println(data);*/
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
    
    /*Serial.print(F("TEMPERATURE: "));Serial.printºln(mpu.getTemp());
    Serial.print(F("ACCELERO  X: "));Serial.print(mpu.getAccX());
    Serial.print("\tY: ");Serial.print(mpu.getAccY());
    Serial.print("\tZ: ");Serial.println(mpu.getAccZ());
  
    Serial.print(F("GYRO      X: "));Serial.print(mpu.getGyroX());
    Serial.print("\tY: ");Serial.print(mpu.getGyroY());
    Serial.print("\tZ: ");Serial.println(mpu.getGyroZ());
  
    Serial.print(F("ACC ANGLE X: "));Serial.print(mpu.getAccAngleX());
    Serial.print("\tY: ");Serial.println(mpu.getAccAngleY());
    
    Serial.print(F("ANGLE     X: "));Serial.print(mpu.getAngleX());
    Serial.print("\tY: ");Serial.print(mpu.getAngleY());
    Serial.print("\tZ: ");Serial.println(mpu.getAngleZ());
    Serial.println(F("=====================================================\n"));*/
    
    timer = millis();
  }

}
