#include "keypad.h"
#include "string.h"
/* 
================Users================
			Ahmed(1212, 1234)
			Morad(2020, 4758)
			Hesham(8778, 6431)
*/

#define OWNER_NUM 3
#define DC_ENA 2
#define DC_IN0 3
#define DC_IN1 4

//Home owner IDs
U8 ID_DB [OWNER_NUM][5] = {"1212", "2020", "8778"};

// Home owner passwords
U8 PASSWORD_DB [OWNER_NUM][5] = {"1234", "4758", "6431"};
	
// Home owner names
U8 NAME_DB [OWNER_NUM][16] = {"     Ahmed", "     Morad", "     Hesham"};

void input(U8*, U8);

int main(void)
{
    U8 INPUT[5] = {0};	// Character array to hold user input	
	U8 try_counter = 0;	// Count number of tries
	U8 ID_flag = 0;		// Set if correct ID
	U8 i = 0;
	LCD_init();
	DDRB = 0x1F;		// Set port B output for LEDs and DC motor(PINB0 --> PINB4)
	
    while (1) 
    {
		PORTB = 0x00;		// Turn off all outputs
		LCD_COMMAND(0x01);	// Clear LCD display
		LCD_COMMAND(0x80);	// Start at first line
		
		// More than 3 tries
		if(try_counter == 3)
		{
			LCD_DATA_STRING("  House Access  ");
			LCD_COMMAND(0xC0);
			LCD_DATA_STRING("    Blocked    ");
			PORTB |= (1 << 0);	// Turn on red LED
			while(1);
		}
		
		LCD_DATA_STRING("ID: ");
		input(INPUT, 1);
		ID_flag = 0;
		
		// Check input ID
		for(i = 0; i < OWNER_NUM; i++)
		{
			if(!(strcmp(ID_DB[i], INPUT)))
			{
				// Correct ID --> input password
				ID_flag = 1;
				LCD_COMMAND(0xC0);							// New line in LCD
				LCD_DATA_STRING("Password: ");
				input(INPUT, 0);
				// Check password
				if(!(strcmp(PASSWORD_DB[i], INPUT)))
				{
					LCD_COMMAND(0x01);						// Clear LCD display
					LCD_COMMAND(0x80);						// Start at first line
					LCD_DATA_STRING("  Welcome Home  ");
					LCD_COMMAND(0xC0);
					LCD_DATA_STRING(NAME_DB[i]);
					PORTB |= (1 << 1);						// Turn on green LED
					// Rotate DC motor clockwise
					PORTB |= (1 << DC_ENA);
					PORTB |= (1 << DC_IN1);
					_delay_ms(4000);
					PORTB &= ~(1 << DC_ENA);				// Stop motor
					LCD_COMMAND(0x01);						// Clear LCD display
					LCD_COMMAND(0x80);						// Start at first line
					LCD_DATA_STRING(" Door is opened ");
					while(1);
				}
				else
				{
					try_counter++;
					LCD_COMMAND(0x01);	// Clear LCD display
					LCD_COMMAND(0x80);	// Start at first line
					LCD_DATA_STRING(" Wrong Password ");
					PORTB |= (1 << 0);	// Turn on red LED
					_delay_ms(2000);
					break;
				}
			}
		}
		
		if((!ID_flag) && (i == 3))
		{
			try_counter++;
			LCD_COMMAND(0x01);	// Clear LCD display
			LCD_COMMAND(0x80);	// Start at first line
			LCD_DATA_STRING("    Wrong ID   ");
			PORTB |= (1 << 0);	// Turn on red LED
			_delay_ms(2000);
		}
    }
}

// Function to take input from user
void input(U8* str, U8 display)
{
	for(char i = 0; i < 4; i++)
	{
		str[i] = KEYPAD_READ();		// Get input from keypad
		if(str[i] == '-' && i > 0)	// Delete for simulation
//		if(str[i] == 'D' && i > 0)	// Delete for Hardware
		{
			i-= 2;
			LCD_COMMAND(0x10);	// Shift cursor to the left by one;
			LCD_DATA(' ');
			LCD_COMMAND(0x10);	// Shift cursor to the left by one;
			continue;
		}
		// Don't store input other than numbers
		if(!(str[i] >= '0' && str[i] <= '9'))
		{
			i--;
			continue;
		}
		// check if inputing password don't display the numbers
		if(display)
			LCD_DATA(str[i]);
		else
			LCD_DATA('*');
	}
}


