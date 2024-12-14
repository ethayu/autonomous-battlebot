#ifndef POINT_H
#define POINT_H
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
#include "config.h"
float euclideanDist(int x1, int y1, int x2, int y2);
float calcAngle(int x1, int y1, int x2, int y2);
bool similarAngle(float angle1, float angle2, float tolerance);
float modAngle(float angle);
bool hasReachedTarget(int currX, int currY, int targetX, int targetY);
#endif