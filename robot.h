#ifndef ROBOT_H
#define ROBOT_H
#include "sensors.h"

#define MAX_PWM 16382

#define pwmPin1 1 // PWM pin for left motor
#define pwmPin2 8

#define cPin1 7 // H bridge control pin 1 for left motor
#define cPin2 9 // H bridge cotronl pin 2 for left motor
#define cPin3 6
#define cPin4 5

#define resolution 14
#define rRPMRange (state == 2 || state == 3) ? 7554 : 7090
#define lRPMRange (state == 2 || state == 3) ? 7200 : 7090

#define lku 0.85
#define ltu 0.405
#define lti 0.5 * ltu
#define ltd 0.125 * ltu
#define lkp 0.6 * lku
#define lki lkp / lti
#define lkd lkp * ltd

#define rku 0.85
#define rtu 0.2 
#define rti 0.5 * rtu
#define rtd 0.125 * rtu
#define rkp 0.6 * rku
#define rki rkp / rti
#define rkd rkp * rtd

#define cx 0
#define cy 0

class Robot {
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
        int target_x = 1000;
        int target_y = 1000;
        float rotate_to_bearing = 0;
        
        Sensors sensors;

        int substate; // 0 = goto target, 1 = goto bearing, 2 = wall follow
        int nav_substate; // 0 = initial orient to target; 1 = moving to target 
    public:
        int state; // 0 = stop, 1 = forward, 2 = backward, 3 = left, 4 = right, 5 = navigate, 6 = attack, 7 = attack static target, 8 = navigate up ramp
        int leftRPM = 0;
        int rightRPM = 0;
        int lSpeed = 0;
        int rSpeed = 0;
        int userSpeed = 0;
        int health = 100;
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
        float x = 0;
        float y = 0;

};
extern Robot robot;
#endif