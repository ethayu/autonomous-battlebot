#ifndef SENSORS_H
#define SENSORS_H
#include "vive510.h"
#include <Wire.h>
#include "Adafruit_VL53L1X.h"
#include "SharpIR.h"

#define leftEncodePinA 13//19
#define leftEncodePinB 14//18
#define rightEncodePinA 2//7//1
#define rightEncodePinB 1//0//2

#define pulsesPerRevolution 11

#define SDA_PIN 8
#define SCL_PIN 9
#define IRQ_PIN 4
#define TOPHAT_I2C_ADDR 0x28
#define TOF_I2C_ADDR 0x29
#define TOF_XSHUT_PIN 5
#define TOPHAT_XSHUT_PIN 4
#define SIGNALPIN1 4 // pin receiving signal from Vive circuit
#define SIGNALPIN2 2 // pin receiving signal from Vive circuit

#define PSD_PIN 11 // pin receiving signal from PSD sensor
#define PSD_model 1080

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
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;

    unsigned long lastHealthUpdate = 0;

public:
    void updateState();
    void startup();
    int leftRPM = 0;
    int rightRPM = 0;
    int health = 100;
    float bearing;
    float x = 0;
    float y = 0;
    int forwardDistance = 0;
    uint16_t rightwardDistance = 0;
    bool usedWifi = false;
};

extern SharpIR psd_sensor;

void plotData(int currentSpeed, int desiredSpeed, float error, float proportional, float integralTerm, float derivativeTerm, int pidOutput);
#endif