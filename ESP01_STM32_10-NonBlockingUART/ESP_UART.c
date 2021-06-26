#include "ESP_UART.h"
esp_uart esp_rx;
esp_uart esp_tx;


//temp
#include "COMM_UART.h"
#include "stdio.h"
char DEBUG1[100];
char string_buffer[200];
/*************************************
*	USART 2 For Communicating with ESP 
* PA2  -> TX
* PA3  -> RX
* Baud Rate -> 115200
**************************************/
void ESP_UART_Init(void){
	//----USART 2 Initialization----//
	RCC->APB1ENR |= ((0x01)<<17);  //Enable USART2 Clock
	GPIOA->CRL |= (0xA00) ;//PA2 pin TX as alternate function output pushpull 
	//PA3 already configured as input pushpull so no need to configure it FOR RX
	USART2->BRR = 0x138;//Baud rate setting to 115200
	USART2->CR1 |= ((0x01)<<2) | ((0x01)<<3) | ((0x01)<<13) | ((0x01)<<5); //TX RX AND USART ENABLE
	NVIC_EnableIRQ(USART2_IRQn);
}

/*******************************************
* ESP_UART_Flush function is used for 
* flushing the uart rx ring buffer
********************************************/
void ESP_UART_Flush(void){
	memset(esp_rx.buffer,'\0',ESP_BUFFER_SIZE);
	esp_rx.head = 0;
	esp_rx.tail = 0;
}

/*******************************************
* ESP_StoreIn_rx_Buff function is used for 
* storing rx buffer from UART DATA Register
********************************************/
void ESP_StoreIn_rx_Buff(char data){
	int i = (esp_rx.head + 1) % ESP_BUFFER_SIZE;
	if(i != esp_rx.tail){
		esp_rx.buffer[esp_rx.head] = data;
		esp_rx.head = i;	
	}
}

/*******************************************
* ESP_StoreIn_tx_Buff function is used for 
* storing tx ring buffer to send it to UART 
* DATA Register
********************************************/
void ESP_StoreIn_tx_Buff(char data){
	int i = (esp_tx.head + 1) % ESP_BUFFER_SIZE;
	esp_tx.buffer[esp_tx.head] = data;
	esp_tx.head = i;
	USART2->CR1 |= (USART_CR1_TXEIE); // Enable TX Data Register Empty Interrupt
}

/*******************************************
* ESP_ReadFrom_rx_Buff function is used for 
* reading rx ring buffer to send it to UART 
* DATA Register
********************************************/
char ESP_ReadFrom_rx_Buff(void){
	int i = (esp_rx.tail + 1) % ESP_BUFFER_SIZE;
	char data = esp_rx.buffer[esp_rx.tail];
	esp_rx.tail = i;
	return data;
}

/*******************************************
* ESP_SendStringIn_tx_Buff function is used 
* for storing string in tx buffer to send it
* to UART DATA Register
********************************************/
void ESP_SendStringIn_tx_Buff(char *data){
	while(* data){
		ESP_StoreIn_tx_Buff(*data++);
	}
}

/*******************************************
* Is_ESP_rx_Available function is used for 
* checking if anything is left to read.
********************************************/
uint16_t Is_ESP_rx_Available(void){
	return (uint16_t) ((ESP_BUFFER_SIZE -  (esp_rx.head - esp_rx.tail)) % ESP_BUFFER_SIZE);
}

/*******************************************
* ESP_FeedbackLoop function is used for 
* debugging purpose. It receives data in Rx
* buffer and sends it back to tx buffer
********************************************/
void ESP_FeedbackLoop(void){
	if(Is_ESP_rx_Available()){
		char data = ESP_ReadFrom_rx_Buff();
		ESP_StoreIn_tx_Buff(data);
	}
}

