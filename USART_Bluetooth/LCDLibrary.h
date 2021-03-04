#ifndef LCDLibrary
#define LCDLibrary
// A 50 micro sec delay is added after every send char and send command to compensate for increased uC operating speed

#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>


#define DataPort PORTB
#define DataPortDirect DDRB
#define ControlPort PORTD
#define ControlPortDirect DDRD
#define EnablePin 5
#define ReadWrite 6
#define RegisterS 7

void CheckBusy(void);
void EnableRoutine(void);
void SendCommand(unsigned char command);
void SendData(unsigned char data_char);
void Send_a_String(char *stringofchars, uint8_t x, uint8_t y);
void InitializeLCD(void);

void CheckBusy(void)
{
	DataPortDirect = 0x00;
	ControlPort |= 1 << ReadWrite;
	ControlPort &= ~(1 << RegisterS);
	while( DataPort >= 0x80)
	{
		EnableRoutine();
	}
	DataPortDirect = 0xFF;
}

void EnableRoutine(void)
{
	ControlPort |= 1 << EnablePin;
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop");
	asm volatile ("nop"); // Increasing "nop" to compensate increase in operating speed
	ControlPort &= ~(1 << EnablePin);
}

void SendCommand(unsigned char command)
{
	CheckBusy();
	DataPort = command & 0xF0;
	ControlPort &= ~(1 << ReadWrite );
	ControlPort &= ~(1 << RegisterS);
	EnableRoutine();
	DataPort = 0x00;
	DataPort = command << 4;
	EnableRoutine();
	DataPort = 0x00;
}

void SendData(unsigned char data_char)
{
	CheckBusy();
	DataPort = data_char & 0xF0;
	ControlPort &= ~(1 << ReadWrite);
	ControlPort |= 1 << RegisterS;
	EnableRoutine();
	DataPort = 0x00;
	DataPort = data_char << 4;
	EnableRoutine();
	DataPort = 0x00;	
}
void Send_a_String(char *stringofchars, uint8_t x, uint8_t y)
{ 
	if(x == 0)
	{
		SendCommand(0x80+y);
		_delay_us(50);
	}
	else
	{
		SendCommand(0xC0+y);
		_delay_us(50);
	}
	
	while(*stringofchars > 0)
	{
		SendData(*stringofchars++);
		_delay_us(50);
	}
}

void InitializeLCD(void)
{
    ControlPortDirect |= ( 1 << EnablePin | 1 << ReadWrite | 1 << RegisterS);
    _delay_ms(15);
    //sei();
    
    SendCommand(0x01); // Clear Screen
    _delay_ms(2);
    SendCommand(0x02);// 4 Bit mode
    _delay_us(50);
	SendCommand(0x28); // Initialize LCD in 4Bit Mode
	_delay_us(50);
	SendCommand(0x06);
    SendCommand(0b00001100); // Cursor Settings
    _delay_us(50);
	SendCommand(0x80);
}
#endif