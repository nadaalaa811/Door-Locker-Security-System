 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.h
 *
 * Description: Header file for the BUZZER driver
 *
 * Author: Nada Alaa
 *
 *******************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_


#include "std_types.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Description :
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO as initialization.
 */
void Buzzer_init(void);


/*
 * Description :
 * enable buzzer through GPIO.
 */

void Buzzer_on(void);

/*
 * Description :
 * disable buzzer through GPIO.
 */
void Buzzer_off(void);


#endif /* BUZZER_H_ */
