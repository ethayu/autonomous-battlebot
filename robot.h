#ifndef ROBOT_H
#define ROBOT_H
#include "config.h"
#include "point.h"
#include "sensors.h"
#include <set>

#define MAX_PWM 16382

#define pwmPin1 17//1//16 // PWM pin for left motor
#define pwmPin2 33//4//33

#define cPin1 10//8//10 // H bridge control pin 1 for left motor
#define cPin2 16//9//17 // H bridge cotronl pin 2 for left motor
#define cPin3 37//5//37
#define cPin4 34//6//34

#define servoPin1 -1
#define servoPin2 -1

#define resolution 14
#define rRPMRange 7090
#define lRPMRange 7090

#define lku 0.85
#define ltu 0.405
#define lti 0.5 * ltu
#define ltd 0.125 * ltu
#define lkp 0.6 * lku
#define lki lkp / lti
#define lkd lkp *ltd

#define rku 0.85
#define rtu 0.2
#define rti 0.5 * rtu
#define rtd 0.125 * rtu
#define rkp 0.6 * rku
#define rki rkp / rti
#define rkd rkp *rtd

#define cx 0
#define cy 0

class Robot
{
private:
    // PID variables for the left motor
    float previousErrorLeft = 0;
    float integralLeft = 0;

    // PID variables for the right motor
    float previousErrorRight = 0;
    float integralRight = 0;

    void setLeftSpeed(int speed);
    void setRightSpeed(int speed);
    void leftWheelForward();
    void leftWheelBackward();
    void rightWheelForward();
    void rightWheelBackward();
    unsigned long lte = 0;
    unsigned long rte = 0;
    float searchLimit = 0;
    float closestDistance = HUGE_VAL;

    int servoAngle = 0;
    bool servoDirection = true;

    // Autonomous
    void navTo();
    void orientTo(float bearing);
    void updateAutonState();
    void attackClosest();
    void attackStructure();

public:
    int state; // 0 = stop, 1 = forward, 2 = backward, 3 = left, 4 = right, 5 = navigate, 6 = attack, 7 = attack static target, 8 = navigate up ramp
    int leftRPM = 0;
    int rightRPM = 0;
    int lSpeed = 0;
    int rSpeed = 0;
    int userSpeed = 50;
    uint8_t health = 100;
    int forwardDistance = 0;
    uint16_t rightwardDistance = 0;
    void updateState();
    void startup();
    void action();
    void printState();
    bool leftForward = true;
    bool rightForward = true;
    float bearing;
    float bearing_deg;
    Point location;
    Point target;
    float target_bearing = PI / 2;
    Sensors sensors;
    bool attacking = false;
    
    int substate;     // FOR NAV TO: 0 = moving forward to target, 1 = turn to target, 2 = turn left to avoid obstacle (wall following), 3 = wall following, 4 = reached close, 5 = done
                      // FOR ATTACK CLOSEST: 0 = scanning, 1 = orienting, 2 = chasing/attacking
                      // FOR ATTACK STRUCTURE: 0-5 (navigating through navTo()), 6 = attacking

    // web
    std::set<int> activeKeys;
};
extern Robot robot;
#endif