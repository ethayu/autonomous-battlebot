#include "robot.h"
#include <cmath>
#include "config.h"
#define PI 3.14159265
Robot robot;
void Robot::setLeftSpeed(int speed)
{
  lSpeed = speed;
  if (speed == 0)
  {
    ledcWrite(pwmPin1, 0);
    previousErrorLeft = 0;
    integralLeft = 0;
  }
  else
  {
    float dt = (millis() - lte) / 1000.0;
    int targetRPM = map(speed, -100, 100, -1 * lRPMRange, lRPMRange);
    float error = targetRPM - leftRPM;

    float proportional = lkp * error;

    integralLeft += error * dt;
    float integralTerm = lki * integralLeft;

    float derivative = (error - previousErrorLeft) / dt;
    float derivativeTerm = lkd * derivative;

    int pidOutput = proportional + integralTerm + derivativeTerm;
    #ifdef DEBUG
    pidOutput = constrain(pidOutput, -1 * lRPMRange, lRPMRange);
    Serial.print("Left: ");
    plotData(leftRPM, targetRPM, error, proportional, integralTerm, derivativeTerm, pidOutput);
    #endif

    previousErrorLeft = error;

    if (targetRPM < 0)
    {
      pidOutput *= -1;
      if (leftForward)
        leftWheelBackward();
    }
    else if (!leftForward)
      leftWheelForward();
    pidOutput = map(pidOutput, 0, lRPMRange, 0, MAX_PWM);

    ledcWrite(pwmPin1, pidOutput);
  }
  lte = millis();
}

void Robot::setRightSpeed(int speed)
{
  rSpeed = speed;
  if (speed == 0)
  {
    ledcWrite(pwmPin2, 0);
    previousErrorRight = 0;
    integralRight = 0;
  }
  else
  {
    float dt = (millis() - rte) / 1000.0;
    int targetRPM = map(speed, -100, 100, -1 * rRPMRange, rRPMRange);
    float error = targetRPM - rightRPM;

    float proportional = rkp * error;

    integralRight += error * dt;
    float integralTerm = rki * integralRight;

    float derivative = (error - previousErrorRight) / dt;
    float derivativeTerm = rkd * derivative;

    int pidOutput = proportional + integralTerm + derivativeTerm;
    pidOutput = constrain(pidOutput, -1 * rRPMRange, rRPMRange);

    #ifdef DEBUG
    Serial.print("Right: ");
    plotData(rightRPM, targetRPM, error, proportional, integralTerm, derivativeTerm, pidOutput);
    #endif
    previousErrorRight = error;

    if (targetRPM < 0)
    {
      pidOutput *= -1;
      if (rightForward)
        rightWheelBackward();
    }
    else if (!rightForward)
      rightWheelForward();
    pidOutput = map(pidOutput, 0, rRPMRange, 0, MAX_PWM);

    ledcWrite(pwmPin2, pidOutput);
  }
  rte = millis();
}

void Robot::leftWheelForward()
{
  digitalWrite(cPin1, HIGH);
  digitalWrite(cPin2, LOW);
  leftForward = true;
}

void Robot::leftWheelBackward()
{
  digitalWrite(cPin1, LOW);
  digitalWrite(cPin2, HIGH);
  leftForward = false;
}

void Robot::rightWheelForward()
{
  digitalWrite(cPin3, HIGH);
  digitalWrite(cPin4, LOW);
  rightForward = true;
}

void Robot::rightWheelBackward()
{
  digitalWrite(cPin3, LOW);
  digitalWrite(cPin4, HIGH);
  rightForward = false;
}

void Robot::updateState()
{
  sensors.updateState();
  health = sensors.health;
  leftRPM = sensors.leftRPM;
  rightRPM = sensors.rightRPM;
  if (!leftForward)
  {
    leftRPM = -1 * leftRPM;
  }
  if (!rightForward)
  {
    rightRPM = -1 * rightRPM;
  }
  if (health <= 0)
    state = 0;
  bearing = sensors.bearing;
  bearing_deg = bearing * 180 / PI;
  double rotated_cx = cx * cos(bearing) - cy * sin(bearing);
  double rotated_cy = cx * sin(bearing) + cy * cos(bearing);
  location.setPoint(sensors.location.x + rotated_cx, sensors.location.y + rotated_cy);
  forwardDistance = sensors.forwardDistance;
  rightwardDistance = sensors.rightwardDistance;
}

void Robot::startup()
{
  activeKeys = std::set<int>();
  #ifdef MOTORS
  ledcAttach(pwmPin1, 30, PWM_RESOLUTION);
  ledcAttach(pwmPin2, 30, PWM_RESOLUTION);
  pinMode(cPin1, OUTPUT);
  pinMode(cPin2, OUTPUT);
  pinMode(cPin3, OUTPUT);
  pinMode(cPin4, OUTPUT);
  leftWheelForward();
  rightWheelForward();
  setLeftSpeed(0);
  setRightSpeed(0);
  lte = millis();
  rte = lte;
  #endif
  #ifdef ATTACK
  attackingTime = millis();
  ledcAttach(servoPin1, 50, PWM_RESOLUTION);
  ledcAttach(servoPin2, 50, PWM_RESOLUTION);
  #endif

  sensors.startup();
}

