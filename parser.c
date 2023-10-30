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
			if(crt_char == 13) { // CR
				state = 1;
				data.line_count = 0;
				col_cnt = 0;
			}
			break;
			
		case 1:
			if(crt_char == 10) { // LF
				state = 2;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}		
			break;
			
		case 2:
			if(crt_char == 'O') {
				state = 3;
			}
			else if(crt_char == 'E') {
				state = 7;
			}
			else if(crt_char == '+') {
				state = 14;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
		
		case 3:
			if(crt_char == 'K') {
				state = 4;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 4:
			if(crt_char == '13') {
				state = 5;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 5:
			if(crt_char == '10') {
				state = 6;
				return STATE_MACHINE_READY_OK;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 7:
			if(crt_char == 'R') {
				state = 8;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
		
		case 8:
			if(crt_char == 'R') {
				state = 9;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 9:
			if(crt_char == 'O') {
				state = 10;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 10:
			if(crt_char == 'R') {
				state = 11;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 11:
			if(crt_char == 13) {
				state = 12;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 12:
			if(crt_char == 10) {
				state = 13;
				return STATE_MACHINE_READY_OK;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
		
		case 14:
			if ((32 <= crt_char) && (crt_char <= 126)) {
				if(col_cnt < 256) {
					data.data[data.line_count][col_cnt] = crt_char;
					col_cnt++;
				}
				state = 15;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 15:
			if ((32 <= crt_char) && (crt_char <= 126)) {
				if(col_cnt < 256) {
					data.data[data.line_count][col_cnt] = crt_char;
					col_cnt++;
				}
				state = 15;
			}
			else if( crt_char == 13) {	// CR
				data.data[data.line_count][col_cnt] = '/0';
				data.line_count++;
				col_cnt = 0;
				state = 16;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 16:
			if(crt_char == 10) { // LF
				state = 17;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}		
			break;
			
		case 17:
			if(crt_char == '+') {
				state = 14;
			}
			else if( crt_char == 13) {
				state = 18;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 18:
			if(crt_char == 10) { // LF
				state = 19;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}		
			break;
			
		case 19:
			if(crt_char == 'O') {
				state = 3;
			}
			else if( crt_char == 'E') {
				state = 7;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
	}
	return STATE_MACHINE_NOT_READY;
}





































