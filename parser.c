//parser.c

#include "parser.h"


AT_COMMAND_DATA data;

//function definition
STATE_MACHINE_RETURN_VALUE at_command_parse(uint8_t crt_char)
{
	static uint8_t state=0;
	static uint8_t col_cnt = 0;
	
	switch(state)
	{
		case 0:
			if(crt_char == 13) // CR
			{
				state = 1;
				data.line_count = 0; //reset data here ->for/function
				col_cnt = 0;
			}
			break;
			
		case 1:
			if(crt_char == 10) // LF
			{
				state = 2;
				//return not-ready 
			}
			else
			{
				 return STATE_MACHINE_READY_WITH_ERROR;
			}		
			break;
			
		case 2:
			if(crt_char == '0') // LF
			{
				state = 2;
			}
			else if(crt_char == 'E') // LF
			{
				state = 7;
			}
			else if(crt_char == '+') // LF
			{
				state = 20;
			}
			else
			{
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			
			break;
		
		
		...
		
		case 22:
			if(crt_char >= 32 && crt_char <= 126)
			{
				if(col_cnt < ???)
				{
					data.data[data.line_count][col_cnt] = crt_char;
					col_cnt++;
				}
			}
			else if( crt_char == 13) //crt_char
			{
				//increase line count or wait for +
				col_cnt = 0;
				//counter update - line width / set terminator 
			}
			else
			{
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			
			break;
		
	}
}





































