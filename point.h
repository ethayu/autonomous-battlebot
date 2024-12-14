#ifndef POINT_H
#define POINT_H

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
        float euclideanDistance(Point p);
        float calcAngle(Point p);
        bool hasReachedPoint(Point p);
        void setPoint(float x, float y);
        void print();
};
#endif