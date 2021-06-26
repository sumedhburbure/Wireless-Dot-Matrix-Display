#include "ESP_APP.h"
#include "web_page.h"
uint8_t ESP_Init(char *SSID, char *Password){
	uint8_t valid = 0;
	char wifi_credentials[100];
	//Checking if ESP is responding	
	valid = ESP_SendCommand("AT\r\n",100,"OK");
	if(!valid){return 2;}
	//Setting ESP on Station Mode
	valid = ESP_SendCommand("AT+CWMODE=1\r\n",100,"OK");
	if(!valid){return 3;}
	//Setting WIFI SSID and Password
	sprintf(wifi_credentials,"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,Password);
	valid = ESP_SendCommand(wifi_credentials,10000,"WIFI CONNECTED");
	if(!valid){return 4;}
	//Enabling Multiple Connections to Server
	valid = ESP_SendCommand("AT+CIPMUX=1\r\n",100,"OK");
	if(!valid){return 5;}
	//Enabling the Server
	valid = ESP_SendCommand("AT+CIPSERVER=1,80\r\n",100,"OK");
	ESP_UART_Flush();
	if(!valid){return 6;}
	return valid;
}


char ESP_GetLinkID(void){
	char buff[1];
	ESP_GetAfter("IPD,",buff,1);
	char LinkID = buff[0] - '0';
	return LinkID;
}

uint8_t ESP_Send_WebPage(char LinkID,char *web_page){
	uint8_t valid = 0;
	char buff[20];
	int webPageSize = strlen(web_page);
	ESP_UART_Flush();	
	sprintf(buff,"AT+CIPSEND=%d,%d\r\n",LinkID,webPageSize);
	valid = ESP_SendCommand(buff,100,">");
	if(!valid){return 7;}
	ESP_SendStringIn_tx_Buff(web_page);
	sprintf(buff,"AT+CIPCLOSE=%d\r\n",LinkID);
	valid = ESP_SendCommand(buff,500,"CLOSED");
	if(!valid){return 8;}
	ESP_UART_Flush();
	return valid;
}

uint8_t web_page_manager(char LinkID){
	uint8_t response = 0;
	if(ESP_CheckForString("/page1",10)){
		Comm_SendStringIn_tx_Buff("\nWebPageManager1\n");
		GetData_From_Web();
		response = ESP_Send_WebPage(LinkID,page1);
	}
	else{
		Comm_SendStringIn_tx_Buff("\nWebPageManager2\n");
		response = ESP_Send_WebPage(LinkID,home);
	}
	return response;
}