/*******************************************
* USART1_IRQHandler function is an interrupt
* handler for usart1.
********************************************/
void USART2_IRQHandler(void){
	if(USART2->SR & USART_SR_RXNE){
		char c = USART2->DR;
		ESP_StoreIn_rx_Buff(c);
	}
	else if(USART2->SR & (USART_SR_TXE)){
		if(esp_tx.head == esp_tx.tail){
			USART2->CR1 &= ~(USART_CR1_TXEIE); //Disable TX Data Register Empty Interrupt
		}
		else{
			volatile int i = (esp_tx.tail +1) % ESP_BUFFER_SIZE;
			USART2->DR = esp_tx.buffer[esp_tx.tail];
			esp_tx.tail = i;
		}
	}
}


//------------------------------------String Functions-------------------------------------------------//
/*******************************************
* ESP_SendCommand function is used to send 
* command to ESP via USART2 and check the 
* desired response. If response is 
********************************************/
uint8_t ESP_SendCommand(char *command,int delay,char *response){
	uint8_t retry = 0;
	ESP_UART_Flush();
	ESP_SendStringIn_tx_Buff(command);
	while((retry < 3) && (!ESP_CheckForString(response,delay))){
		retry++;
		ESP_SendStringIn_tx_Buff(command);
		sprintf(DEBUG1,"retry!\r\n");
		Comm_SendStringIn_tx_Buff(DEBUG1);
	}
	if(retry >=3){return 0;}
	else{return 1;}
}

/*******************************************
* ESP_CheckForString function is used to 
* check whether the desired response is 
* received in ESP buffer or not
********************************************/
uint8_t ESP_CheckForString(char *data,int timeOut){
	DelayMs(timeOut);
	esp_rx.searchIndex = 0;
	int strLength = strlen(data);
	int pos = 0;
	int match = 0;

	while(esp_rx.searchIndex < ESP_BUFFER_SIZE){
			if(data[pos] != esp_rx.buffer[esp_rx.searchIndex]){
				esp_rx.searchIndex++;
				match = 0;
				pos = 0;
			}
			else if(data[pos] == esp_rx.buffer[esp_rx.searchIndex]){
				pos++;
				match++;
				esp_rx.searchIndex++;
				if(match == strLength){
					return 1;
				}
			}
	}
	return 0;
}

void ESP_GetAfter(char *string,char *buffer,int size){
	esp_rx.searchIndex = 0;
	int strLength = strlen(string);
	int bufferLength =strlen(buffer);
	int pos = 0;
	int match = 0;
	uint8_t match_found = 0;
	while(esp_rx.searchIndex < ESP_BUFFER_SIZE){
		if(string[pos] != esp_rx.buffer[esp_rx.searchIndex]){
				esp_rx.searchIndex++;
				match = 0;
				pos = 0;
			}
			else if(string[pos] == esp_rx.buffer[esp_rx.searchIndex]){
				pos++;
				match++;
				esp_rx.searchIndex++;
				if(match == strLength){
					match_found = 1;
					break;
				}
			}
	}
	if(match_found){
		for(int i = 0; i<size;i++){
			buffer[i] = esp_rx.buffer[esp_rx.searchIndex++];
		}
	}
}

//fstring= hhtp
uint8_t GetData_From_Web(void){
	memset(string_buffer,'\0',100);
	char data_buff[200];
	ESP_GetAfter("fstring=",data_buff,50);
	ESP_CopyUpto(data_buff," HTTP",string_buffer);
	Comm_SendStringIn_tx_Buff("REply Got:");
	Comm_SendStringIn_tx_Buff(string_buffer);
	return 0;
}

void ESP_CopyUpto(char * buffer,char *trimafter,char *saveBuffer){
	int buff_size = strlen(buffer);
	int trimmer_size = strlen(trimafter);
	int pos = 0;
	int search_pos = 0;
	int tag = 0;
	int match = 0;
	
	while(search_pos < buff_size){
		if(trimafter[pos] !=  buffer[search_pos]){
			search_pos++;
			pos = 0;
			match = 0;
		}
		else if(trimafter[pos] ==  buffer[search_pos]){
			match++;
			pos++;
			search_pos++;
		}
		if(match == trimmer_size){
			break;
		}
	}
	if(match == trimmer_size){
		tag = search_pos - trimmer_size ;
		for(int i = 0; i < tag; i++){
			saveBuffer[i] = buffer[i];
		}
	}
}
