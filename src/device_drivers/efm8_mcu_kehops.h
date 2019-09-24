/**
 * \file efm8_mcu_kehops.h
 * \brief efm8bb microcontroller with kehops firmware
 * \author Raphael Thurnherr
 * \version 0.1
 * \date 18.03.2019
 *
 * Library to get sonar, counter and din data
 * 
 */

#ifndef EFM8_MCU_KEHOPS_H
#define EFM8_MCU_KEHOPS_H

#include <Arduino.h>

/**
 * \struct efm8_mcu_kehops [efm8_mcu_kehops.h] Configuration structure definition
 */
typedef struct efm8_mcu_kehops{
    char deviceName[25];                        // Device Name of IC
    unsigned char deviceAddress;                // Bus device address
} device_efm8McuKehops;

/**
 * \brief EFM8BB_getChannel, Get the value of register for channel xxx
 * \param pointer on the configuration structure
 * \return code error
 */
extern int EFM8BB_getChannel(device_efm8McuKehops *efm8bbconfig, unsigned char channel);

/**
 * \brief EFM8BB_clearWheelDistance, reset the counter
 * \param pointer on the configuration structure
 * \return code error
 */
extern int EFM8BB_clearWheelDistance(device_efm8McuKehops *efm8bbconfig, unsigned char channel);


/**
 * \brief EFM8BB_init, microcontroller initialization
 * \param pointer on the configuration structure
 * \return code error
 */
extern int EFM8BB_init(device_efm8McuKehops *efm8bbconfig);
                        
#endif /* EFM8_MCU_KEHOPS_H */