/**
 * \file pca9629.h
 * \brief pca9629 Stepper motor drivers 
 * \author Raphael Thurnherr
 * \version 0.1
 * \date 18.03.2019
 *
 * Setup and drive stepper motors via PCA9629 IC
 * 
 */


#ifndef PCA9629_H
#define PCA9629_H

/**
 * \struct device_pca9629 [pca9629.h] Configuration structure definition
 */

typedef struct pca9629{
    char deviceName[25];                        // Device Name of IC
    unsigned char deviceAddress;                // Bus device address
    unsigned char pulsesWidth_ms;               // Specify the pulse width for motor driving
} device_pca9629;



/**
 * \brief Initial configuration for Stepper motor controller
 * \return code error
 */

extern int pca9629_init(device_pca9629 *pca9629config);



extern int PCA9629_StepperMotorControl(device_pca9629 *pca9629config, int data);


extern int PCA9629_StepperMotorSetStep(device_pca9629 *pca9629config, int stepCount);         //Configuration du registre "PAS" du driver moteur
extern int PCA9629_StepperMotorMode(device_pca9629 *pca9629config, int data);                 // Mode action continue ou unique
extern int PCA9629_StepperMotorPulseWidth(device_pca9629 *pca9629config, int data);           // Définition de la largeur d'impulstion
extern int PCA9629_ReadMotorState(device_pca9629 *pca9629config);                             // Lecture du registre de contrôle du moteur

/**
 * \brief pca9629_motorControl, Set the high level control of motor
 * \return code error
 */

extern int actuator_setStepperSpeed(device_pca9629 *pca9629config, int speed);
extern int actuator_setStepperStepAction(device_pca9629 *pca9629config, int direction, int stepCount);
extern int actuator_getStepperState(device_pca9629 *pca9629config);

#endif /* PCA9629_H */