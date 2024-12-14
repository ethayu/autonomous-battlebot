#ifndef SENSORS_H
#define SENSORS_H
#include "config.h"
#include "vive510.h"
#include "point.h"
#include <Wire.h>
#include "Adafruit_VL53L1X.h"
#include "SharpIR.h"

uint32_t med3filt(uint32_t a, uint32_t b, uint32_t c);

class Sensors
{
private:
    Vive510 *vive1;
    Vive510 *vive2;
    Adafruit_VL53L1X tofSensor = Adafruit_VL53L1X(TOF_XSHUT_PIN, IRQ_PIN);
    static void leftUpdateEncoderA();
    static void leftUpdateEncoderB();
    static void rightUpdateEncoderA();
    static void rightUpdateEncoderB();
    void updateHealth();
    void updateLeftSpeed();
    void updateRightSpeed();
    void updateLocalization();
    void updateForwardDistance();
    void updateRightwardDistance();
    Point loc1;
    Point loc2;

    unsigned long lastHealthUpdate = 0;

public:
    void updateState();
    void startup();
    int leftRPM = 0;
    int rightRPM = 0;
    int health = 100;
    float bearing;
    Point location;
    int forwardDistance = 0;
    uint16_t rightwardDistance = 0;
    bool usedWifi = false;
};

extern SharpIR psd_sensor;

void plotData(int currentSpeed, int desiredSpeed, float error, float proportional, float integralTerm, float derivativeTerm, int pidOutput);
#endif