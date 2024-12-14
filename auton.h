#ifndef AUTON_H
#define AUTON_H
#include "config.h"
#include "robot.h"
#define wallFTolerance 100 // forward tolerance
#define wallRTolerance 100 // right sensor tolerance
#define wallRLowerTol 50
#define bearingTolerance .2 // tolerance of bearing
#define aggresiveBearingConst PI * 5
#define passiveBearingConst PI * 1
#define wallFollowConst 0.1
#define wallFollowRightDist 20

// helper functions
float euclideanDistance(int x1, int y1, int x2, int y2);
float calcAngle(int x1, int y1, int x2, int y2);
bool similarAngle(float angle1, float angle2, float tolerance);
float modAngle(float angle);
bool hasReachedTarget(int currX, int currY, int targetX, int targetY);
#endif