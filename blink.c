#include <msp430.h>				
#include <stdint.h>

static const uint8_t SMILEY_FACE[8][8] =
{
 {0, 0, 1, 1, 1, 1, 0, 0},
 {0, 1, 0, 0, 0, 0, 1, 0},
 {1, 0, 1, 0, 0, 1, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 1, 0, 0, 1, 0, 1},
 {1, 0, 0, 1, 1, 0, 0, 1},
 {0, 1, 0, 0, 0, 0, 1, 0},
 {0, 0, 1, 1, 1, 1, 1, 0}
};

static const uint8_t SAD_FACE[8][8] =
{
 {0, 0, 1, 1, 1, 1, 0, 0},
 {0, 1, 0, 0, 0, 0, 1, 0},
 {1, 0, 1, 0, 0, 1, 0, 1},
 {1, 0, 0, 0, 0, 0, 0, 1},
 {1, 0, 0, 1, 1, 0, 0, 1},
 {1, 0, 1, 0, 0, 1, 0, 1},
 {0, 1, 0, 0, 0, 0, 1, 0},
 {0, 0, 1, 1, 1, 1, 1, 0}
};

void main(void)
{
	WDTCTL = WDTPW | WDTHOLD;		// stop watchdog timer
	P3DIR |= 0b11111111; // all row pins as outputs
	P6DIR |= 0b11111111; // all column pins as outputs
	P6OUT  = 0b00000001; // rows
	P2REN |= 0b00000010; // enable pull-up/down resistor for button
	P2OUT |= 0b00000010; // set resistor as pull-up not pull-down

	volatile unsigned int i;		// volatile to prevent optimization

	unsigned int row = 0;
	while(1)
	{
	    const uint8_t button_pressed = P2IN & 0b00000010; // read button

	    uint8_t value = 0;
	    for (int col = 0; col < 8; col++)
	    {
	        value |= (button_pressed ? SAD_FACE : SMILEY_FACE)[row][col] << col;
	    }
	    value = ~value;

	    P6OUT = 0; // turn off all rows
	    P3OUT = value; // turn on columns
	    P6OUT = 1 << row; // turn on a single row

	    for(i=100; i>0; i--);     // delay
	    /*P6OUT  = 0b00000001; // rows
	    P3OUT  = 0b11111110; // cols
		for(i=10000; i>0; i--);     // delay
		P3OUT  = 0b11111111; // cols
		P6OUT  = 0b00000010; // rows
		P3OUT  = 0b11111110; // cols
		for(i=10000; i>0; i--);     // delay*/
	    row = row + 1;
	    if (row > 7)
	        row = 0;
	}
}
