#include "robot.h"
#include <cmath>
#define PI 3.14159265
Robot robot;
void Robot::setLeftSpeed(int speed) {
  if (speed == 0) {
    ledcWrite(pwmPin1, 0);
    previousErrorLeft = 0;
    integralLeft = 0;
  } else {
    float dt = (millis() - lte) / 1000.0;
    int targetRPM = map(speed, 0, 100, 0, lRPMRange);
    float error = targetRPM - leftRPM;

    float proportional = lkp * error;

    integralLeft += error * dt;
    float integralTerm = lki * integralLeft;

    float derivative = (error - previousErrorLeft) / dt;
    float derivativeTerm = lkd * derivative;

    int pidOutput = proportional + integralTerm + derivativeTerm;
    pidOutput = constrain(pidOutput, 0, lRPMRange);
    // Serial.print("Left: ");
    plotData(leftRPM, targetRPM, error, proportional, integralTerm, derivativeTerm, pidOutput);

    previousErrorLeft = error;

    pidOutput = map(pidOutput, 0, lRPMRange, 0, MAX_PWM);

    ledcWrite(pwmPin1, pidOutput);
  }
  lte = millis();
}

void Robot::setRightSpeed(int speed) {
  if (speed == 0) {
    ledcWrite(pwmPin2, 0);
    previousErrorRight = 0;
    integralRight = 0;
  } else {
    float dt = (millis() - rte) / 1000.0;
    int targetRPM = map(speed, 0, 100, 0, rRPMRange);
    float error = targetRPM - rightRPM;

    float proportional = rkp * error;

    integralRight += error * dt;
    float integralTerm = rki * integralRight;

    float derivative = (error - previousErrorRight) / dt;
    float derivativeTerm = rkd * derivative;

    int pidOutput = proportional + integralTerm + derivativeTerm;
    pidOutput = constrain(pidOutput, 0, rRPMRange);

    // Serial.print("Right: ");
    // plotData(rightRPM, targetRPM, error, proportional, integralTerm, derivativeTerm, pidOutput);
    previousErrorRight = error;

    pidOutput = map(pidOutput, 0, rRPMRange, 0, MAX_PWM);

    ledcWrite(pwmPin2, pidOutput);
  }
  rte = millis();
}

void Robot::leftWheelForward(){
  digitalWrite(cPin1, HIGH);
  digitalWrite(cPin2, LOW);
  leftForward = true;
}

void Robot::leftWheelBackward(){
  digitalWrite(cPin1, LOW);
  digitalWrite(cPin2, HIGH);
  leftForward = false;
}

void Robot::rightWheelForward(){
  digitalWrite(cPin3, HIGH);
  digitalWrite(cPin4, LOW);
  rightForward = true;
}

void Robot::rightWheelBackward(){
  digitalWrite(cPin3, LOW);
  digitalWrite(cPin4, HIGH);
  rightForward = false;
}

void Robot::updateState(){
    sensors.updateState();
    health = sensors.health;
    leftRPM = sensors.leftRPM;
    rightRPM = sensors.rightRPM;
    if (!leftForward) {
        leftRPM = -1 * leftRPM;
    }
    if (!rightForward) {
        rightRPM = -1 * rightRPM;
    }
    if (health <= 0) state = 0;
    bearing = sensors.bearing;
    bearing_deg = bearing * 180 / PI;
    double rotated_cx = cx * cos(bearing) - cy * sin(bearing);
    double rotated_cy = cx * sin(bearing) + cy * cos(bearing);
    x = sensors.x + rotated_cx;
    y = sensors.y + rotated_cy;
}

void Robot::startup(){
  ledcAttach(pwmPin1, 30, resolution);
  ledcAttach(pwmPin2, 30, resolution);

  pinMode(cPin1, OUTPUT);
  pinMode(cPin2, OUTPUT);
  pinMode(cPin3, OUTPUT);
  pinMode(cPin4, OUTPUT);
  leftWheelForward();
  rightWheelForward();
  setLeftSpeed(0);
  setRightSpeed(0);
  sensors.startup();

  lte = millis();
  rte = lte;
}

void Robot::action(){
  switch (state) {
    case 0:
      //do nothing
      break;
    case 1:
      leftWheelForward();
      rightWheelForward();
      break;
    case 2:
      leftWheelBackward();
      rightWheelBackward();
      break;
    case 3:
      leftWheelBackward();
      rightWheelForward();
      break;
    case 4:
      leftWheelForward();
      rightWheelBackward();
      break;
    case 5:
      //TODO: navigate to inputted x, y position (wall following)
      //if nav_substate == 0, orient to target
      //if nav_substate == 1, 
      //  if we are oriented to target and something is in the way and not the target:
      //    turn left aggressively
      //  if we are not oriented to target:
      //    wall follow
      //  if we are oriented to target and nothing is in the way:
      //    move forward
      break;
    case 6:
      //TODO: attack nearest target
      break;
    case 7:
      //TODO: attack specified static target (nexus, tower, etc.)
      break;
    case 8:
      //TOOD: drive up ramp
      break;
    default:
      //Serial.printf("invalid state %d\n", state); 
      break;
  }
  if (state <= 4) {
    if (state == 0) {
      setLeftSpeed(0);
      setRightSpeed(0);
    } else {
      setLeftSpeed(userSpeed);
      setRightSpeed(userSpeed);
    }
  } else {
    setLeftSpeed(lSpeed);
    setRightSpeed(rSpeed);
  }
}