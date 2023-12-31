//parser.h

//header guard
#ifndef __PARSER_AT__
#define __PARSER_AT__

//custom types
#define AT_COMMAND_MAX_LINES 100
#define AT_COMMAND_MAX_LINE_SIZE 128
typedef struct
{
	uint8_t ok;
	uint8_t data[AT_COMMAND_MAX_LINES][AT_COMMAND_MAX_LINE_SIZE + 1];
	uint32_t lineCount;
}AT_COMMAND_DATA; 

typedef enum
{
	STATE_MACHINE_NOT_READY,
	STATE_MACHINE_READY_OK,
	STATE_MACHINE_READY_WITH_ERROR
}STATE_MACHINE_RETURN_VALUE;

//shared variables
extern AT_COMMAND_DATA data;

//function decl
STATE_MACHINE_RETURN_VALUE parse(uint8_t c); 
void printChar(uint8_t c);

#endif