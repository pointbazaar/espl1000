#include <stdio.h>


void sd_print_help(){
	
	printf("NAME\n");
	printf("\tsd - smalldragon transpiler\n\n");
	
	printf("SYNOPSIS\n");
	printf("\tsd [OPTION] FILE\n\n");
	
	printf("OPTIONS:\n");
	
	printf("-help		display help\n");
	printf("-avr		transpile for AVR MCU's (experimental)\n");
	printf("-test		run the tests for smalldragon\n");
	printf("-debug		print debug statements\n");
	printf("All other options will be forwarded to gcc\n");
	
	printf("\n");
	
	printf("EXAMPLES\n\n");
	printf("\tsd -test\n");
	printf("\tsd -avr Test.dg\n");
	printf("\tsd -help\n");
	printf("\n");
	
	printf("GITHUB:         https://github.com/pointbazaar/smalldragon\n");
	printf("\n");
	
	printf("AUTHOR:         alex23667@gmail.com\n");
	printf("\n");
	
	printf("REPORTING BUGS: alex23667@gmail.com\n");
}
