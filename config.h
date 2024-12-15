// Flags
#ifndef CONFIG_H
#define CONFIG_H
#include <Wire.h>
#include "point.h"
extern TwoWire I2Ctof;
extern TwoWire I2Ctophat;

// uncomment/comment the following line to enable/disable the corresponding feature
#define TOPHAT
#define VIVE
#define PSD
#define MOTORS
#define TOF
#define ATTACK
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
#define pwmPin1 33 // PWM pin for left motor
#define pwmPin2 17

#define cPin1 37 // H bridge control pin 1 for left motor
#define cPin2 34 // H bridge control pin 2 for left motor
#define cPin3 10
#define cPin4 16

#define leftEncodePinA 1
#define leftEncodePinB 41
#define rightEncodePinA 5
#define rightEncodePinB 6

#define servoPin1 3
#define servoPin2 -1

#define TOF_SDA_PIN 36//22
#define TOF_SCL_PIN 35//23
#define TOPHAT_SDA_PIN 9//16
#define TOPHAT_SCL_PIN 8//17
#define TOPHAT_FREQ 2
#define IRQ_PIN -1
#define TOPHAT_I2C_ADDR 0x28
#define TOF_I2C_ADDR 0x29
#define TOF_XSHUT_PIN 21
#define TOPHAT_XSHUT_PIN -1
#define VIVE1_PIN 12 // pin receiving signal from Vive circuit
#define VIVE2_PIN 13 // pin receiving signal from Vive circuit

#define PSD_PIN 2 // pin receiving signal from PSD sensor
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
#define wallRTolerance 75 // right sensor tolerance
#define wallRLowerTol 50
#define bearingTolerance .5 // tolerance of bearing
#define aggresiveBearingConst 10 / PI
#define passiveBearingConst 15 / PI
#define wallFollowConst 0.1
#define wallFollowRightDist 100 
#define distTolerance 100   // tolerance of target region
#define cx 0
#define cy 0

//structures
const Point RED_NEXUS(6100, 4350);
const Point BLUE_NEXUS(1500, 4350);
const Point RED_TOWER(3700, 4350);
const Point BLUE_TOWER(4000, 4350);
const Point RED_UPPER_TOWER(4200, 2400);
const Point BLUE_UPPER_TOWER(3200, 4300);
#define LOOK_AT_RED_NEXUS PI / 2
#define LOOK_AT_BLUE_NEXUS -PI / 2
#define LOOK_AT_TOP 0

#endif