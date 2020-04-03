#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "lcd_4bit.h"
#include "std_types.h"
#include "KeyPad.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define ledpin 0

xSemaphoreHandle button;
xSemaphoreHandle lcd_data;
xSemaphoreHandle idle;

//void vApplicationIdleHook(void);
void pwm(u8 duty);

void task_lcd(void* p);
void task_keypad(void* p);
void screen_saver(void);
void welcome(void);
void blinking_pattern(void);
void dispaly_integer (int n, int n_count);
void calc_operation(void);


int main(void)
{
	lcd_init();
	DDRA = 255;
	xTaskHandle keypadtask;
	xTaskHandle lcdtask;
	xTaskCreate(task_keypad,NULL,100,NULL,2,&keypadtask);
	xTaskCreate(task_lcd,NULL,100,NULL,1,&lcdtask);
	
	vTaskStartScheduler();
	return 0;
}


void task_lcd(void* p)
{
	portTickType x;
	x = xTaskGetTickCount();
	for(;;)
	{
		vTaskDelayUntil(&x,100);
		if(xSemaphoreTake(lcd_data,portMAX_DELAY) == pdTRUE)
		{
			if (button == "C")
				break;
		}
	}
	for(;;)
	{
		vTaskDelayUntil(&x,100);
		if(xSemaphoreTake(lcd_data,portMAX_DELAY) == pdTRUE)
		{
			calc_operation();
			xSemaphoreGive(lcd_data);
		}
	}
}


void welcome(void)
{
	portTickType x;
	x = xTaskGetTickCount();
	unsigned char i;
	for(i = 0;i < 10;i++)
	{
		vTaskDelayUntil(&x,50);
		lcd_clrScreen();
		lcd_disp_string_xy("welcome",0,i);
	}
	for(i = 9;i > 0;i--)
	{
		vTaskDelayUntil(&x,50);
		lcd_clrScreen();
		lcd_disp_string_xy("welcome",0,i);
	}
}


void screen_saver(void)
{
	portTickType x;
	x = xTaskGetTickCount();
	while(1)
	{
		lcd_clrScreen();
		vTaskDelayUntil(&x,250);
		lcd_dispString("Press any Key");
		lcd_disp_string_xy("to Continue",1,0);
		vTaskDelayUntil(&x,500);
	}
}


void task_keypad(void* p)
{
	//xTaskHandle xHandle;
	//xTaskCreate(task_lcd,NULL,100,NULL,1,&xHandle);
	portTickType x;
	x = xTaskGetTickCount();
	unsigned char button_local;
	u8 count;
	lcd_displayChar('h');
	for(;;)
	{
		vTaskDelayUntil(&x,100);
		xSemaphoreTake(button,portMAX_DELAY);
		button_local = KeyPad_u8GetPressedKey();
		if(button_local == 'c')
		{
			xSemaphoreGive(button);
			break;
		}
	}
	
	for(count = 0;count < 3; count++)
	{
		welcome();
	}
	
	count = 0;
	
	screen_saver();
	
	for(;;)
	{
		vTaskDelayUntil(&x,100);
		xSemaphoreTake(button,portMAX_DELAY);
		button_local = KeyPad_u8GetPressedKey();
		if(button_local != NO_KEY_PRESSED)
		{
			if(button != button_local)
			{
				button = button_local;
				count =0;
				xSemaphoreGive(button);
			}
		}
		else
			count++;
		
		if(count == 100)
		{
				lcd_clrScreen();
				blinking_pattern();
		}
			
		PORTA ^= 255;
	}
}


void blinking_pattern(void)
{
	u8 mode = 0;
	portTickType x;
	for(;;)
	{
		lcd_clrScreen();
		lcd_displayChar(mode+48);
		switch(mode)
		{
			case 0:
				for(u8 i = 1;i < 11;i++)
				{
					pwm(i);
					vTaskDelayUntil(&x,25);
				}
				mode = 1;
				break;
			case 1:
				DIO_voidSetPin(ledpin,1);
				vTaskDelayUntil(&x,250);
				mode = 2;
				break;
			case 2:
				for(u8 i = 10;i > 0;i--)
				{
					pwm(i);
					vTaskDelayUntil(&x,25);
				}
				mode = 3;
				break;
			case 3:
				DIO_voidSetPin(ledpin,0);
				mode = 0;
				vTaskDelayUntil(&x,250);
				break;
		}
	}
}

