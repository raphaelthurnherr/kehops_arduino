/**
 * \file pca9685.c
 * \brief PCA9685 16 channels driver
 * \author Raphael Thurnherr
 * \version 0.1
 * \date 08.03.2019
 *
 * Library to setup and drive the 16 channel PCA9685 I2C device
 * 
 */

#ifndef I2CSIMU

// Register Definitions
#define MODE1 0x00		//Mode  register  1
#define MODE2 0x01		//Mode  register  2
#define SUBADR1 0x02		//I2C-bus subaddress 1
#define SUBADR2 0x03		//I2C-bus subaddress 2
#define SUBADR3 0x04		//I2C-bus subaddress 3
#define ALLCALLADR 0x05         //LED All Call I2C-bus address
#define LED0 0x06		//LED0 start register
#define LED0_ON_L 0x06		//LED0 output and brightness control byte 0
#define LED0_ON_H 0x07		//LED0 output and brightness control byte 1
#define LED0_OFF_L 0x08		//LED0 output and brightness control byte 2
#define LED0_OFF_H 0x09		//LED0 output and brightness control byte 3

#define ALLLED_ON_L 0xFA        //load all the LEDn_ON registers, byte 0 (turn 0-7 channels on)
#define ALLLED_ON_H 0xFB	//load all the LEDn_ON registers, byte 1 (turn 8-15 channels on)
#define ALLLED_OFF_L 0xFC	//load all the LEDn_OFF registers, byte 0 (turn 0-7 channels off)
#define ALLLED_OFF_H 0xFD	//load all the LEDn_OFF registers, byte 1 (turn 8-15 channels off)
#define PRE_SCALE 0xFE		//prescaler for output frequency

#define CHANNEL_MULTIPLYER 4	// For the other 15 channels
#define INTERNAL_CLK_FREQ 25000000     //25MHz default oscillator clock

// dependency definition
#include "pca9685.h"

#include "arduino-i2c.h"

char pca9685_init(device_pca9685 *pca9685_handler);                                            // PCA9685 driver initialization
char pca9685_setPulseWidthTime(device_pca9685 *pca9685config, unsigned char channel, float timeMs);  // Set the pulse width time in mS
char pca9685_setPWMdutyCycle(device_pca9685 *pca9685config, unsigned char channel, unsigned char value); // Set dutycycle for selected channel
    
/**
 * \fn char pca9685_init(device_pca9685 *pca9685config)
 * \brief PCA9685 driver initialization
 *
 * \param handler to PCA9685 configuration structure
 * \return Error
 * =0, no error
 * >0, error count during device initialization
 * <0, configuration error
 *  Note that configuration error is prioritary on device bus initialization error
 */


char pca9685_init(device_pca9685 *pca9685config){
    char err=0;
    
    unsigned char deviceAddr = pca9685config->deviceAddress;
    unsigned char freq = pca9685config->frequency;
    unsigned char totemPole = pca9685config->totemPoleOutput;
    unsigned char invertedOut = pca9685config->invertedOutput;
    long extClkMHz = pca9685config->externalClock;

    long clockFreq;

    unsigned char data, config_error;


    // MODE1 register, sleep before config
    err+= i2c_write(0, deviceAddr, MODE1, 0x10);


    // MODE1 register setup and maintain sleep mode
    data=0x10;

    clockFreq = INTERNAL_CLK_FREQ;                      // default value is 25MHz

    if(extClkMHz>0){                                    // Check if external clock is used
        if(extClkMHz < 50000000){
            data |= 0x40;                                   
            clockFreq = extClkMHz;
        }
        else
            config_error=1;
    }

    // Write MODE1 Register
    err+= i2c_write(0, deviceAddr, MODE1, data);

    // Prescaler config for user frequency operation
    unsigned char prescale_value =((clockFreq/4096) / freq)-1;
    // Write PRE_SCALE Register
    err+= i2c_write(0, deviceAddr, PRE_SCALE, prescale_value);

    // Setup MODE 2 register (inverted output and output type)
    data=0;

    if(invertedOut)
        data |= 0x10;
    if(totemPole)
        data |= 0x04;

    // Write MODE2 Register
    err+= i2c_write(0, deviceAddr, MODE2, data);

    // All outputs turn-on on clock 0
    err+= i2c_write(0, deviceAddr, ALLLED_ON_L, 0x00);
    err+= i2c_write(0, deviceAddr, ALLLED_ON_H, 0x00);

    // Write MODE 1 register, system ready (no sleep, no allcall_adr)
    err+= i2c_write(0, deviceAddr, MODE1, 0x81);

    // Return configuration error in priority
    if(!config_error)
        return err;    
    else 
        return -1;
}

