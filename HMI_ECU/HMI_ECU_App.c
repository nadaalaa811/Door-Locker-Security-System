/******************************************************************************
 *
 * File Name: main.c
 *
 * Description: Main source file for HMI_ECU
 *
 * Author: Nada Alaa
 *
 *******************************************************************************/
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include "timer1.h"
#include "gpio.h"
#include <avr/io.h>        /* To use the SREG register */
#include <util/delay.h>

void count15Sec(void);
void count3Sec(void);
void system_options(void);

uint8 g_ticks = 0;

Timer1_ConfigType config_ptr = { 0, 8192, CPU_1024, COMPARE };

void countticks(void) {
	g_ticks++;
}
//
//void count15Sec(void) {
//	g_ticks++;
//	if (g_ticks == 15) {
//		g_ticks = 0;
//		if (g_rounds == 1) {
//			g_rounds = 0;
//			Timer1_deInit();
//		}else{
//			Timer1_setCallBack(count3Sec);
//		}
//	}
//}
//
//void count3Sec(void) {
//	LCD_clearScreen();
//	g_ticks++;
//	if (g_ticks == 3) {
//		LCD_displayString("Door is locking");
//		Timer1_setCallBack(count15Sec);
//		g_ticks = 0;
//		g_rounds++;
//	}
//}
//
//void count1min(void) {
//	g_ticks++;
//	if (g_ticks == 10) {
//		g_ticks = 0;
//		Timer1_deInit();
//	}
//}

void enterPass(uint8 *arr) {
	uint8 i = 0;
	for (; i < 5;) {
		arr[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		i++;
		_delay_ms(350);
	}
	arr[i] = '\0';
	uint8 eq = ' ';
	while (eq != '=') {
		eq = KEYPAD_getPressedKey();
		_delay_ms(350);
	}

}

uint8 wrongPassLogic(void) {
	uint8 reenterpass1[6];
	uint8 flag = 'F';
	LCD_clearScreen();
	LCD_moveCursor(0, 0);
	LCD_displayString("Please enter pass:");
	LCD_moveCursor(1, 0);
	enterPass(reenterpass1);
	UART_sendByte('/');
	UART_sendString(reenterpass1);
	UART_sendByte('#');
	flag = UART_recieveByte();
	return flag;
}

void set_and_confirm_Password(void) {
	LCD_clearScreen();
	uint8 pass1[6];
	uint8 pass2[6];
	uint8 flag = 'F';
	do {
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("Plz enter pass:");
		LCD_moveCursor(1, 0);
		enterPass(pass1);
		LCD_moveCursor(0, 0);
		LCD_clearScreen();
		LCD_displayString("Plz re-enter the");
		LCD_moveCursor(1, 0);
		LCD_displayString("same pass:");
		enterPass(pass2);
		UART_sendByte('*');
		UART_sendString(pass1);
		UART_sendByte('#');
		UART_recieveByte();
		UART_sendString(pass2);
		UART_sendByte('#');

		flag = UART_recieveByte();
	} while (flag != 'T');
	system_options();
	//go to step 2
	/*not same password check in control ECU*/
}

void system_options(void) {
	uint8 flag = 'F';
	uint8 i = 0;
	uint8 choice = ' ';
	do {
		flag = 'F';
		choice = ' ';
		i = 0;
		LCD_clearScreen();
		LCD_moveCursor(0, 0);
		LCD_displayString("+ : Open Door");
		LCD_moveCursor(1, 0);
		LCD_displayString("- : Change Pass");

		choice = KEYPAD_getPressedKey();
		_delay_ms(350);
		if (choice == '+') {
			uint8 pass[6];
			LCD_clearScreen();
			LCD_moveCursor(0, 0);
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			enterPass(pass);

			UART_sendByte('+');
			UART_sendString(pass);
			UART_sendByte('#');

			flag = UART_recieveByte();

			if (flag == 'T') {
				LCD_clearScreen();
				LCD_moveCursor(0, 0);
				LCD_displayString("Door is Unlocking");
				Timer1_init(&config_ptr);
				while (g_ticks < 15)
					;
				g_ticks = 0;
				LCD_clearScreen();
				while (g_ticks < 3)
					;
				g_ticks = 0;
				LCD_displayString("Door is locking");
				while (g_ticks < 15)
					;
				g_ticks = 0;
				Timer1_deInit();
			} else {
				i = 0;
				while (flag != 'T' && i != 3) {
					i++;
					flag = wrongPassLogic();
				}
				if (flag == 'T') {
					UART_sendByte('r');
					LCD_clearScreen();
					LCD_moveCursor(0, 0);
					LCD_displayString("Door is Unlocking");
					Timer1_init(&config_ptr);
					while (g_ticks < 15)
						;
					g_ticks = 0;
					LCD_clearScreen();
					while (g_ticks < 3)
						;
					g_ticks = 0;
					LCD_displayString("Door is locking");
					while (g_ticks < 15)
						;
					g_ticks = 0;
					Timer1_deInit();
				} else {
					UART_sendByte('w');
					LCD_clearScreen();
					LCD_moveCursor(0, 0);
					LCD_displayString("System Error!");
					Timer1_init(&config_ptr);
					while (g_ticks < 60)
						;
					g_ticks = 0;
					LCD_clearScreen();
					Timer1_deInit();
				}
			}
		} else if (choice == '-') {
			uint8 newpass[6];
			LCD_clearScreen();
			LCD_moveCursor(0, 0);
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);
			enterPass(newpass);
			UART_sendByte('-');
			UART_sendString(newpass);
			UART_sendByte('#');

			flag = UART_recieveByte();
			if (flag == 'T') {
				set_and_confirm_Password();
			} else {
				i = 0;
				while (flag != 'T' && i != 3) {
					i++;
					flag = wrongPassLogic();
				}
				if (flag == 'T') {
					set_and_confirm_Password();
				} else {
					UART_sendByte('w');
					LCD_clearScreen();
					LCD_moveCursor(0, 0);
					LCD_displayString("System Error!");
					Timer1_init(&config_ptr);
					while (g_ticks < 60)
						;
					g_ticks = 0;
					LCD_clearScreen();
					Timer1_deInit();
				}
			}
		}
	} while (1);
}

int main() {
	uint8 recievedbyte, sentbyte;
	UART_ConfigType config_ptr = { BIT_8, EVEN, BIT_1, BP9600 };
	LCD_init();
	/* Configure RW of the LCD as output pin, put 0 on it */
	GPIO_setupPinDirection(PORTB_ID, PIN1_ID, PIN_OUTPUT);
	GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);

	UART_init(&config_ptr);

//	UART_sendByte('/');
//	recievedbyte = UART_recieveByte();
//	if(recievedbyte == 'Y'){
//		system_options();
//	}
//	else if(recievedbyte == 'N'){
//		set_and_confirm_Password();
//	}
	Timer1_setCallBack(countticks);
	SREG |= (1 << 7); /* Enable Global Interrupt I-Bit */
	set_and_confirm_Password();

	while (1) {
		system_options();
//		set_and_confirm_Password();
	}
	return 0;
}
