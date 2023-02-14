/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: MOTOR.h
 *
 * Description: header file for DC-Motor driver
 *
 * Author: Nada
 *
 *******************************************************************************/

#ifndef MOTOR_H_
#define MOTOR_H_
#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

/* MOTOR HW Ports and Pins Ids */

#define MOTOR_IN1_PORT_ID                 PORTA_ID
#define MOTOR_IN1_PIN_ID                  PIN1_ID

#define MOTOR_IN2_PORT_ID                 PORTA_ID
#define MOTOR_IN2_PIN_ID                  PIN2_ID

#define SPEED_TO_DUTY_CYCLE_SCALING      2.55
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum {
	STOP,A_CW,CW
} DcMotor_State;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for initialize the DC-Motor driver.
 */
void DcMotor_Init(void);

/*
 * Description :
 * Function responsible for initialize the DC-Motor driver.
 * The function responsible for rotate the DC Motor or stop the motor
 *  based on the input state value
 */
void DcMotor_Rotate(DcMotor_State state, uint8 speed);

#endif /* MOTOR_H_ */
