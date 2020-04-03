#ifndef KEYPAD_INT_H_
#define KEYPAD_INT_H_

#define FOURxFOUR  16
#define FOURxTHREE 12

#define NO_KEY_PRESSED 255

/***  ORIENTATION OF KEYPAD  
 * FOURxTHREE
 * FOURxFOUR
 ***/
#define KEYPAD_ORIENTATION  FOURxFOUR

#if KEYOAD_ORIENTATION == FOURxTHREE
#define N_row 4
#define N_col 3
#elif KEYPAD_ORIENTATION == FOURxFOUR
#define N_row 4
#define N_col 4
#endif

/* Keypad Port Configuration */
#define KEYPAD_PORT DIO_PORTC

unsigned char KeyPad_u8GetPressedKey(void);
unsigned char KeyPad_u8AdjustKeyNumber(unsigned char button_number);

#endif
