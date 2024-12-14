#ifndef POINT_H
#define POINT_H
#include "config.h"

#define PI 3.14159265
#define distTolerance 50   // tolerance of target region


float euclideanDist(int x1, int y1, int x2, int y2);
float calcAngle(int x1, int y1, int x2, int y2);
bool similarAngle(float angle1, float angle2, float tolerance);
float modAngle(float angle);
bool hasReachedTarget(int currX, int currY, int targetX, int targetY);

class Point {
    public:
        float x;
        float y;

        Point(float x, float y);
        Point();

        float euclideanDistance(Point p);
        float calcAngle(Point p);
        bool hasReachedPoint(Point p);
        void setPoint(float x, float y);
        void setPoint(Point p);
        void print();
        
};


// Structures
const Point RED_NEXUS(-1, -1);
const Point BLUE_NEXUS(1450, -1);
const Point RED_TOWER(3500, 4300);
const Point BLUE_TOWER(3900, 4300);
const Point RED_UPPER_TOWER(3000, 2400);
const Point BLUE_UPPER_TOWER(3900, 4300);
#define LOOK_AT_RED_NEXUS PI;
#define LOOK_AT_BLUE_NEXUS 0;
#define LOOK_AT_TOP PI / 2;

#endif