#include "MATRIX_DISP.h"
extern char string_buffer[200];
char DEBUG2[100];
uint8_t disp1ay[26][8] = {
	{0x18,0x24,0x42,0x42,0x7E,0x42,0x42,0x42},		//A
	{0x7C,0x42,0x42,0x42,0x7C,0x42,0x42,0x7C},		//B
	{0x7E,0x40,0x40,0x40,0x40,0x40,0x40,0x7E},		//C
	{0x78,0x44,0x42,0x42,0x42,0x42,0x44,0x78},		//D
	{0x7E,0x40,0x40,0x78,0x40,0x40,0x40,0x7E},		//E
	{0x7E,0x40,0x40,0x78,0x40,0x40,0x40,0x40},		//F
	{0x7E,0x40,0x40,0x40,0x5E,0x42,0x42,0x7E},		//G
	{0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42},		//H
	{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x7E},		//I
	{0x7E,0x08,0x08,0x08,0x08,0x08,0x08,0x38},		//J
	{0x42,0x42,0x44,0x48,0x70,0x48,0x44,0x42},		//K
	{0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7E},		//L
	{0x42,0x66,0x5A,0x5A,0x42,0x42,0x42,0x42},		//M
	{0x42,0x62,0x52,0x4A,0x4A,0x4A,0x46,0x42},		//N
	{0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},		//O
	{0x7E,0x42,0x42,0x42,0x7E,0x40,0x40,0x40},		//P
	{0x7C,0x44,0x44,0x44,0x44,0x54,0x48,0x76},		//Q
	{0x7C,0x42,0x42,0x44,0x78,0x44,0x42,0x42},		//R
	{0x7E,0x42,0x40,0x7E,0x02,0x02,0x42,0x7E},		//S
	{0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x18},		//T
	{0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x7E},		//U
	{0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x18},		//V
	{0x42,0x42,0x42,0x42,0x5A,0x7E,0x66,0x42},		//W
	{0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42},		//X
	{0x42,0x42,0x24,0x18,0x18,0x18,0x18,0x18,},		//Y
	{0x7E,0x02,0x04,0x08,0x10,0x20,0x40,0x7E},		//Z
};

uint8_t display_clear[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void write_byteTo_Display(uint8_t byte){
	for(int i = 0 ; i < 8; i++){
		MATRIX_Display_WritePIN(DISP_CLOCK,LOW);					//Pull the Clock Pin LOW 
		MATRIX_Display_WritePIN(DISP_DATA ,byte&0x80);   	// write the MSB bit to the data pin	
		byte = byte << 1;
		MATRIX_Display_WritePIN(DISP_CLOCK,HIGH);					//Pull the Clock Pin HIGH 
	}
}

void write_to_Display(uint8_t address,uint8_t data){
	MATRIX_Display_WritePIN(DISP_CS,LOW);		// pull the CS pin LOW
	write_byteTo_Display(address);
	write_byteTo_Display(data);
	MATRIX_Display_WritePIN(DISP_CS,HIGH);		// pull the CS pin LOW
}

void MATRIX_DISP_CHAR(uint8_t data){
		uint8_t temp = data - 65;
		for(int i = 1; i < 9;i++){
			write_to_Display(i,disp1ay[temp][i-1]);
		}
		DelayMs(100);
}

void MATRIX_DISP_CLEAR(void){
		for(int i = 1; i < 9;i++){
			write_to_Display(i,display_clear[i-1]);
		}
		DelayMs(100);
}


void MATRIX_DISP_Scroll(uint8_t data[8]){
		uint8_t temp[8];
		for(int j = 0 ; j <= 8  ; j++){
			for (int i=0; i<8; i++)
			{
				temp[i] = (data[i] >> (8 - j));
			}
			for (int i=1; i<9; i++)
			{
				write_to_Display(i , temp[i-1]);
			}
			DelayMs(70);
		}
		
		for(int j = 7 ; j >= 0 ; j--){
			for (int i=0; i<8; i++)
			{
				temp[i] = (data[i] << (8 - j));
			}
			for (int i=1; i<9; i++)
			{
				write_to_Display(i , temp[i-1]);
			}
			DelayMs(100);
		}
}

void MATRIX_DISP_Scroll_String(void){
		uint8_t data_size = strlen(string_buffer);
		sprintf(DEBUG2,"The scroll string is :%s\r\n",string_buffer);
		Comm_SendStringIn_tx_Buff(DEBUG2);
		for(int i = 0;i < data_size;i++){
			if(string_buffer[i] == 43){			
			Comm_SendStringIn_tx_Buff("SPACE DETECTED \n");}
			uint8_t temp = string_buffer[i] - 65;
			if(temp < 27){
				sprintf(DEBUG2,"The array char is %c and index is :%d\r\n",string_buffer[i],temp);
				Comm_SendStringIn_tx_Buff(DEBUG2);
				MATRIX_DISP_Scroll(disp1ay[temp]);
			}
			else{
				MATRIX_DISP_Scroll(display_clear);
			}
		}
	
	
}


// A5 as Clock 
// A6 as CS
// A7 as Data
void MATRIX_DISP_INIT(void){
 write_to_Display(0x09, 0x00);       //  no decoding
 write_to_Display(0x0a, 0x01);       //  brightness intensity
 write_to_Display(0x0b, 0x07);       //  scan limit = 8 LEDs
 write_to_Display(0x0c, 0x01);       //  power down =0,normal mode = 1
 write_to_Display(0x0f, 0x00);       //  no test display
}


