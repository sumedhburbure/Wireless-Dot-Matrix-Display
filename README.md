# Wireless-Dot-Matrix-Display
This is the Project Demonstration of working of Dot Matrix Display paired with ESP-01 and STM32 BluePill. The demo illustrates the Scrolling Text displayed on Dot Matrix Display controlled wirelessly from any mobile phone or any device capable of opening a web browser. Dot Matrix Display enables user to update the Display information from Mobile Web Application.

You Can Watch the Working demo of the Project By clicking the Below Image:

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/lqELUlHfRbE/0.jpg)](http://www.youtube.com/watch?v=lqELUlHfRbE)


Block Diagram of Connections:
![Block Diagram of Connections](https://github.com/sumedhburbure/Wireless-Dot-Matrix-Display/blob/main/ESP01-STM32-MatrixDotDisplay.png)

---------------------------
## Project Overview:

#### UART:
The project uses two UARTs , one is to communicate with the ESP-01 and one for debugging. Both the UARTs operate at 115200 with no stop bit configurations.

UART1 -> Communication to PC for debugging via TTL 


| UART 1        | STM32 Pins    |    TTL     |
| ------------- |:-------------:| ---------- |
| UART TX       | Pin A9        |     RX     |
| UART RX       | Pin A10       |     TX     |

UART2 -> Communication with ESP-01 

| UART 2        | STM32 Pins    |   ESP-01 Pins   |
| ------------- |:-------------:| ----------      |
| UART TX       | Pin A2        |   Pin 7 (RX)    |
| UART RX       | Pin A3        |   Pin 2 (TX)    |

#### ESP-01
ESP-01 is used in this project for enabling the TCP communication between the Microcontroller STM32F103 BluePill and the web browser. ESP-01 is flashed with AT-Command firmware from ESPRESSIF official website. 
The library for communicating the ESP-01 via AT commands is written from scatch and ESP-01 is operated on Station Mode with static IP. Before Flashing Firmware on STM32 device please ensure to enter the correct SSID and Password.

[Click Here to View AT-Commands Manual](https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf)

#### STM32 BluePill:
STM32 Bluepill Development uses STM32F103C8 micro-controller and is operated at it's default clock frequency. The micro-controller plays a key role in operating Dot Matrix Display, operating ESP-01 and handling user requests over TCP-IP. The micro-controller peripherals are written in register level programming to reduce the size of the code.
The micro-controller also manages the browser part via uart to the ESP-01 . The micro-controller sends the HTML code to the browser and receives the user input from the browser via ESP-01.
[Click Here to View STM32F103 Reference Manual](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)

#### Dot-Matrix Display:
The Dot-Matrix Display is an 8x8 led matrix display which consumes 5v and has a custom interface somewhat similar to shift registers. The display is driven by the MAX7219 chip. The library to drive the led display is written by examining the datasheet. The font is written from scratch. Hence , only Capital Characters are supported , no numeric or small letters are supported as this is just for demonstration purpose. Scrolling effects enables the small display to show full words or sentences. The Display can be cascaded by adding more 8x8 Dot matrix display. [Click here to view the datasheet](https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf)

#### Button
Button is used to Toggle between two operational modes , namely Display Scrolling and User Input. On restart , the User Input mode runs where the micro-controller waits for the user to connect to web browser and send a string of characters. After User sends a string, pressing the button will enable Display Scrolling as shown in the youtube video attested at the beginning. 

-------------------------

Thank You! For any queries , please contact me on sumedhburbure@outlook.com.
