#include "DIO_priv.h"
#include "DIO_config.h"
#include "DIO_int.h"
#include "KeyPad.h"


/***  checks for the number of the  button pressed  ***/
unsigned char KeyPad_u8GetPressedKey(void)
{
    unsigned char col,row,i=0;
	while(i < 210)
	{
		for(col=0; col < N_col; col++) /* loop for columns */
		{

			/*
				* Each time only one of the column pins will be output and
			* the rest will be input pins including the row pins
			*/

			DIO_voidSetPortDir(KEYPAD_PORT, (0b00010000 << col));
			/*
			* clear the output pin column and enable the internal
			* pull up resistors for the other pins
			*/
			
			DIO_voidSetPort(KEYPAD_PORT, (~(0b00010000 << col)));
			for(row=0; row<N_row; row++) /* loop for rows */
			{
				if(DIO_u8GetPinInPort(KEYPAD_PORT, row) == DIO_LOW) /* if the switch is pressed in this row */
				{
					i++;
					if(i > 200)
					{
						return KeyPad_u8AdjustKeyNumber((row*N_col)+col+1);
					}
				}
			}
		}
	}

    return NO_KEY_PRESSED;
}

/***  decodes the putton pressed from the matrix which one is it on the keypad  ***/

#if KEYOAD_ORIENTATION == FOURxTHREE
unsigned char KeyPad_u8AdjustKeyNumber(u8 button_number)
{
    switch(button_number)
	{
	    case 1: return '1';
				break;
		case 2: return '2';
				break;
		case 3: return '3';
				break;
		case 4: return '4';
				break;
		case 5: return '5';
				break;
		case 6: return '6';
				break;
		case 7: return '7';
				break;
		case 8: return '8'
				break;
		case 9: return '9';
				break;
		case 10: return '*';
				 break;
		case 11: return '0';
				 break;
		case 12: return '#';
				 break;
		default: return NO_KEY_PRESSED;
	}

}
#elif KEYPAD_ORIENTATION == FOURxFOUR
unsigned char KeyPad_u8AdjustKeyNumber(u8 button_number)
{
    switch(button_number)
	{
		case 1: return '7';
				break;
		case 2: return '8';
				break;
		case 3: return '9';
				break;
		case 4: return '÷'; 
				break;
		case 5: return '4';
				break;
		case 6: return '5';
				break;
		case 7: return '6';
				break;
		case 8: return 'X'; 
				break;
		case 9: return '1';
				break;
		case 10: return '2';
				break;
		case 11: return '3';
				break;
		case 12: return '-'; 
				break;
		case 13: return 'C';  
				break;
		case 14: return '0';
				break;
		case 15: return '='; 
				break;
		case 16: return '+'; 
				break;
		default: return NO_KEY_PRESSED;
	}

}
#endif



