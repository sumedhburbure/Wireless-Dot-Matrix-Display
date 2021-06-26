#ifndef __MATRIX_DISP_H
#define __MATRIX_DISP_H
	#include "BSP.h"
	#include "COMM_UART.h"	//temp for debugg
	#include "string.h"
	#include "stdio.h"			//temp for debugg
	// A5 as Clock 
// A6 as CS
// A7 as Data
	#define DISP_CLOCK 5
	#define DISP_CS    6
	#define DISP_DATA  7
	#define LOW 			 0
	#define HIGH			 1
	//function declaration
	void MATRIX_DISP_INIT(void);
	void write_to_Display(uint8_t address,uint8_t data);
	void MATRIX_DISP_CHAR(uint8_t data);
	void MATRIX_DISP_Scroll_String(void);
	void MATRIX_DISP_Scroll(uint8_t data[8]);
	void MATRIX_DISP_CLEAR(void);
#endif
