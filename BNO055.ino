#include "BNO055.h"

BNO055 mpu; //Create an instance of driver

void setup() {
  Serial.begin(115200); //Start serial comms
  while (!Serial) {}; //Wait for Serial to be ready, some arduino boards can use this, could also be set to timeout
  mpu.setTempSource(true); //Select gyro as temp source

  //Start running with normal power mode, full fusion op mode, P4 placement(see datasheet, and loads calibrations
  mpu.begin(POWER_MODE::NORMAL, OPERATION_MODE::FUSION_NDOF_NORMAL, PLACEMENT::P4, true);
}

float data[3]{0.0f, 0.0f, 0.0f};
bool calib = false;
void Calprint();
void HRPprint();
void accelprint();

void loop() {
  //Perform an action based on Serial input
  if (Serial.available() > 0) {
    char c = Serial.read();
    if (c == 's') mpu.saveCalib(); //Save calibrations
    if (c == 'l') mpu.loadCalib(); //Load calibrations
    if (c == 'c') Calprint(); //Print calibration status
    if (c == 'e') HRPprint(); //Print heading, roll and pitch(Euler angles)
    if (c == 'a') accelprint(); //Print linear acceleration values
    if (c == 't') { //Print measured temperature
      Serial.print("Temperature: ");
      Serial.println(mpu.getTemp());
      }  
    }
    //Save calibrations if calibration status is fully calibrated
    //Don't set calib to true if you always want it to print out when calibrated
    //Potentially useful if continuing to calibrate for unstable fields
    if (mpu.getCalibration() == 0xFF && !calib) {
      mpu.saveCalib();
      calib=true;
    }
    delay(500);
}

void Calprint() {
  int temp = mpu.getCalibration();
  Serial.println("             ToGyAcMa");
  Serial.print("Calibration: ");
  if (temp < 128) Serial.print("0");
  if (temp < 64) Serial.print("0");
  Serial.println(temp,BIN);
}

void HRPprint() {
  Serial.println("Eulers:");
  mpu.getEuler(data);
  Serial.print("Heading: ");
  Serial.println(data[0]);
  Serial.print("Roll: ");
  Serial.println(data[1]);
  Serial.print("Pitch: ");
  Serial.println(data[2]);
}

void accelprint() {
  Serial.print("Linear Acceleration X:");
  Serial.print(mpu.getLinearAccelX());
  Serial.print(" Y: ");
  Serial.print(mpu.getLinearAccelY());
  Serial.print(" Z: ");
  Serial.println(mpu.getLinearAccelZ());  
}

