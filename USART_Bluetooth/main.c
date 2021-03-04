/*********************************************Bluetooth-HC-05_Module************************************/
// The code accepts 10 chars from bluetooth terminal application on mobile phone and displays the chars on LCD screen

#include <avr/io.h>
#define F_CPU 11059200UL		// Using external 11.0592 MHz crystal
#include <util/delay.h>
#include "USART_Header.h"
#include "LCDLibrary.h"

int main(void)
{
	
    USART_Init(71);				// 71 is the UBBR value to get 9600 bps baud rate at 11.0592 MHz
    InitializeLCD();
	
	_delay_ms(100);
	unsigned char Data_Received[10];	// Data buffer
	
    while (1) 
    {
		Send_a_String("Data:",0,0);     // Send string on LCD
		_delay_us(50);
		for (int i =0; i < 10; i++)
		{
			Data_Received[i]=USART_Receive();  // Receive 1 byte / char
			SendData(Data_Received[i]);        // Send Data to LCD
			_delay_us(50);					   // Delay introduced to compensate uC operating speed increase
		}
    }
}

