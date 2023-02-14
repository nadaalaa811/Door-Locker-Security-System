/******************************************************************************
 *
 * Module: DC-Motor
 *
 * File Name: MOTOR.c
 *
 * Description: source file for DC-Motor driver
 *
 * Author: Nada
 *
 *******************************************************************************/
#include <avr/io.h>
#include "motor.h"
#include "gpio.h"
#include "pmw.h"
#include "common_macros.h"  /* To use the macros like SET_BIT */

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void DcMotor_Init(void) {
	/* configure pin PB0, PB1,PB3 output pins */
	GPIO_setupPinDirection(MOTOR_IN1_PORT_ID, MOTOR_IN1_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_IN2_PIN_ID, MOTOR_IN2_PIN_ID, PIN_OUTPUT);


	/*Stop at the DC-Motor at the beginning through the GPIO driver*/
	 DcMotor_Rotate( STOP, 0);
}

void DcMotor_Rotate(DcMotor_State state, uint8 speed) {
	/*rotate the DC Motor CW/ or A-CW or stop the motor based on the input state value*/
	PORTA = (PORTA & 0xF9) | (state<<1);

	/*Send the required duty cycle to the PWM driver based on the required speed value.*/
	PWM_Timer0_Start(speed * SPEED_TO_DUTY_CYCLE_SCALING);
}

