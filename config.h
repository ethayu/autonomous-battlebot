// Flags
#ifndef CONFIG_H
#define CONFIG_H
#include "point.h"
// uncomment/comment the following line to enable/disable the corresponding feature
// #define TOPHAT
#define VIVE
// #define PSD
// #define MOTORS
// #define TOF
// #define ATTACK
// #define DEBUG

// CONSTANTS
#define fidelity 0.05
#define PI 3.14159265
#define MAX_PWM 16382
#define PWM_RESOLUTION 14
#define rRPMRange 7090
#define lRPMRange 7090
#define pulsesPerRevolution 11

// pins
#define pwmPin1 17 // PWM pin for left motor
#define pwmPin2 33

#define cPin1 10 // H bridge control pin 1 for left motor
#define cPin2 16 // H bridge cotronl pin 2 for left motor
#define cPin3 37
#define cPin4 34

#define leftEncodePinA 13
#define leftEncodePinB 14
#define rightEncodePinA 2
#define rightEncodePinB 1

#define servoPin1 -1
#define servoPin2 -1

#define TOF_SDA_PIN 8
#define TOF_SCL_PIN 9
#define TOPHAT_SDA_PIN 35
#define TOPHAT_SCL_PIN 36
#define TOPHAT_FREQ 2
#define IRQ_PIN 4
#define TOPHAT_I2C_ADDR 0x28
#define TOF_I2C_ADDR 0x29
#define TOF_XSHUT_PIN 5
#define TOPHAT_XSHUT_PIN 4
#define SIGNALPIN1 4 // pin receiving signal from Vive circuit
#define SIGNALPIN2 2 // pin receiving signal from Vive circuit

#define PSD_PIN 11 // pin receiving signal from PSD sensor
#define PSD_model 1080

// pid
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

// autonomous
#define wallFTolerance 100 // forward tolerance
#define wallRTolerance 100 // right sensor tolerance
#define wallRLowerTol 50
#define bearingTolerance .2 // tolerance of bearing
#define aggresiveBearingConst PI * 5
#define passiveBearingConst PI * 1
#define wallFollowConst 0.1
#define wallFollowRightDist 20
#define distTolerance 50   // tolerance of target region
#define cx 0
#define cy 0

//structures
const Point RED_NEXUS(6100, 4350);
const Point BLUE_NEXUS(1500, 4350);
const Point RED_TOWER(3700, 4350);
const Point BLUE_TOWER(4000, 4350);
const Point RED_UPPER_TOWER(4200, 2400);
const Point BLUE_UPPER_TOWER(3200, 4300);
#define LOOK_AT_RED_NEXUS PI / 2;
#define LOOK_AT_BLUE_NEXUS -PI / 2;
#define LOOK_AT_TOP 0;
#endif