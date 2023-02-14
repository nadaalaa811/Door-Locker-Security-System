/******************************************************************************
 *
 * File Name: main.c
 *
 * Description: Main source file for Control_ECU
 *
 * Author: Nada Alaa
 *
 *******************************************************************************/

#include "uart.h"
#include "motor.h"
#include "twi.h"
#include "std_types.h"
#include "external_eeprom.h"
#include "timer1.h"
#include <avr/io.h>        /* To use the SREG register */
#include <util/delay.h>

void count15Sec(void);
void count3Sec(void);

uint8 g_ticks = 0;

Timer1_ConfigType config_ptr = { 0, 8192, CPU_1024, COMPARE };

//
//void count3Sec(void) {
//	DcMotor_Rotate(STOP, 0);
//	g_ticks++;
//	if (g_ticks == 3) {
//		DcMotor_Rotate(CW, 100);
//		Timer1_setCallBack(count15Sec);
//		g_ticks = 0;
//		g_rounds++;
//	}
//}
//
//void count15Sec(void) {
//	g_ticks++;
//	if (g_ticks == 15) {
//		g_ticks = 0;
//		if (g_rounds == 1) {
//			g_rounds = 0;
//			Timer1_deInit();
//		} else {
//			Timer1_setCallBack(count3Sec);
//		}
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

uint8 checkMatching(uint8 *pass1, uint8 *pass2) {
	uint8 flag = 'F';
	for (uint8 i = 0; i < 5; i++) {
		if (pass1[i] != pass2[i]) {
			return 'F';
		}
	}
	return 'T';
}

void warning(void){
	Buzzer_on();
	Timer1_init(&config_ptr);
	while (g_ticks < 60)
		;
	g_ticks = 0;
	Buzzer_off();
	Timer1_deInit();
}
//uint8 systemHasPass() {
//	uint8 mempass[6];
//	uint8 sucsses;
//	for (uint8 i = 0; i < 5; i++) {
//		sucsses = EEPROM_readByte(0x0311 + i, mempass[i]);
//		_delay_ms(500);
//	}
//	if (mempass[0] == '\0') {
//		return 'N';
//	} else {
//		return 'Y';
//	}
//}

void countticks(void) {
	g_ticks++;
}

void rotateMotor(){

	DcMotor_Rotate(CW, 100);
	Timer1_init(&config_ptr);
	while (g_ticks < 15)
		;
	g_ticks = 0;
	DcMotor_Rotate(STOP, 0);
	while (g_ticks < 3)
		;
	g_ticks = 0;
	//A_CW problem
	DcMotor_Rotate(CW, 100);
	while (g_ticks < 15)
		;
	g_ticks = 0;
	DcMotor_Rotate(STOP, 0);
	Timer1_deInit();

}

void savePass() {
	uint8 flag = 'F';
	uint8 pass1[6];
	uint8 pass2[6];
	UART_receiveString(pass1);
	UART_sendByte('o');
	UART_receiveString(pass2);

	flag = checkMatching(pass1, pass2);
	UART_sendByte(flag);
	if (flag == 'T') {
		for (uint8 i = 0; i < 5; i++) {
			EEPROM_writeByte(0x0311 + i, pass1[i]);
			_delay_ms(20);
		}
	}
}
void recheck(void) {
	uint8 pass[6];
	uint8 mempass[6];
	uint8 success, flag;
	UART_receiveString(pass);
	for (uint8 i = 0; i < 5; i++) {
		success = EEPROM_readByte(0x0311 + i, &mempass[i]);
		_delay_ms(20);
	}
	mempass[5] = '\0';
	flag = checkMatching(pass, mempass);
	UART_sendByte(flag);
}
void changePass() {
	uint8 flag = 'F';
	uint8 success;
	uint8 pass[6];
	uint8 mempass[6];
	UART_receiveString(pass);

	for (uint8 i = 0; i < 5; i++) {
		success = EEPROM_readByte(0x0311 + i, &mempass[i]);
		_delay_ms(20);
	}

	flag = checkMatching(pass, mempass);
	UART_sendByte(flag);


}

void openDoor() {
	uint8 flag = 'F';
	uint8 success;
	uint8 pass[6];
	uint8 mempass[6];
	UART_receiveString(pass);

	for (uint8 i = 0; i < 5; i++) {
		success = EEPROM_readByte(0x0311 + i, &mempass[i]);
		_delay_ms(20);
	}
	mempass[5] = '\0';
	flag = checkMatching(pass, mempass);
	UART_sendByte(flag);

	if (flag == 'T') {
          rotateMotor();
	}

}

int main() {
	uint8 recievedbyte, sentbyte;
	UART_ConfigType uart_config_ptr = { BIT_8, EVEN, BIT_1, BP9600 };

	TWI_ConfigType twi_Config_Ptr = { MYADD, br };

	UART_init(&uart_config_ptr);

	TWI_init(&twi_Config_Ptr);
	DcMotor_Init();
	Buzzer_init();
	Timer1_setCallBack(countticks);
	SREG |= (1 << 7); /* Enable Global Interrupt I-Bit */

	while (1) {
		recievedbyte = UART_recieveByte();
		if (recievedbyte == '/') {
			recheck();
		}
		else if (recievedbyte == '+') {
			openDoor();
		} else if (recievedbyte == '-') {
			changePass();
		} else if (recievedbyte == '*') {
			savePass();
		}
		else if (recievedbyte == 'w') {
					warning();
				}
		else if(recievedbyte == 'r'){
			rotateMotor();
		}
	}
	return 0;
}
