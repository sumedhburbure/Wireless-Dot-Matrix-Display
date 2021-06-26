#include "BSP.h"

//Systick CLock Functions
void systick_init(void)
{
	SysTick->CTRL = 0;
	SysTick->LOAD = 0x00FFFFFF;
	SysTick->VAL = 0;
	SysTick->CTRL |= 5;
}

void DelayMillis(void)
{
	SysTick->LOAD = 0x11940;
	SysTick->VAL = 0;
	while((SysTick->CTRL & 0x00010000) == 0);
}

void DelayMs(unsigned long t)
{
	for(;t>0;t--)
		{
			DelayMillis();
		}
}

//--------------GPIO PINS CONFIGURATIONS---------------------------------------------//
void LED_INIT(void){
	RCC->APB2ENR |= 0x10; /// Or  0b10000 --> Anabling Preiph GPIOC
	GPIOC->CRH &= 0xFF0FFFFF;  /// Reset the PORT C PIN 13
	GPIOC->CRH |= 0x00300000;  /// Set Port C PIN 13 as Output 
	GPIOC->ODR  |= 0x2000;     /// Set Port C Pin 13 
}

void BUTTON_INIT(void){
	RCC->APB2ENR |= (0x01)<<2;			//Enable Port A clock
	GPIOA->CRL &= ~(0xFF);
	GPIOA->CRL |= (0x01)<<3;				//Set Pin A0 as input with pull up / pull down
	EXTI->IMR |= (0x01);						//ENABLE INTERRUPT FOR EXTI0 LINE
	EXTI->FTSR |= (0X01);						//SET PIN TRIGGER FOR FALLING EDGE
	AFIO->EXTICR[0] &= ~(0xF);			//Setting Port A for EXTI0 line
	NVIC_EnableIRQ(EXTI0_IRQn);
}

// A5 as Clock 
// A6 as CS
// A7 as Data
void MATRIX_Display_GPIO_INIT(void){
	RCC->APB2ENR |= (0x01)<<2;			//Enable Port A Clock
	GPIOA->CRL &= ~((0x0FFF)<<20);  //Clear any previous values for Pin 5,6 and 7
	//Config for A5 Pin as Output
	GPIOA->CRL |= (0x1)<<21;				//Output mode with max speed 2MHz
	//Config for A6 Pin as Output
	GPIOA->CRL |= (0x01)<<25;				//Output mode with max speed 2MHz
	//COnfig for A7 Pin as Output
	GPIOA->CRL |= (0x01)<<29;				//Output mode with max speed 2MHz
}

void MATRIX_Display_WritePIN(uint8_t PinNo , uint8_t state){
	if(state){
		GPIOA->ODR |= (0x01)<<PinNo;
	}
	else{
		GPIOA->ODR &= ~((0x01)<<PinNo);
	}
}

void LED_Toggle(void){
		if(wifi_status){
			GPIOC->ODR  |= (0x2000);     /// Set Port C Pin 13 
		//	led_status = 1;
		}
		else if(!wifi_status){
			GPIOC->ODR  &= ~(0x2000);     /// Set Port C Pin 13 
		//	led_status = 0;
		}
		DelayMs(100);
}

void LED_Function(uint8_t status){
	if(status == LED_ON){
		GPIOC->ODR  &= ~(0x2000);
		led_status = 1;
	}
	else if(status == LED_OFF){
		GPIOC->ODR  |= (0x2000);     /// Set Port C Pin 13 
		led_status = 0;
	}
	else if(status == LED_TOGGLE){
		LED_Toggle();
	}
}

void EXTI0_IRQHandler(void){
		EXTI->PR |= (0X01);							//CLEAR THE INTERRUPT WHEN INTERRUPT IS HANDELED
		//LED_Toggle();
		wifi_status = (wifi_status? 0 : 1);
		if(wifi_status){
			LED_Function(LED_ON);
		}
		else{
			LED_Function(LED_OFF);
		}
		DelayMs(200);
}
