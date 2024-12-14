#include "sensors.h"
#include <math.h>
#include <SharpIR.h>

SharpIR psd_sensor(PSD_PIN, PSD_model);

int leftLastStateA = LOW;
int leftLastStateB = LOW;
int rightLastStateA = LOW;
int rightLastStateB = LOW;

volatile int leftPulseCount = 0;
unsigned long leftLastTime = 0;
volatile int rightPulseCount = 0;
unsigned long rightLastTime = 0;

void Sensors::updateHealth()
{
  unsigned long currentTime = millis();
  if (currentTime - lastHealthUpdate > 5 * 100 || true)
  {
    if (usedWifi) {
      Wire1.beginTransmission(TOPHAT_I2C_ADDR); 
      Wire1.write(1);                      
      Wire1.endTransmission();
      usedWifi = false;
    }
    Wire1.requestFrom(TOPHAT_I2C_ADDR, 1);
    if (Wire1.available())
    {
      uint8_t byte1 = Wire1.read();
      Serial.printf("Recieved data: %d\n", byte1);
      Serial.println(byte1);
      health = byte1;
    }
    lastHealthUpdate = currentTime;
  }
}

uint32_t med3filt(uint32_t a, uint32_t b, uint32_t c)
{
  uint32_t middle;
  if ((a <= b) && (a <= c))
    middle = (b <= c) ? b : c;
  else if ((b <= a) && (b <= c))
    middle = (a <= c) ? a : c;
  else
    middle = (a <= b) ? a : b;
  return middle;
}

void Sensors::leftUpdateEncoderA()
{
  int stateA = digitalRead(leftEncodePinA);
  int stateB = digitalRead(leftEncodePinB);

  if (stateA != leftLastStateA)
  {
    if (stateA == stateB)
    {
      leftPulseCount++;
    }
    else
    {
      leftPulseCount--;
    }
  }
  leftLastStateA = stateA;
}

void Sensors::leftUpdateEncoderB()
{
  int stateA = digitalRead(leftEncodePinA);
  int stateB = digitalRead(leftEncodePinB);

  if (stateB != leftLastStateB)
  {
    if (stateA == stateB)
    {
      leftPulseCount--;
    }
    else
    {
      leftPulseCount++;
    }
  }
  leftLastStateB = stateB;
}

void Sensors::rightUpdateEncoderA()
{
  int stateA = digitalRead(rightEncodePinA);
  int stateB = digitalRead(rightEncodePinB);

  if (stateA != rightLastStateA)
  {
    if (stateA == stateB)
    {
      rightPulseCount++;
    }
    else
    {
      rightPulseCount--;
    }
    // rightPulseCount += 2;
  }
  rightLastStateA = stateA;
}

void Sensors::rightUpdateEncoderB()
{
  int stateA = digitalRead(rightEncodePinA);
  int stateB = digitalRead(rightEncodePinB);

  if (stateB != rightLastStateB)
  {
    if (stateA == stateB)
    {
      rightPulseCount--;
    }
    else
    {
      rightPulseCount++;
    }
  }
  rightLastStateB = stateB;
}

void Sensors::updateLeftSpeed()
{
  unsigned long currentTime = millis();
  if (currentTime - leftLastTime >= 10)
  {
    leftRPM = (leftPulseCount / (float)(pulsesPerRevolution * 4)) * (60000.0 / (currentTime - leftLastTime)); // RPM
    leftPulseCount = 0;
    leftLastTime = currentTime;
  }
}

void Sensors::updateRightSpeed()
{
  unsigned long currentTime = millis();
  if (currentTime - rightLastTime >= 10)
  {
    rightRPM = (rightPulseCount / (float)(pulsesPerRevolution * 4)) * (60000.0 / (currentTime - rightLastTime)); // RPM
    rightPulseCount = 0;
    rightLastTime = currentTime;
  }
}

