 /******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the BUZZER driver
 *
 * Author: Nada Alaa
 *
 *******************************************************************************/
#include "buzzer.h"
#include "gpio.h"


/*
 * Description :
 * Setup the direction for the buzzer pin as output pin through the GPIO driver.
 * Turn off the buzzer through the GPIO as initialization.
 */
void Buzzer_init(void) {
	GPIO_setupPinDirection(PORTA_ID, PIN0_ID, PIN_OUTPUT);
	//at the beginning(BUZZER OFF)
	GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_LOW);
}

/*
 * Description :
 * enable buzzer through GPIO.
 */

void Buzzer_on(void) {
	GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_HIGH); // Set pin 5 in PORTC with value 1 (BUZZER ON)
}

/*
 * Description :
 * disable buzzer through GPIO.
 */
void Buzzer_off(void) {
	// Set pin 5 in PORTC with value 0 (BUZZER OFF)
	GPIO_writePin(PORTA_ID, PIN0_ID, LOGIC_LOW);
}

