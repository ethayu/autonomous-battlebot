#include "point.h"
#include "Arduino.h"
#include "config.h"
#include <cmath>

float euclideanDist(int x1, int y1, int x2, int y2)
{
  float dist = hypot(x2 - x1, y2 - y1);
  return dist;
}

float calcAngle(int x1, int y1, int x2, int y2)
{
  float angle = atan2(y2 - y1, x2 - x1);
  return angle;
}

bool similarAngle(float angle1, float angle2, float tolerance)
{
  angle1 = modAngle(angle1);
  angle2 = modAngle(angle2 + 2 * PI);

  float diff = fabs(angle1 - angle2);

  diff = fmin(diff, 2 * PI - diff);

  return diff <= tolerance;
}

float modAngle(float angle)
{
  return fmod(angle + 3 * PI, 2 * PI) - PI;
}

bool hasReachedTarget(int currX, int currY, int targetX, int targetY)
{
  return euclideanDist(currX, currY, targetX, targetY) < distTolerance;
}

Point::Point(float x, float y) : x(x), y(y) {}
Point::Point(): x(1400), y(2400) {}

float Point::euclideanDistance(Point p)
{
  float dist = hypot(p.x - x, p.y - y);
  return dist;
}

float Point::calcAngle(Point p)
{
  float angle = modAngle(atan2(p.y - y, p.x - x));
  return angle;
}

bool Point::hasReachedPoint(Point p)
{
  return euclideanDistance(p) < distTolerance;
}

void Point::setPoint(float x, float y) 
{
  this->x = x;
  this->y = y;
}

void Point::setPoint(Point p) 
{
  x = p.x;
  y = p.y;
}

void Point::print() {
  Serial.print("x: ");
  Serial.print(x);
  Serial.print(" y: ");
  Serial.println(y);
}