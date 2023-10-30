//main.c

#include <stdio.h>
#include <stdint.h>
#include "parser.h"

int main( int argc, char *argv[] )  {

	if( argc != 2 ) {
		printf("Usage: parser.exe test.txt\n");
		return -1;
	}
	
	FILE *f = fopen(argv[1], "r");
	if(f == NULL) {
		printf("Not able to open the file.\n");
		return -1;
	}

	uint8_t c = getc(f);
	while(c != EOF) {
			printChar(c);
		STATE_MACHINE_RETURN_VALUE result = parse(c);
		if(result == STATE_MACHINE_READY_OK) {
			printf("\nNo errors\n");
			return 0;
		}
		if(result == STATE_MACHINE_READY_WITH_ERROR){
			printf("\nError!\n");
			break;
		}
		c = getc(f);
	}
	fclose(f);
	return 0;
}