void Robot::action()
{
  if (state == 0) {
    setLeftSpeed(0);
    setRightSpeed(0);
  } else if (state <= 1)
  {
    #ifdef DEBUG
    Serial.print("Set contents before action: ");
    for (const int &element : activeKeys) {
        Serial.print(element);
        Serial.print(" "); // Space between elements
    }
    Serial.println(); // Newline at the end
    #endif
    if (!activeKeys.empty())
    {
      float leftSpeed = 0;
      float rightSpeed = 0;
      if (activeKeys.contains(1))
      {
        leftSpeed += 0.75;
        rightSpeed += 0.75;
      }
      if (activeKeys.contains(2))
      {
        leftSpeed -= 0.75;
        rightSpeed -= 0.75;
      }
      if (activeKeys.contains(3))
      {
        if (!activeKeys.contains(2) && (!activeKeys.contains(4) || activeKeys.contains(1)))
        {
          leftSpeed -= 0.5;
          rightSpeed += 0.5;
        }
        else if (activeKeys.contains(2) && !activeKeys.contains(1))
        {
          leftSpeed += 0.5;
          rightSpeed -= 0.5;
        }
      }
      if (activeKeys.contains(4))
      {
        if (!activeKeys.contains(2) && (!activeKeys.contains(3) || activeKeys.contains(1)))
        {
          leftSpeed += 0.5;
          rightSpeed -= 0.5;
        }
        else if (activeKeys.contains(2) && !activeKeys.contains(1))
        {
          leftSpeed -= 0.5;
          rightSpeed += 0.5;
        }
      }
      float mag = std::max(abs(leftSpeed), abs(rightSpeed));
      if (mag == 0)
      {
        setLeftSpeed(0);
        setRightSpeed(0);
      }
      else
      {
        if (activeKeys.size() == 1 && (activeKeys.contains(3) || activeKeys.contains(4))) mag *= 2;
        setLeftSpeed(userSpeed * leftSpeed / mag);
        setRightSpeed(userSpeed * rightSpeed / mag);
      }
    }
    else
    {
      state = 0; // sanity set
      setLeftSpeed(0);
      setRightSpeed(0);
    }
  }
  else if (state > 1)
  {
    updateAutonState();
    switch (state)
    {
    case 2: // navTo() state
    {
      navTo();
      break;
    }
    case 3:
    {
      attackClosest();
      break;
    }
    case 4:
    {
      attackStructure();
      break;
    }
    case 5:{
      // TOOD: drive up ramp
      break;
    }
    default:
      Serial.printf("invalid state %d\n", state);
      break;
    }
    // autonomous mode
    setLeftSpeed(lSpeed);
    setRightSpeed(rSpeed);
  }
  if (attacking && health > 0)
  {
    if (millis() - attackingTime < 1000) {
      servoAngle = 180;
    } else if (millis() - attackingTime < 2000) {
      servoAngle = 0;
    } else {
      attackingTime = millis();
      servoAngle = 180;
    }

    ledcWrite(servoPin1, map(servoAngle, 0, 180, .025 * MAX_PWM, .2 * MAX_PWM)); //may be unexpected behavior due to PWM changing every 1ms
    // ledcWrite(servoPin2, map(servoAngle, 0, 180, .025 * MAX_PWM, .2 * MAX_PWM));
  }
}

void Robot::printState()
{
  #ifdef DEBUG 
  Serial.print("PSD ADC Output: ");
  Serial.println(analogRead(PSD_PIN));
  #endif
  Serial.print("State: ");
  Serial.print(state);
  Serial.print(" Health: ");
  Serial.print(health);
  Serial.print(" Left RPM: ");
  Serial.print(leftRPM);
  Serial.print(" Right RPM: ");
  Serial.print(rightRPM);
  Serial.print(" Left Speed: ");
  Serial.print(lSpeed);
  Serial.print(" Right Speed: ");
  Serial.print(rSpeed);
  Serial.print(" Bearing: ");
  Serial.print(bearing_deg);
  Serial.print(" x: ");
  Serial.print(location.x);
  Serial.print(" y: ");
  Serial.print(location.y);
  Serial.print(" target x: ");
  Serial.print(target.x);
  Serial.print(" target y: ");
  Serial.print(target.y);
  Serial.print(" Forward Distance: ");
  Serial.print(forwardDistance);
  Serial.print(" Rightward Distance: ");
  Serial.println(rightwardDistance);
}