#include <Arduino.h>
#include <Wire.h>
#include "kehopsboard.h"


Kehops::Kehops(int pin){
    Wire.begin();   // Initiate the Wire library for I2C
    pinMode(pin, OUTPUT);
    _pin = pin;
    device_pca9685 dev_pca9685;
    device_pca9629 dev_pca9629;
    device_ads111x dev_ads111x;
    device_mcp230xx dev_mcp23008;
    device_efm8McuKehops dev_efm8bb;
}

void Kehops::begin(){

// TODO:  Initialize I2C Port

    // PWM DRIVER PCA9685 INITIALISATION CONFIGURATION
    dev_pca9685.deviceAddress = 0x40;
    dev_pca9685.totemPoleOutput = 1;
    dev_pca9685.frequency = 100;
    dev_pca9685.invertedOutput = 0;
    pca9685_init(&dev_pca9685);


    dev_pca9629.deviceAddress = 0x20;
    dev_pca9629.pulsesWidth_ms = 1;
    pca9629_init(&dev_pca9629);

    dev_ads111x.deviceAddress = 0x49;
    ads111x_init(&dev_ads111x);        


    // Setting up the mcp23008GPIO extender              
    dev_mcp23008.deviceAddress = 0x21;
    dev_mcp23008.pullupEnable  = 0xff;
    dev_mcp23008.gpioDirection = 0x60;
    mcp23008_init(&dev_mcp23008);
    
    // Setting up the mcp23008GPIO extender
    dev_efm8bb.deviceAddress = 0x0a;
    EFM8BB_init(&dev_efm8bb);


    // Set motor H-Bridge ON
    actuator_setDigitalOutput(&dev_mcp23008, 4, 1);
}

void Kehops::setPWM(unsigned char pwmNb, unsigned char value){
    pca9685_setPWMdutyCycle(&dev_pca9685, pwmNb, value);
}


void  Kehops::stepperRotation(char speed, unsigned int steps){
    int direction;
    if(speed > 0)
        direction = 1;
    else 
        if(speed < 0){
            direction = -1;
            speed *= -1;
        }

    actuator_setStepperSpeed(&dev_pca9629, speed);
    actuator_setStepperStepAction(&dev_pca9629, direction, steps);
}


int  Kehops::getVoltage(unsigned char channel){
    int voltage_mV;
    voltage_mV = actuator_getVoltage(&dev_ads111x, channel);

    return voltage_mV;
}

int  Kehops::getDigitalInput(unsigned char channel){
    int state;
    state = actuator_getDigitalInput(&dev_mcp23008, channel);

    return state;
}

int  Kehops::getButton(unsigned char name){
    int state;
    unsigned char channel;

    if(name == LEFT)
        channel = 6;
    if(name == RIGHT)
        channel = 5;  
    state = actuator_getDigitalInput(&dev_mcp23008, channel);
    return state;
}

void Kehops::setLED(unsigned char name, unsigned char value){
    unsigned char channel;
    if(name == LED0)
        channel = 9;
    if(name == LED1)
        channel = 12;
    if(name == LED2)
        channel = 14; 

    pca9685_setPWMdutyCycle(&dev_pca9685, channel, value);
}


int  Kehops::setDigitalOutput(unsigned char channel, unsigned char value){
    int result = -1;
    actuator_setDigitalOutput(&dev_mcp23008, channel, value);

    return result;
}


int  Kehops::setMotorAction(unsigned char motorNumber, char speed){
    
    unsigned char directionChannel;
    unsigned char speedChannel;

    // Set Motor OFF and select appropriate direction channel
    switch(motorNumber){
        case 0 : speedChannel = 0;      
                 if(speed > 0)
                    directionChannel = 2;
                 else if(speed < 0)
                    directionChannel = 1;
                 // Set CW and CCW output to off (Hbridge OFF) 
                 actuator_setDigitalOutput(&dev_mcp23008, 1, 0);
                 actuator_setDigitalOutput(&dev_mcp23008, 2, 0);
                 break;

        case 1 : speedChannel = 1;
                 if(speed > 0)
                    directionChannel = 3;
                 else if(speed < 0)
                    directionChannel = 0;
                 // Set CW and CCW output to off (Hbridge OFF)
                 actuator_setDigitalOutput(&dev_mcp23008, 0, 0);
                 actuator_setDigitalOutput(&dev_mcp23008, 3, 0);
                 break;
    }

    // 50mS delay for prevent short-circuits on H-Bridge
    delay(50);

    // Set the new speed and activate the CW or CCW channel
    if(speed == 0){
        // Set PWM to 0 (motor speed = 0)
        pca9685_setPWMdutyCycle(&dev_pca9685, speedChannel, 0);
    }else{
        if(speed<0)
        speed *=-1;
        actuator_setDigitalOutput(&dev_mcp23008, directionChannel, 1);
        pca9685_setPWMdutyCycle(&dev_pca9685, speedChannel, speed);        
    }

    Serial.write("Speed: ");
    Serial.print(speed, DEC);
    Serial.write("    Direction: ");
    Serial.print(directionChannel, DEC);
    Serial.write("    CHannel: ");
    Serial.print(speedChannel, DEC);
    return 0;
}

int  Kehops::getStepperState(void){
    //int state = (actuator_getStepperState(&dev_pca9629) & 0x80) >> 8;
    int state = PCA9629_ReadMotorState(&dev_pca9629);
    return state;
}


int  Kehops::getSonarDistance(void){
    //int state = (actuator_getStepperState(&dev_pca9629) & 0x80) >> 8;
    // Get the channel 6 value (Sonar register)
    int distance = EFM8BB_getChannel(&dev_efm8bb, 6);
    return distance;
}

int  Kehops::getSensor(unsigned char number){
    //int state = (actuator_getStepperState(&dev_pca9629) & 0x80) >> 8;
    // Get the channel 6 value (Sonar register)
    unsigned char channel;

    if(number == 0)
        channel = 3;
    else
        channel = 2;

    int value = EFM8BB_getChannel(&dev_efm8bb, channel);
    return value;
}