/**
 * \fn char pca9685_setPWMdutyCycle(device_pca9685 *pca9685config, unsigned char channel, unsigned char value)
 * \brief Set Dutycyle value for selected channel
 *
 * \param handler to PCA9685 configuration structure
 * \return Error
 * =0, no error
 * >0, error count during device initialization
 * <0, configuration error
 *  Note that configuration error is prioritary on device bus initialization error
 */

char pca9685_setPWMdutyCycle(device_pca9685 *pca9685config, unsigned char channel, unsigned char dutyCycle){
    char err=0;        
    
    unsigned int power;
    unsigned char PowerLow;
    unsigned char PowerHigh;
    unsigned char channelAddr;
    unsigned char deviceAddr = pca9685config->deviceAddress;

    // Dutycycle % to register value conversion
    if(dutyCycle<=100){
        
        //power = (4096*dutyCycle) /100;
        power = ((float)(4096/100) *dutyCycle);
        PowerLow = power & 0x00FF;
        PowerHigh = (power & 0x0F00) >>8;

        // Get the channel address register for ON value and set to clock 0
        channelAddr = LED0_ON_H + (channel * CHANNEL_MULTIPLYER);
        err+=i2c_write(0, deviceAddr, channelAddr, 0x00);
        err+=i2c_write(0, deviceAddr, channelAddr+1, 0x00);

        // Get the channel address register for OFF value        
        channelAddr = LED0_OFF_L + (channel * CHANNEL_MULTIPLYER);    
    }else{
        // Set output full ON
        channelAddr = LED0_ON_H + (channel * CHANNEL_MULTIPLYER);
        PowerLow = PowerHigh = 0xFF;                      
    }

    err+=i2c_write(0, deviceAddr, channelAddr, PowerLow);
    err+=i2c_write(0, deviceAddr, channelAddr+1, PowerHigh);
    return err;
}

/**
 * \fn char pca9685_setPulseWidthTime(device_pca9685 *pca9685config, unsigned char channel, int value)
 * \brief Set the pulse width time in mS
 *
 * \param handler to PCA9685 configuration structure
 * \return Error
 * =0, no error
 * >0, error count during device initialization
 * <0, configuration error
 *  Note that configuration error is prioritary on device bus initialization error
 */

char pca9685_setPulseWidthTime(device_pca9685 *pca9685config, unsigned char channel, float timeMs){
    char err=0;
    unsigned char deviceAddr = pca9685config->deviceAddress;
    int userFreq = pca9685config->frequency;
   
    unsigned char lsbValue, msbValue;
    unsigned int reloadValue;
    
    float clockPulseTime;

    // Calculation of pulse time clock for external oscillator;
    if(timeMs>0.0){
        clockPulseTime = (float)(1000/userFreq)/4096;

        reloadValue = timeMs/clockPulseTime;

        if(reloadValue > 4095) 
            reloadValue = 4095;
    }else 
        reloadValue = 0;
    
    lsbValue = reloadValue & 0x00FF;
    msbValue = (reloadValue & 0x0F00)>>8;
    
    // Get the channel address register
    unsigned char channelAddr = LED0_OFF_L + (channel * CHANNEL_MULTIPLYER);
        
    err+=i2c_write(0, deviceAddr, channelAddr, lsbValue);
    err+=i2c_write(0, deviceAddr, channelAddr+1, msbValue);
    
    return err;
}

#endif