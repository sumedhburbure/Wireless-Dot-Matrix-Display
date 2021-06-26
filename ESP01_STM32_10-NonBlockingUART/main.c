#include "main.h"
char DEBUG[100];
extern uint8_t display_clear[8];
int led_status = 0;
int conn_status = 0;
int wifi_status = 0;

extern uint8_t disp1ay[26][8];		//temp for debugg
int main(void){
	systick_init();
	Comm_UART_Init();
	ESP_UART_Init();
	LED_INIT();
	BUTTON_INIT();
	MATRIX_Display_GPIO_INIT();
	MATRIX_DISP_INIT();
	MATRIX_DISP_CLEAR();
	int data = ESP_Init("Galaxy M310E33","sumedh123");
	sprintf(DEBUG,"The ESP Status is :%d\r\n",data);
	Comm_SendStringIn_tx_Buff(DEBUG);

	while(1){
		if(wifi_status){
			conn_status = ESP_CheckForString("IPD",0);
			if(conn_status){
				int Link = ESP_GetLinkID();
				int stat = web_page_manager(Link);
//				if(stat){
//					sprintf(DEBUG,"The string is:%s\r\n",string_buffer);
//					Comm_SendStringIn_tx_Buff(DEBUG);
//				}
				ESP_UART_Flush();
				conn_status = 0;
			}	
		}
		else{
				MATRIX_DISP_Scroll_String();
				DelayMs(2000);
				}
	}
}


//logic for loopback::
//		if(Is_Comm_rx_Available()){
//			char data = Comm_ReadFrom_rx_Buff();
//			ESP_StoreIn_tx_Buff(data);
//		}
//		if(Is_ESP_rx_Available()){
//			char data = ESP_ReadFrom_rx_Buff();
//			Comm_StoreIn_tx_Buff(data);
//		}
