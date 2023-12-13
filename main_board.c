#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "LPC177x_8x.h"
#include "system_LPC177x_8x.h"
#include <retarget.h>
#include "parser.h"

#include <DRV\drv_sdram.h>
#include <DRV\drv_lcd.h>
#include <DRV\drv_uart.h>
#include <DRV\drv_touchscreen.h>
#include <DRV\drv_led.h>
#include <utils\timer_software.h>
#include <utils\timer_software_init.h>

const char at_command_simple[] = "AT\r\n";
const char at_command_csq[] = "AT+CSQ\r\n";
const char at_command_creg[] = "AT+CREG\r\n";
const char at_command_cops[] = "AT+COPS\r\n";
const char at_command_gsn[] = "AT+GSN\r\n";
const char at_command_gmi[] = "AT+GMI\r\n";
const char at_command_gmr[] = "AT+GMR\r\n";
STATE_MACHINE_RETURN_VALUE StateMachineReturn;
timer_software_handler_t my_timer_handler; 
timer_software_handler_t my_handler;

void timer_callback_1(timer_software_handler_t h)
{
}

void testLCD()
{
	uint32_t i,j;
	LCD_PIXEL foreground = {0, 255, 0, 0};
	LCD_PIXEL background = {0, 0, 0, 0};
	
	
	for (i = 0; i < LCD_HEIGHT; i++)
	{
		for (j = 0; j < LCD_WIDTH / 3; j++)
		{
			DRV_LCD_PutPixel(i, j, 255, 0, 0);
		}
		for (j = LCD_WIDTH / 3; j < 2 * (LCD_WIDTH / 3); j++)
		{
			DRV_LCD_PutPixel(i, j, 230, 220, 0);
		}
		for (j = 2 * (LCD_WIDTH / 3); j < LCD_WIDTH; j++)
		{
			DRV_LCD_PutPixel(i, j, 0, 0, 255);
		}
	}

	DRV_LCD_Puts("Hello", 20, 30, foreground, background, TRUE);
	DRV_LCD_Puts("Hello", 20, 60, foreground, background, FALSE);	
}

void TouchScreenCallBack(TouchResult* touchData)
{
	printf("touched X=%3d Y=%3d\n", touchData->X, touchData->Y);		
	
}

void BoardInit()
{
	
	
	timer_software_handler_t handler;
	
	TIMER_SOFTWARE_init_system();
	
	
	DRV_SDRAM_Init();
	
	initRetargetDebugSystem();
	DRV_LCD_Init();
	DRV_LCD_ClrScr();
	DRV_LCD_PowerOn();	
	
	DRV_TOUCHSCREEN_Init();
	DRV_TOUCHSCREEN_SetTouchCallback(TouchScreenCallBack);
	DRV_LED_Init();
	
	handler = TIMER_SOFTWARE_request_timer();
	TIMER_SOFTWARE_configure_timer(handler, MODE_1, 100, 1);
	TIMER_SOFTWARE_set_callback(handler, timer_callback_1);
	TIMER_SOFTWARE_start_timer(handler);
}

void SendCommand(const char command[])
{
 DRV_UART_FlushRX(UART_3);
 DRV_UART_FlushTX(UART_3);
 DRV_UART_Write(UART_3, (uint8_t*) command, strlen(command));
}

void GetCommandResponse()
{
 uint8_t ch;
 BOOLEAN ready = FALSE;
 TIMER_SOFTWARE_reset_timer(my_handler);
 TIMER_SOFTWARE_start_timer(my_handler);
	
 data.lineCount = 0;
	
 while ((!TIMER_SOFTWARE_interrupt_pending(my_handler)) && (ready == FALSE))
 {
	while (DRV_UART_BytesAvailable(UART_3) > 0)
	{
	 DRV_UART_ReadByte(UART_3, &ch);
	 printf("%c", ch);
	 StateMachineReturn = parse(ch);
	 if ( StateMachineReturn != STATE_MACHINE_NOT_READY)
	 {
		ready = TRUE;
	 }
	}
 }
}

void ExecuteCommand(const char *command)
{
 SendCommand(command);
 GetCommandResponse();
}

uint32_t ExtractData ()
{
	uint32_t result= 0;
	//while(data.data[0][i]!= '\0')
	//{
	//	putchar(data.data[0][i]);
	//	i++;
	//}
	result = 10* (data.data[0][5] - '0') + (data.data[0][6] - '0');
	return result;
}


uint32_t ConvertAsuToDbmw (uint32_t asu )
{
	return 2 * asu - 113;
}

int CommandResponseValid()
{
	// use STATE MACHINE READY
	if(StateMachineReturn == STATE_MACHINE_READY_OK)
		return 1;
	return 0;
}


int main(void)
{
	
	uint32_t rssi_value_asu;
	uint32_t rssi_value_dbmw; 

	uint8_t ch;

	BoardInit();
	testLCD();
	
	DRV_UART_Configure(UART_3, UART_CHARACTER_LENGTH_8, 115200, UART_PARITY_NO_PARITY, 1, TRUE, 3);
	DRV_UART_Configure(UART_0, UART_CHARACTER_LENGTH_8, 115200, UART_PARITY_NO_PARITY, 1, TRUE, 3);
	
	my_timer_handler = TIMER_SOFTWARE_request_timer();
	TIMER_SOFTWARE_configure_timer(my_timer_handler, MODE_1, 5000, 1);
	TIMER_SOFTWARE_start_timer(my_timer_handler);
	
	printf ("=========== Program started =========== \n");
	
	SendCommand(at_command_simple);
	
	while(1)
	{
		if (TIMER_SOFTWARE_interrupt_pending(my_timer_handler))
		 {
			 
				 ExecuteCommand(at_command_csq);
				 if (CommandResponseValid())
				 {
					 
					 rssi_value_asu = ExtractData();
					 rssi_value_dbmw = ConvertAsuToDbmw(rssi_value_asu);
					 
					 printf("\nGSM Modem signal %u ASU -> %d dBmW\n\n",rssi_value_asu, rssi_value_dbmw);
				 }
			 
			 ExecuteCommand(at_command_csq);
				 TIMER_SOFTWARE_clear_interrupt(my_timer_handler);
		 } 
	}
	/*
	while(1)
	{
		DRV_UART_SendByte(UART_3, 'A');
	//	TIMER_SOFTWARE_Wait(1000);
	}
	*/
	/*
	while(1)
	{
		if (DRV_UART_ReadByte(UART_3, &ch) == OK)
		{
			DRV_UART_SendByte(UART_3, ch);
		}		
	}
*/
	while(1)
	{
		if (DRV_UART_ReadByte(UART_0, &ch) == OK)
		{
			DRV_UART_SendByte(UART_3, ch);
		}
		if (DRV_UART_ReadByte(UART_3, &ch) == OK)
		{
			DRV_UART_SendByte(UART_0, ch);
		}
		if (DRV_UART_ReadByte(UART_2, &ch) == OK)
		{
			DRV_UART_SendByte(UART_0, ch);
		}
	}
	
	while(1)
	{
		DRV_UART_Process();
		DRV_TOUCHSCREEN_Process();
	}
	
	return 0; 
}
