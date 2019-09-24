#ifndef kehopsboard_h
#define kehopsboard_h

#define LED0 9
#define LED1 12
#define LED2 14

#define PWM0 2
#define PWM1 3
#define PWM2 5
#define PWM3 6
#define PWM4 15
#define PWM5 8
#define PWM6 7
#define PWM7 13
#define PWM8 10
#define PWM9 11

#define LEFT 0
#define RIGHT 1

#include <Arduino.h>

// Low level device hardware library
#include "device_drivers/pca9685.h"
#include "device_drivers/pca9629.h"
#include "device_drivers/ads111x.h" 
#include "device_drivers/mcp230xx.h"
#include "device_drivers/efm8_mcu_kehops.h"

class Kehops{
    public:
        Kehops(int pin); 
        void begin();
        void setPWM(unsigned char pwmNb, unsigned char ratio);
        void stepperRotation(char speed, unsigned int steps);
        int getVoltage(unsigned char channel);
        int getDigitalInput(unsigned char channel);
        int setDigitalOutput(unsigned char channel, unsigned char value);
        int setMotorAction(unsigned char motorNumber, char speed);
        int getStepperState(void);
        int getSonarDistance(void);
        int getSensor(unsigned char number);

        int getButton(unsigned char name);
        void setLED(unsigned char pwmNb, unsigned char ratio);

    private:
        int _pin;
        device_pca9685 dev_pca9685;
        device_pca9629 dev_pca9629;
        device_ads111x dev_ads111x;
        device_mcp230xx dev_mcp23008;
        device_efm8McuKehops dev_efm8bb;
};
    
#endif