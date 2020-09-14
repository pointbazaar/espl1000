#include <stdio.h>


void sd_print_help(){
	
	printf("NAME:     sd - smalldragon transpiler\n\n");
	
	printf("SYNOPSIS: sd [OPTION] FILE\n\n");
	
	printf("OPTIONS:\n\n");
	
	printf(" -help		display help\n");
	printf(" -avr		transpile for AVR MCU's (experimental)\n");
	printf(" -test		run the tests for smalldragon\n");
	printf(" -debug		print debug statements\n");
	printf(" -clean		re-generate all artifacts used for incremental compilation\n");
	printf(" -dumpdot	create a .dot file with the call graph (not yet implemented)\n");
	printf(" -astdot	create a .dot file with the AST tree (not yet implemented)\n");
	printf(" -stdout	print transpiled file to stdout (incompatible with -debug)\n");
	printf(" -version	print Version Information\n");
	
	printf("\n");
	
	printf("All other options will be forwarded to gcc\n");
	
	printf("\n");
	
	printf("EXAMPLES\n\n");
	printf("\tsd -test\n");
	printf("\tsd Test.dg\n");
	printf("\tsd -help\n");
	printf("\n");
	
	printf("GITHUB:         https://github.com/pointbazaar/smalldragon\n");
	printf("\n");
	
	printf("AUTHOR:         alex23667@gmail.com\n");
	printf("\n");
	
	printf("REPORTING BUGS: alex23667@gmail.com\n");
}
