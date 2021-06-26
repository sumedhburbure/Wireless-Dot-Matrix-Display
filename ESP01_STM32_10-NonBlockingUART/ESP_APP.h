#ifndef __ESP_APP_H
#define __ESP_APP_H
	#include "ESP_UART.h"
  #include "stdio.h"
	
	#include "COMM_UART.h"
	uint8_t ESP_Init(char *SSID, char *Password);
	char ESP_GetLinkID(void);
	uint8_t web_page_manager(char LinkID);
#endif
