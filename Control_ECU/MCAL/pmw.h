/******************************************************************************
 *
 * Module: PMW Signal
 *
 * File Name: pmw.h
 *
 * Description: header file for PWM driver
 *
 * Author: Nada
 *
 *******************************************************************************/

#ifndef PMW_H_
#define PMW_H_

#include "std_types.h"

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
/*
 * Description:
 * Generate a PWM signal with Timer0 using PWM Mode
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PMW_H_ */
