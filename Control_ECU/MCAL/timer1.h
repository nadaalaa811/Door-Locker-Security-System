/*
 * timer1.h
 *
 *  Created on: Oct 26, 2022
 *      Author: Nada
 */

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum {
	NO_CLK_SOURCE, NO_PRESCALING, CPU_8, CPU_64, CPU_256, CPU_1024
} Timer1_Prescaler;

typedef enum {
	NORMAL , COMPARE
} Timer1_Mode;

typedef struct {
	uint16 initial_value;
	uint16 compare_value; // it will be used in compare mode only.
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
} Timer1_ConfigType;


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function to initialize the Timer driver with chosen passed configurations.
 * configurations passed as input structure are:
 * 1)initial value of the timer counter
 * 2)mode that timer will operate in
 * 3)pre-scaler value
 * 4)compare value to be used if mode is compare
 */
void Timer1_init(const Timer1_ConfigType * Config_Ptr);

/*
 * Description :
 * Function to disable the Timer1.
 */
void Timer1_deInit(void);

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void(*a_ptr)(void));



#endif /* TIMER1_H_ */
