#include "sensors.h"
#include <math.h>       
#define PI 3.14159265

int leftLastStateA = LOW;            
int leftLastStateB = LOW;
int rightLastStateA = LOW;            
int rightLastStateB = LOW;

volatile int leftPulseCount = 0;
unsigned long leftLastTime = 0;
volatile int rightPulseCount = 0;
unsigned long rightLastTime = 0;

void Sensors::updateHealth(){
  unsigned long currentTime = millis(); 
  if (currentTime - lastHealthUpdate > 2 * 1000) {
    lastHealthUpdate = currentTime;
    uint8_t bytesReceived = Wire.requestFrom(I2C_SLAVE_ADDR, 1);
    uint8_t byteIn = 0;

    if (bytesReceived > 0) {
      Serial.print("Received from slave: ");
      while (Wire.available()) {
        byteIn = Wire.read();
        Serial.printf("0x%02X ", byteIn);
      }
      Serial.println();
      health = byteIn;
    } 
  }
  health = 0;
}

uint32_t med3filt(uint32_t a, uint32_t b, uint32_t c) {
  uint32_t middle;
  if ((a <= b) && (a <= c))
    middle = (b <= c) ? b : c;  
  else if ((b <= a) && (b <= c))
    middle = (a <= c) ? a : c;
  else    middle = (a <= b) ? a : b;
  return middle;
}

void Sensors::leftUpdateEncoderA() {
  int stateA = digitalRead(leftEncodePinA);
  int stateB = digitalRead(leftEncodePinB);

  if (stateA != leftLastStateA) {  
    if (stateA == stateB) {
      leftPulseCount++;  
    } else {
      leftPulseCount--;  
    }
  }
  leftLastStateA = stateA;
}

void Sensors::leftUpdateEncoderB() {
  int stateA = digitalRead(leftEncodePinA);
  int stateB = digitalRead(leftEncodePinB);

  if (stateB != leftLastStateB) { 
    if (stateA == stateB) {
      leftPulseCount--; 
    } else {
      leftPulseCount++; 
    }
  }
  leftLastStateB = stateB;
}

void Sensors::rightUpdateEncoderA() {
  int stateA = digitalRead(rightEncodePinA);
  int stateB = digitalRead(rightEncodePinB);

  if (stateA != rightLastStateA) {  
    if (stateA == stateB) {
      rightPulseCount++;  
    } else {
      rightPulseCount--;  
    }
    // rightPulseCount += 2;
  }
  rightLastStateA = stateA;
}

void Sensors::rightUpdateEncoderB() {
  int stateA = digitalRead(rightEncodePinA);
  int stateB = digitalRead(rightEncodePinB);

  if (stateB != rightLastStateB) { 
    if (stateA == stateB) {
      rightPulseCount--; 
    } else {
      rightPulseCount++; 
    }
  }
  rightLastStateB = stateB;
}

void Sensors::updateLeftSpeed(){
  unsigned long currentTime = millis();
  if (currentTime - leftLastTime >= 10) {
    leftRPM = (leftPulseCount / (float)(pulsesPerRevolution * 4)) * (60000.0 / (currentTime - leftLastTime)); // RPM
    leftPulseCount = 0;
    leftLastTime = currentTime;
  }
}

void Sensors::updateRightSpeed(){
  unsigned long currentTime = millis(); 
  if (currentTime - rightLastTime >= 10) {
    rightRPM = (rightPulseCount / (float)(pulsesPerRevolution * 4)) * (60000.0 / (currentTime - rightLastTime)); // RPM
    rightPulseCount = 0;
    rightLastTime = currentTime;
  }
}

void Sensors::updateLocalization(){
  static uint16_t x,y;
  
  if (vive1->status() == VIVE_RECEIVING) {
    static uint16_t x0, y0, oldx1, oldx2, oldy1, oldy2;
    oldx2 = oldx1; oldy2 = oldy1;
    oldx1 = x0;     oldy1 = y0;
    
    x0 = vive1->xCoord();
    y0 = vive1->yCoord();
    x = med3filt(x0, oldx1, oldx2);
    y = med3filt(y0, oldy1, oldy2);
    if (x > 8000 || y > 8000 || x< 1000 || y < 1000) {
      x=0; y=0;
    }
  }
  else {
    x=0;
    y=0; 
    vive1->sync(5); 
  }
  x1 = x, y1 = y;

  if (vive2->status() == VIVE_RECEIVING) {
    static uint16_t x0, y0, oldx1, oldx2, oldy1, oldy2;
    oldx2 = oldx1; oldy2 = oldy1;
    oldx1 = x0;     oldy1 = y0;
    
    x0 = vive2->xCoord();
    y0 = vive2->yCoord();
    x = med3filt(x0, oldx1, oldx2);
    y = med3filt(y0, oldy1, oldy2);
    if (x > 8000 || y > 8000 || x< 1000 || y < 1000) {
      x=0; y=0;
    }
  }
  else {
    x=0;
    y=0; 
    vive2->sync(5); 
  }
  x2 = x, y2 = y;
  float dx = x2 - x1;
  float dy = y2 - y1;
  bearing = atan2(dy, dx);
  x = (x1 + x2) / 2;
  y = (y1 + y2) / 2;
  Serial.print("x: "); Serial.print(x); Serial.print(" y: "); Serial.print(y); Serial.print(" bearing: "); Serial.println(bearing);
}

// void Sensors::updateRightwardDistance(){
//   uint16_t distance = tofSensor.distance();
//   // Serial.print("Distance (mm): "); Serial.println(distance); DEBUG
//   tofSensor.clearInterrupt();
//   return distance;
// }

void Sensors::updateForwardDistance(){
  uint16_t newDist = tofSensor.distance();
  Serial.print("Distance (mm): "); Serial.println(distance); DEBUG
  tofSensor.clearInterrupt();
  forwardDistance = newDist;
}

void Sensors::updateState(){
  updateHealth();
  updateLeftSpeed();
  updateRightSpeed();
  updateLocalization();
  updateForwardDistance();
}

void Sensors::startup(){
  vive1 = new Vive510(SIGNALPIN2);
  vive2 = new Vive510(SIGNALPIN2);
  tofSensor = Adafruit_VL53L1X();
  pinMode(leftEncodePinA, INPUT_PULLUP);
  pinMode(leftEncodePinB, INPUT_PULLUP);
  pinMode(rightEncodePinA, INPUT_PULLUP);
  pinMode(rightEncodePinB, INPUT_PULLUP);
  vive1->begin();
  vive2->begin();
  Wire.begin(SDA_PIN, SCL_PIN, 40000);
  tofSensor.begin(I2C_TOF_ADDR, &Wire);
  tofSensor.startRanging();
  tofSensor.setTimingBudget(50);

  attachInterrupt(digitalPinToInterrupt(leftEncodePinA), leftUpdateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftEncodePinB), leftUpdateEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightEncodePinA), rightUpdateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightEncodePinB), rightUpdateEncoderB, CHANGE);
}

void plotData(int currentSpeed, int desiredSpeed, float error, float proportional, float integralTerm, float derivativeTerm, int pidOutput) {
  Serial.print(millis());
  Serial.print(" ");
  Serial.print(currentSpeed);
  Serial.print(" ");
  Serial.print(desiredSpeed);
  Serial.print(" ");
  Serial.print(error);
  Serial.print(" ");
  Serial.print(proportional);
  Serial.print(" ");
  Serial.print(integralTerm);
  Serial.print(" ");
  Serial.print(derivativeTerm);
  Serial.print(" ");
  Serial.println(pidOutput);
}