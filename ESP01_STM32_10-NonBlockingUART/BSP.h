#ifndef __BSP_H
#define __BSP_H
	#include "stm32f10x.h"
	
	#define LED_TOGGLE 		0
	#define LED_ON 				1
	#define LED_OFF 			2
	extern int led_status;
	extern int wifi_status;
	//function definations
	void DelayMs(unsigned long t);
	void systick_init(void);
	void LED_INIT(void);
	void BUTTON_INIT(void);
	void LED_Function(uint8_t status);
	void MATRIX_Display_GPIO_INIT(void);
	void MATRIX_Display_WritePIN(uint8_t PinNo , uint8_t state);
#endif
