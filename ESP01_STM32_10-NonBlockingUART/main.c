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
	int data = ESP_Init("YOUR SSID","YOUR PASSWORD");		//Enter Your SSID and Password Here
	sprintf(DEBUG,"The ESP Status is :%d\r\n",data);
	Comm_SendStringIn_tx_Buff(DEBUG);

	while(1){
		if(wifi_status){
			conn_status = ESP_CheckForString("IPD",0);
			if(conn_status){
				int Link = ESP_GetLinkID();
				int stat = web_page_manager(Link);
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

