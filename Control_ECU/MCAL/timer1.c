/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.c
 *
 * Description: Source file for the TIMER1 driver
 *
 * Author: Nada Alaa
 *
 *******************************************************************************/

#include "timer1.h"
#include <avr/interrupt.h>
#include "avr/io.h" /* To use the TIMER1 Registers */

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR (TIMER1_COMPA_vect) {
	if (g_callBackPtr != NULL_PTR) {
		(*g_callBackPtr)();
	}
}


ISR (TIMER1_OVF_vect) {
	if (g_callBackPtr != NULL_PTR) {
		(*g_callBackPtr)();
	}
}


/*
 * Description :
 * Function to initialize the Timer driver with chosen passed configurations.
 * configurations passed as input structure are:
 * 1)initial value of the timer counter
 * 2)mode that timer will operate in
 * 3)pre-scaler value
 * 4)compare value to be used if mode is compare
 */
void Timer1_init(const Timer1_ConfigType *Config_Ptr) {
	TCNT1 = Config_Ptr->initial_value;

	TCCR1A |= (1 << FOC1A);
	TCCR1B |= (Config_Ptr->prescaler) | (Config_Ptr->mode << 3);

	if (Config_Ptr->mode == COMPARE) {
		OCR1A = Config_Ptr->compare_value;
		TIMSK = (1 << OCIE1A);
	} else if (Config_Ptr->mode == NORMAL) {
		TIMSK = (1 << TOIE1);
	}



}

/*
 * Description :
 * Function to disable the Timer1 by disabling clock source
 */
void Timer1_deInit(void) {
	TCCR1B = 0;
}

/*
 * Description :
 * Function to set the Call Back function address.
 */
void Timer1_setCallBack(void (*a_ptr)(void)) {
	g_callBackPtr = a_ptr;
}

