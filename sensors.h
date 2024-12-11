#ifndef SENSORS_H
#define SENSORS_H
#include "vive510.h"
#include <Wire.h>

#define leftEncodePinA 18
#define leftEncodePinB 19
#define rightEncodePinA 4
#define rightEncodePinB 10

#define pulsesPerRevolution 11

#define SDA_PIN 20
#define SCL_PIN 20
#define I2C_SLAVE_ADDR 0x28
#define SIGNALPIN1 34 // pin receiving signal from Vive circuit
#define SIGNALPIN2 35 // pin receiving signal from Vive circuit

uint32_t med3filt(uint32_t a, uint32_t b, uint32_t c);

class Sensors {
    private:
        Vive510* vive1;
        Vive510* vive2;
        static void leftUpdateEncoderA();
        static void leftUpdateEncoderB();
        static void rightUpdateEncoderA();
        static void rightUpdateEncoderB();
        void updateHealth();
        void updateLeftSpeed();
        void updateRightSpeed();
        void updateLocalization();
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
};

void plotData(int currentSpeed, int desiredSpeed, float error, float proportional, float integralTerm, float derivativeTerm, int pidOutput);
#endif