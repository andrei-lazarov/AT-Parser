//main.c

#include <stdio.h>
#include <stdint.h>

int main( int argc, char *argv[] )  {

	if( argc != 2 ) {
		printf("Usage: test_program_executable <test_file>\n");
		return;
	}
	
	FILE *f = fopen(argv[1], "r");
	if(fptr == NULL) {
		printf("Not able to open the file.");
	}

	int c = getc(f);
	while(c != EOF) {
		c = getc(f);
		STATE_MACHINE_RETURN_VALUE result = parser(c);
		if(result == STATE_MACHINE_READY_OK)
			putc(c);
		if(result == STATE_MACHINE_READY_WITH_ERROR){
			printf("error\n")
			c = EOF;
		}
	}
	fclose(f);
	return 0;
}
