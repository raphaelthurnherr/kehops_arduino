/**
 * @file arduino-i2c.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2019-09-16
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "arduino-i2c.h"
#include <Arduino.h>
#include <Wire.h>

int i2c_write(unsigned char bus, unsigned char devAddr, unsigned char reg, unsigned char data){
    Wire.beginTransmission(devAddr); // Select device address
    Wire.write(reg);                 // Select register data
    Wire.write(data);                // sends data
    Wire.endTransmission();          // stop transmitting

    return 0;
}

int i2c_writeBuffer(unsigned char bus, unsigned char devAddr, unsigned char reg, unsigned char * data, unsigned char count){
    Wire.beginTransmission(devAddr); // Select device address
    Wire.write(reg);                 // Select register data
    Wire.write(data, count);
    Wire.endTransmission();          // stop transmitting

    return 0;
}

int i2c_readByte(unsigned char bus, unsigned char devAddr, unsigned char reg, unsigned char * ptrDest){

    Wire.beginTransmission(devAddr);    // Get the slave's attention, tell it we're sending a command byte
    Wire.write(reg);                   //  The command byte, sets pointer to register with address of 0x32
    Wire.endTransmission();             // "Hang up the line" so others can use it (can h

    Wire.requestFrom(devAddr, 1);        // Tell slave we need to read 1byte from the current register
    while(Wire.available())    // slave may send less than requested
    {
        ptrDest[0] = Wire.read();      // read that byte into 'slaveByte2' variable
    }
    Wire.endTransmission();             // "Hang up the line" so others can use it (can h

    return 0;
}

int i2c_read(unsigned char bus, unsigned char devAddr, unsigned char reg, unsigned char * data, unsigned char count){
    unsigned char i=0;

    Wire.beginTransmission(devAddr);    // Get the slave's attention, tell it we're sending a command byte
    Wire.write(reg);                   //  The command byte, sets pointer to register with address of 0x32
    Wire.endTransmission();             // "Hang up the line" so others can use it (can h

    Wire.requestFrom(devAddr, count);    // request n bytes from slave device #devAddr
    while(Wire.available())    // slave may send less than requested
    {
        data[i++]= Wire.read();    // receive a byte as character
    }

    Wire.endTransmission();             // "Hang up the line" so others can use it (can h
      return 0;

}