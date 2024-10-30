#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "LCD.h"

#define KEYPAD_PORT PORTA
#define KEYPAD_PIN  PINA
#define KEYPAD_DDR DDRA
#define KEYPAD_ROW 0xF0
#define KEYPAD_COL 0x0F

// Hardware Keypad
/*U8 keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};*/

// Simulator Keypad
U8 keypad[4][4] = {
	{'7', '8', '9', '/'},
	{'4', '5', '6', '*'},
	{'1', '2', '3', '-'},
	{'A', '0', '=', '+'}
};

U8 KEYPAD_READ();

// Function to read input from keypad
U8 KEYPAD_READ()
{
	while(1)
	{
		for(char row = 0; row < 4; row++)
		{
			KEYPAD_DDR = KEYPAD_ROW;
			KEYPAD_PORT = ~(1 << row + 4);
			for(char col = 0; col < 4; col++)
			{
				if(!(KEYPAD_PIN & (1 << col)))
				_delay_ms(5);
				if(!(KEYPAD_PIN & (1 << col)))
				{
					while(!(KEYPAD_PIN & (1 << col)));
					return keypad[row][col];
				}
			}
		}
	}
	return 0;
}
						  

#endif /* KEYPAD_H_ */