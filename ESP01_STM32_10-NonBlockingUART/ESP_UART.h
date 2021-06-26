#ifndef __ESP_UART_H
#define __ESP_UART_H
	#include "stm32f10x.h"
	#include "string.h"
	#include "BSP.h"
	#include "stdint.h"
	#define ESP_BUFFER_SIZE 1000
	//variables declaration
	typedef struct{
		uint8_t buffer[ESP_BUFFER_SIZE];
		volatile int head;
		volatile int tail;
		int searchIndex;
	}esp_uart;
	extern char string_buffer[200];
//function declarations
	void ESP_UART_Init(void);
  char ESP_ReadFrom_rx_Buff(void);
	void ESP_StoreIn_tx_Buff(char data);
	void ESP_UART_Flush(void);
	uint16_t Is_ESP_rx_Available(void);
  void ESP_SendStringIn_tx_Buff(char *data);
	void ESP_FeedbackLoop(void);
	
	uint8_t ESP_SendCommand(char *command,int delay,char *response);
	uint8_t ESP_CheckForString(char *data,int timeOut);
	void ESP_GetAfter(char *string,char *buffer,int size);
	uint8_t GetData_From_Web(void);
	void ESP_CopyUpto(char * buffer,char *trimafter,char *saveBuffer);
#endif
