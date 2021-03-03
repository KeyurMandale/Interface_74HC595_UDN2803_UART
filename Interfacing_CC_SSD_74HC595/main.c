/*********************************Two_Digit_Counter*******************************/
#include <avr/io.h>
#define F_CPU 1000000UL
#include <util/delay.h>

/*********************************************Macros*****************************/
#define D_S          0
#define SHCP         2
#define STCP         3
#define SIPO         PORTB
#define SIPO_Direct  DDRB
#define Unit_tran    4
#define Tens_Tran    5
//RCLK = STCP
//SRCLK = SHCP

void init_shift(void);
void shiftout(uint8_t num);

uint8_t lookup[10] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; // Lookup table for Common Cathode Seven Segment Display

int main(void)
{
	uint8_t counter   = 0;
	uint8_t UnitDigit = 0;
	uint8_t TensDigit = 0;
	uint8_t j=0;
	init_shift();
	while (1)
	{
		init_shift();
		while(j < 50)						// Execute the routine 50 times for displayed number to be discernible. Otherwise all segments of SSD will appear lit.
		{
			UnitDigit = counter % 10;		// Extract units digit
			shiftout(lookup[UnitDigit]);	// Shift out units digit from lookup table
			SIPO &= ~(1 << Tens_Tran);		// Disable the Tens Digit (Switch off SSD). Logic 0 on I/P of UDN2803 results in Logic 1 on O/P.
			SIPO |= (1 << Unit_tran);		// Enable the Units digit (Switch on). Logic 1 on I/P of UDN2803 results in Logic 0 on O/P.
			
			TensDigit = counter / 10;		// Extarct tens digit
			shiftout(lookup[TensDigit]);	// Shift out tens digit from lookup table
			SIPO &= ~(1 << Unit_tran);		// Disable the units digit (Switch off SSD)
			SIPO |= (1 << Tens_Tran);		// Enable the units digit (Switch on SSD)
			
			j++;
		}
		j=0;
		counter ++;
		if (counter > 99)
		{
			counter = 0;					// Refresh counter
		}
	}
}

void init_shift(void)
{
	SIPO_Direct |= 1 << D_S | 1 << SHCP | 1 << STCP | 1 << Unit_tran | 1 << Tens_Tran; // Configure Data Pin,STCP,SHCP, Unit Digit, Tens Digit pins as O/P
	SIPO &= ~((1 << D_S) | (1 << SHCP) | (1 << STCP));	// Initialize the Shift register pins
}

void shiftout(uint8_t num)
{
	SIPO &= ~(1 << STCP);		// Setup STCP Pulse
	uint8_t counter = 8;		// Initialize counter to 8
	while(counter > 0)
	{
		SIPO &= ~(1 << SHCP);	// Setup SHCP Pulse
		if(((num & 0b10000000) >> 7))  SIPO |= 1 << D_S;    // Set logic 1 on Data Pin if 1 is encountered
		else                           SIPO &= ~(1 << D_S); // Set logic 0 on Data Pin if 0 is encountered
		SIPO |= 1 << SHCP;		// Complete SHCP Pulse
		num = num << 1;			// Shift number to the left by 1 bit to test next bit of number
		counter --;				// Decrement counter
	}
	SIPO |= 1 << STCP;			// Complete STCP Pulse. Latch output to output pins.
}
