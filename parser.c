//parser.c

#include <stdio.h>
#include <stdint.h>
#include "parser.h"

AT_COMMAND_DATA data;

//function definition
STATE_MACHINE_RETURN_VALUE parse(uint8_t c)
{
	static uint8_t state = 0;
	static uint8_t colCount = 0;
	
	switch(state)
	{
		case 0:
			if(c == '\r') { // CR
				colCount = 0;
				state = 1;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 1:
			if(c == '\n') { // LF
				data.lineCount++;
				colCount = 0;
				state = 2;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}		
			break;
			
		case 2:
			if(c == 'O') {
				state = 3;
			}
			else if(c == 'E') {
				state = 7;
			}
			else if(c == '+') {
				state = 14;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
		
		case 3:
			if(c == 'K') {
				state = 4;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 4:
			if(c == '\r') {
				state = 5;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 5:
			if(c == '\n') {
				data.lineCount++;
				colCount = 0;
				state = 0;
				return STATE_MACHINE_READY_OK;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 6:
			break;

		case 7:
			if(c == 'R') {
				state = 8;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
		
		case 8:
			if(c == 'R') {
				state = 9;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 9:
			if(c == 'O') {
				state = 10;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 10:
			if(c == 'R') {
				state = 11;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 11:
			if(c == '\r') {
				state = 12;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 12:
			if(c == '\n') {
				data.lineCount++;
				colCount = 0;
				state = 0;
				return STATE_MACHINE_READY_OK;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
		
		case 13:
			break;

		case 14:
			if ((32 <= c) && (c <= 126)) {
				if(data.lineCount < AT_COMMAND_MAX_LINES) {
					if(colCount < AT_COMMAND_MAX_LINE_SIZE - 1) {
						data.data[data.lineCount][colCount] = c;
						colCount++;
					}
				}
				state = 15;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 15:
			if ((32 <= c) && (c <= 126)) {
				if(data.lineCount < AT_COMMAND_MAX_LINES){
					if(colCount < AT_COMMAND_MAX_LINE_SIZE - 1) {
						data.data[data.lineCount][colCount] = c;
						colCount++;
					}
				}
				state = 15;
			}
			else if( c == '\r') {	// CR
				if(data.lineCount < AT_COMMAND_MAX_LINES){
					data.data[data.lineCount][colCount] = '\0';
					data.lineCount++;
					colCount = 0;
				}
				state = 16;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 16:
			if(c == '\n') { // LF
				state = 17;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}		
			break;
			
		case 17:
			if(c == '+') {
				state = 14;
			}
			else if( c == '\r') {
				state = 18;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
			
		case 18:
			if(c == '\n') { // LF
				state = 19;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}		
			break;
			
		case 19:
			if(c == 'O') {
				state = 3;
			}
			else if( c == 'E') {
				state = 7;
			}
			else {
				return STATE_MACHINE_READY_WITH_ERROR;
			}
			break;
	}
	colCount++;
	return STATE_MACHINE_NOT_READY;
}

void printChar(uint8_t c)
{
	if (c=='\r') {
		printf("<CR>");
		return;
	}
	if (c=='\n') {
		printf("<LF>\n");
		return;
	}
	putchar(c);
}
