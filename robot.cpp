#include "robot.h"
#include <cmath>
#define PI 3.14159265
Robot robot;
void Robot::setLeftSpeed(int speed) {
  if (speed < 0) {
    speed *= -1;
    if (leftForward) leftWheelBackward();
  } else {
    if (!leftForward) {
      leftWheelForward();
      leftRPM = -1 * leftRPM;
    }
  }
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
  if (speed < 0) {
    speed *= -1;
    if (rightForward) rightWheelBackward();
  } else {
    if (!rightForward) {
      rightWheelForward();
      rightRPM = -1 * rightRPM;
    }
  }
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

//AUTONOMOUS FUNCTIONS

int distTolerance = 50; //tolerance of target region
int wallFTolerance = 100; //forward tolerance
int wallRTolerance = 100; //right sensor tolerance
int wallRLowerTol = 50;
float bearingTolerance = .2; //tolerance of bearing

//math helper functions
int euclideanDist(x1, y1, x2, y2) {
  int dist = sqrt(sq(x2 - x1) + sq(y2 - y1));
  return dist;
}

float calcAngle(int x1, int y1, int x2, int y2) {
  float angle = atan2((y2 - y1), (x2 - x1));
  if (angle < 0) {
    angle = 2*PI + angle;
  }
  return angle;
}

boolean similarAngle(float angle1, float angle2, float tolerance) {
  double diff = ( angle2 - angle1 + 180 ) % 360 - 180;
  return (diff < -180 ? diff + 360 : diff) < tolerance;
}

boolean similarDist(dist1, dist2, tolerance) {
  return abs(dist1 - dist2) < tolerance;
}

boolean hasReachedTarget(int targetX, int targetY) {
  return euclideanDist(currX, currY, targetX, targetY) < distTolerance;
}

void turnToBearing(float targetBearing) {
  if (similarAngle(currOrient, targetBearing, bearingTolerance)) {
    moveForward();
  } else {
    if (currOrient < targetBearing) {
      if (targetBearing - currOrient < PI) {
        turnRight();
      } else {
        turnLeft();
      }
    } else {
      if (currOrient - targetBearing < PI) {
        turnLeft();
      } else {
        turnRight();
      }
    }
  }
}

//Wallfollowing Motor Controls
int pathFindSpeed = 20;
void moveForward() {
  setLeftSpeed(pathFindSpeed);
  setRightSpeed(pathFindSpeed);
}

void turnLeft() {
  setRightSpeed(pathFindSpeed);
  setLeftSpeed(-pathFindSpeed);
}

void turnRight() {
  setRightSpeed(-pathFindSpeed);
  setLeftSpeed(pathFindSpeed);
}

boolean prepareForWallFollow = false;

//Pathfinding behaviors
void straightFind(targetX, targetY) {
  if (similarAngle(currOrient, calcAngle(currX, currY, targetX, targetY, bearingTolerance) || prepareForWallFollow) {) {
    if(forwardDistance > wallFTolerance && !prepareForWallFollow) {
      moveForward(); 
    } else if (rightwardDistance > wallFTolerance) { //we want right dist to be equal to forward gap
      turnLeft(); // turn left to avoid obstacle
      prepareForWallFollow = true;
    }
    else {
      pathFindState = 2; // obstacle ahead, switch to wall follow
    }
  } else {
    turnToBearing(calcAngle(currX, currY, targetX, targetY)); // turn to target bearing to start bearing
  }
}

void wallFollow(targetX, targetY) {
  if(!similarAngle(currOrient, calcAngle(currX, currY, targetX, targetY))) {
    if (rightDist() < wallRLowerTolerance) {
      turnLeft();
    } else if (rightDist() > wallRTolerance) {
      turnRight();
    } else {
      moveForward();
    }
  } else {
    pathFindState = 1;
  }
}

int pathFindState = 1;//needs to be initiliazed at start.

void Robot::pathFind(int targetX, int targetY, float targetBearing){
  int currX = robot.x;
  int currY = robot.y;
  int currOrient = robot.bearing;
  if (!hasReachedTarget(targetX, targetY)) {
    if (pathFindState == 1) {
      straightFind(targetX, targetY);
    } else if (pathFindState == 2) {
      wallFollow(targetX, targetY);
    } else {
      Serial.println("Invalid pathfind state"); // debug
    }
  } else() {
    pathFindState = 0;
    robot.state = 0;//pathfind complete
  }
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
    forwardDistance = sensors.forwardDistance;
    rightwardDistance = sensors.rightwardDistance;
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
      setLeftSpeed(0);
      setRightSpeed(0);
      break;
    case 1:
      setLeftSpeed(userSpeed);
      setRightSpeed(userSpeed);
      break;
    case 2:
      setLeftSpeed(-1 * userSpeed);
      setRightSpeed(-1 * userSpeed);
      break;
    case 3:
      setLeftSpeed(-1 * userSpeed);
      setRightSpeed(userSpeed);
      break;
    case 4:
      setLeftSpeed(userSpeed);
      setRightSpeed(-1 * userSpeed);
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
      pathFindState = 1;
      int targetX = 4000;
      int targetY = 4000;
      float targetBearing = 0;
      pathFind(targetX, targetY, targetBearing);
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
  if (state > 4) { // autonomous mode
    setLeftSpeed(lSpeed);
    setRightSpeed(rSpeed);
  }
}

void Robot::printState() {
  Serial.print("State: ");
  Serial.print(state);
  Serial.print(" Health: ");
  Serial.print(health);
  Serial.print(" Left RPM: ");
  Serial.print(leftRPM);
  Serial.print(" Right RPM: ");
  Serial.print(rightRPM);
  Serial.print(" Bearing: ");
  Serial.print(bearing_deg);
  Serial.print(" x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.print(y);
  Serial.print(" Forward Distance: ");
  Serial.print(forwardDistance);
  Serial.print(" Rightward Distance: ");
  Serial.println(rightwardDistance);
}