void Sensors::updateLocalization()
{
  static uint16_t l_x, l_y;

  if (vive1->status() == VIVE_RECEIVING)
  {
    static uint16_t x0, y0, oldx1, oldx2, oldy1, oldy2;
    oldx2 = oldx1;
    oldy2 = oldy1;
    oldx1 = x0;
    oldy1 = y0;

    x0 = vive1->xCoord();
    y0 = vive1->yCoord();
    l_x = med3filt(x0, oldx1, oldx2);
    l_y = med3filt(y0, oldy1, oldy2);
    if (l_x > 8000 || l_y > 8000 || l_x < 1000 || l_y < 1000)
    {
      l_x = 0;
      l_y = 0;
    }
  }
  else
  {
    l_x = 0;
    l_y = 0;
    vive1->sync(5);
  }
  if (l_x != 0 && l_y != 0) {
    loc1.setPoint(l_x, l_y);
  }

  if (vive2->status() == VIVE_RECEIVING)
  {
    static uint16_t x0, y0, oldx1, oldx2, oldy1, oldy2;
    oldx2 = oldx1;
    oldy2 = oldy1;
    oldx1 = x0;
    oldy1 = y0;

    x0 = vive2->xCoord();
    y0 = vive2->yCoord();
    l_x = med3filt(x0, oldx1, oldx2);
    l_y = med3filt(y0, oldy1, oldy2);
    if (l_x > 8000 || l_y > 8000 || l_x < 1000 || l_y < 1000)
    {
      l_x = 0;
      l_y = 0;
    }
  }
  else
  {
    l_x = 0;
    l_y = 0;
    vive2->sync(5);
  }
  if (l_x != 0 && l_y != 0) {
    loc2.setPoint(l_x, l_y);
  }
  #ifdef DEBUG
  loc1.print();
  loc2.print();
  #endif
  float dx = loc2.x - loc1.x;
  float dy = loc2.y - loc1.y;
  bearing = atan2(dy, dx);

  location.setPoint((loc1.x + loc2.x) / 2.0, (loc1.y + loc2.y) / 2.0);
}

void Sensors::updateRightwardDistance()
{
  rightwardDistance = psd_sensor.distance();
}

void Sensors::updateForwardDistance()
{
  if (tofSensor.dataReady())
  {
    int newDist = tofSensor.distance();
    if (newDist == -1)
    {
      return;
    }
    forwardDistance = newDist;
    tofSensor.clearInterrupt();
  }
}

void Sensors::updateState()
{
  #ifdef TOPHAT
  updateHealth();
  #endif
  #ifdef MOTORS
  updateLeftSpeed();
  updateRightSpeed();
  #endif
  #ifdef VIVE
  updateLocalization();
  #endif
  #ifdef TOF
  updateForwardDistance();
  #endif
  #ifdef PSD
  updateRightwardDistance();
  #endif
}

void Sensors::startup()
{
  #ifdef TOPHAT
  Wire1.begin(35, 36, 2);
  pinMode(TOPHAT_XSHUT_PIN, OUTPUT);
  digitalWrite(TOPHAT_XSHUT_PIN, LOW);
  #endif
  #ifdef VIVE
  vive1 = new Vive510(SIGNALPIN1);
  vive2 = new Vive510(SIGNALPIN2);
  vive1->begin();
  vive2->begin();
  #endif
  #ifdef MOTORS
  pinMode(leftEncodePinA, INPUT_PULLUP);
  pinMode(leftEncodePinB, INPUT_PULLUP);
  pinMode(rightEncodePinA, INPUT_PULLUP);
  pinMode(rightEncodePinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(leftEncodePinA), leftUpdateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(leftEncodePinB), leftUpdateEncoderB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightEncodePinA), rightUpdateEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightEncodePinB), rightUpdateEncoderB, CHANGE);
  #endif
  #ifdef TOF
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!tofSensor.begin(TOF_I2C_ADDR, &Wire))
  {
    Serial.print(F("Error on init of VL sensor: "));
    Serial.println(tofSensor.vl_status);
    while (1)
      delay(10);
  }
  Serial.println(F("VL53L1X sensor OK!"));
  Serial.print(F("Sensor ID: 0x"));
  Serial.println(tofSensor.sensorID(), HEX);

  if (!tofSensor.startRanging())
  {
    Serial.print(F("Couldn't start ranging: "));
    Serial.println(tofSensor.vl_status);
    while (1)
      delay(10);
  }
  Serial.println(F("Ranging started"));

  tofSensor.setTimingBudget(15);
  Serial.print(F("Timing budget (ms): "));
  Serial.println(tofSensor.getTimingBudget());
  #endif
}

void plotData(int currentSpeed, int desiredSpeed, float error, float proportional, float integralTerm, float derivativeTerm, int pidOutput)
{
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