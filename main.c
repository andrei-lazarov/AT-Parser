//main.c

#include <stdio.h>
#include <stdint.h>
#include "parser.h"

int main( int argc, char *argv[] )  {

	if( argc != 2 ) {
		printf("Usage: parser.exe test.txt\n");
		return -1;
	}
	
	FILE *f = fopen(argv[1], "rb");
	if(f == NULL) {
		printf("Not able to open the file.\n");
		return -1;
	}

	printf("================ Parsing started ================\n");
	int c = getc(f);
	while(c != EOF) {
		printChar(c);
		STATE_MACHINE_RETURN_VALUE result = parse(c);
		if(result == STATE_MACHINE_READY_OK) {
			printf("\n======== Parsing finished with no errors ========\n");
			printf(" > Press ENTER to parse the next command\n");
			getchar();
		}
		if(result == STATE_MACHINE_READY_WITH_ERROR){
			printf("\n=========== Parsing stopped by error ============\n");
			break;
		}
		c = getc(f);
	}
	fclose(f);
	
	printf("   No more commands to parse.\n");
	printf(" > Press ENTER to print the stored data\n");
	getchar();
	printf("============= Printing stored data ==============\n");
			for(int i = 0; i < data.lineCount ; i++) {
				for(int j=0; j < AT_COMMAND_MAX_LINE_SIZE; j++){
					putchar(data.data[i][j]);
				}
				printf("\n");
			}
	printf("============== Printing finished ================\n");
	return 0;
}
