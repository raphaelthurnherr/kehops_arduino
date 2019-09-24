/**
 * \file efm8_mcu_kehops.c
 * \brief efm8bb microcontroller with kehops firmware
 * \author Raphael Thurnherr
 * \version 0.1
 * \date 18.03.2019
 *
 * Library to get sonar, counter and din data
 * 
 */

#ifndef I2CSIMU

// REGISTER DEFINITION FOR EFM8 MICROCONTROLLER

#define BOARDTYPE_REG                       0x00
#define FIRMWARE_REG                        0x01
#define DIN_REG                             0x04
#define DISTANCE_INST_MM                    0x08
#define AIN0MV                              0x0E
#define ENC_FREQ0                           0x13
#define ENC_CNT1                            0x14    
#define ENC_FREQ1                           0x17
#define ENC_CNT0                            0x18
#define ENC_CNT1_RESET                      0x24
#define ENC_CNT0_RESET                      0x28

#include "efm8_mcu_kehops.h"
#include "arduino-i2c.h"


/**
 * \brief EFM8BB_getChannel, Get the value of register for channel xxx
 * \param pointer on the configuration structure
 * \return code error
 */
int EFM8BB_getChannel(device_efm8McuKehops *efm8bbconfig, unsigned char channel){
    char err = 0;
    unsigned int deviceReg_msb, deviceReg_lsb;
    unsigned char deviceAddress = efm8bbconfig->deviceAddress;
    unsigned char data_lsb =0 , data_msb =0;
    
    deviceReg_lsb = deviceReg_msb = 0;
    
    switch(channel){
        case    0   : deviceReg_lsb = BOARDTYPE_REG; break;
        case    1   : deviceReg_lsb = FIRMWARE_REG; break;
   
        case    2   : deviceReg_lsb = DIN_REG; break;
        case    3   : deviceReg_lsb = DIN_REG; break;        
        case    4   : deviceReg_lsb = DIN_REG; break;
        case    5   : deviceReg_lsb = DIN_REG; break;
        
        case    6   : deviceReg_lsb = DISTANCE_INST_MM; 
                      deviceReg_msb = DISTANCE_INST_MM+1;  break;
        
        case    7   : deviceReg_lsb = AIN0MV;
                      deviceReg_msb = AIN0MV+1; break;
        
        case    8   : deviceReg_lsb = ENC_FREQ0; break;
        case    9   : deviceReg_lsb = ENC_CNT0;
                      deviceReg_msb = ENC_CNT0+1; break;
        
        case    10   : deviceReg_lsb = ENC_FREQ1; break;
        case    11   : deviceReg_lsb = ENC_CNT1;
                      deviceReg_msb = ENC_CNT1+1; break;
        
        case    12   : deviceReg_lsb = ENC_CNT0_RESET; break;
        case    13   : deviceReg_lsb = ENC_CNT1_RESET; break;
        
        default:    break;
    }


    err += i2c_readByte(0, deviceAddress, deviceReg_lsb, &data_lsb);
    
    if(deviceReg_msb >0);
        err += i2c_readByte(0, deviceAddress, deviceReg_msb, &data_msb);

    if(!err){
        switch(channel){
            case 2 : data_lsb &= 0x01; break;    // Set mask for din 0
            case 3 : data_lsb &= 0x02; break;    // Set mask for din 1
            case 4 : data_lsb &= 0x04; break;    // Set mask for din 2
            case 5 : data_lsb &= 0x08; break;    // Set mask for din 3
            default: break;
        }
        
        return (data_msb<<8) + data_lsb;
    }else{
        //printf("EFM8BB_getChannel() -> Read error\n");
        return -1;
    }
}

/**
 * \brief EFM8BB_clearWheelDistance, reset the counter
 * \param pointer on the configuration structure
 * \return code error
 */
int EFM8BB_clearWheelDistance(device_efm8McuKehops *efm8bbconfig, unsigned char channel){
    char err = 0;
    unsigned char deviceAddress = efm8bbconfig->deviceAddress;
    
    unsigned char deviceReg_lsb;
    int dummy;
    
        switch(channel){
        case    9   : deviceReg_lsb = ENC_CNT0_RESET; break;
        case    10   : deviceReg_lsb = ENC_CNT1_RESET; break;
        default:    break;
    }

    err += i2c_readByte(0, deviceAddress, deviceReg_lsb, dummy);
    
    return err;
}

/**
 * \brief EFM8BB_init, microcontroller initialization
 * \param pointer on the configuration structure
 * \return code error
 */
int EFM8BB_init(device_efm8McuKehops *efm8bbconfig){
    char err=0;
    err += EFM8BB_clearWheelDistance(efm8bbconfig, 9);
    err += EFM8BB_clearWheelDistance(efm8bbconfig, 10);
    return err;
}






#endif