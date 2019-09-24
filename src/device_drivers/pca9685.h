/**
 * \file pca9685.h
 * \brief PCA9685 16 channels driver
 * \author Raphael Thurnherr
 * \version 0.1
 * \date 08.03.2019
 *
 * Library to setup and drive the 16 channel PCA9685 I2C device
 * 
 */

#ifndef PCA9685_H
#define PCA9685_H


/**
 * \struct pca9685 [pca9685.h] Configuration structure definition
 */

typedef struct pca9685{
    char deviceName[25];                        // Device Name of IC
    unsigned char deviceAddress;                // Bus device address
    int frequency;                              // Output frequency required
    unsigned char invertedOutput;               // >0, invert the output logic level
    unsigned char totemPoleOutput;              // >0, use the totem pole type (push-pull), otherwise open-drain type is used
    long externalClock;                         // 0, driver internal clock 25MHz, otherwise set the external frequency in [Hz]
} device_pca9685;


/**
 * \brief PCA9685 driver initialization
 * \param pointer on the configuration structure
 * \return code error
 */
extern char pca9685_init(device_pca9685 *pca9685config);        // PCA9685 driver initialization


/**
 * \brief Set the pulse width time in mS for channel
 * \return code error
 */
extern char pca9685_setPulseWidthTime(device_pca9685 *pca9685config, unsigned char channel, float timeMs);  // Set OFF value for selected channel


/**
 * \brief Set PWM dutycycle for selected channel
 * \return code error
 */
extern char pca9685_setPWMdutyCycle(device_pca9685 *pca9685config, unsigned char channel, unsigned char value); // Set dutycycle for selected channel

#endif /* PCA9685_H */