void pwm(u8 duty)
{
	portTickType x;
	for(u8 i = 1;i < 11;i++)
	{
		if(i <= duty)
		{
			DIO_voidSetPin(ledpin,1);
			vTaskDelayUntil(&x,1);
		}
		else if(i > duty)
		{
			DIO_voidSetPin(ledpin,0);
			vTaskDelayUntil(&x,1);
		}
	}
	for(u8 i = 1;i < 11;i++)
	{
		if(i <= duty)
		{
			DIO_voidSetPin(ledpin,1);
			vTaskDelayUntil(&x,1);
		}
		else if(i > duty)
		{
			DIO_voidSetPin(ledpin,0);
			vTaskDelayUntil(&x,1);
		}
	}
	for(u8 i = 1;i < 6;i++)
	{
		if(i <= duty)
		{
			DIO_voidSetPin(ledpin,1);
			vTaskDelayUntil(&x,1);
		}
		else if(i > duty)
		{
			DIO_voidSetPin(ledpin,0);
			vTaskDelayUntil(&x,1);
		}
	}
}


void calc_operation(void)
{
	static u8 op1[16]={0};
	static u8 op1_count = 0;
	static u8 op2[16]={0};
	static u8 op2_count = 0;
	static u8 equation_op=1;
	static u8 op1_complete = 0;
	static int result=0;
	static int op1_int=0;
	static int op2_int=0;
	static u8 current_position = 0;
	
	if (xSemaphoreTake(button , portMAX_DELAY) == pdTRUE)
	{
		if (button <= '9' && button >= '0')
		{
			lcd_gotoxy(0,current_position);
			current_position++;
			lcd_displayChar(button);
			
			if (op1_complete == 0)
			{
				op1[op1_count]=button;
				op1_count++;
				
			}
			else
			{
				op2[op2_count]=button;
				op2_count++;
			}
		}
		else
		{
			if (button == '=')
			{
				op1_int = atoi(op1);
				op2_int = atoi(op2);
				switch (equation_op)
				{
					case '+':
					result = op1_int+op2_int;
					break;
					case '-':
					result = op1_int-op2_int;
					break;
					case '*':
					result = op1_int*op2_int;
					break;
					case '/':
					if(op2_int == 0)
					{
						lcd_disp_string_xy("Math Error",0,0);
						vTaskDelay(500);
						lcd_clrScreen();
						lcd_displayChar('0');
						current_position =0;
						op1_complete = 0;
						op1_count = 0;
						for (int i = 0 ; i< 16 ; i++)
						{
							op1[i] = 0;
							op2[i] = 0;
						}
						op2_count =0;
						result = 0;
					}
					else
					result = op1_int/op2_int;
					break;
				}
				
				lcd_clrScreen();
				dispaly_integer(result,op1_count+op2_count);
				
				for (int i = 0 ; i< 16 ; i++)
				{
					op2[i] = 0;
				}
				op2_count =0;
			}
			else if (button == 'C')
			{
				lcd_clrScreen();
				
				lcd_displayChar('0');
				current_position =0;
				op1_complete = 0;
				op1_count = 0;
				for (int i = 0 ; i< 16 ; i++)
				{
					op1[i] = 0;
					op2[i] = 0;
				}
				op2_count =0;
				result = 0;
			}
			else
			{
				lcd_clrScreen();
				current_position=0;
				equation_op = button;
				op1_complete =1;
			}
			
		}
	}
}


void dispaly_integer (int n, int n_count)
{
	unsigned char str[16];
	for(int count = 0 ; count < n_count ; count++)
	{
		lcd_displayChar((unsigned char)str[count]);
	}

}