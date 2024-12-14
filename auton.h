#ifndef AUTON_H
#define AUTON_H
#include "config.h"
#include "robot.h"

// helper functions
float euclideanDistance(int x1, int y1, int x2, int y2);
float calcAngle(int x1, int y1, int x2, int y2);
bool similarAngle(float angle1, float angle2, float tolerance);
float modAngle(float angle);
bool hasReachedTarget(int currX, int currY, int targetX, int targetY);
#endif