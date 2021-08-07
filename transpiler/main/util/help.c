#include <stdio.h>


void sd_print_help(){
	
	printf("NAME:     sd - smalldragon transpiler\n\n");
	
	printf("SYNOPSIS: sd [OPTION] FILE...\n\n");
	
	printf("OPTIONS:\n\n");
	
	printf(" -help      display help\n");

	printf(" -debug     print debug statements\n");
	printf(" -version   print Version Information\n");
	printf(" -h         emit .h (header) file\n");
	
	printf(" -g         pass to gcc\n");
	printf(" -Werror    pass to gcc\n");
	
	printf("\n");
	
	printf("EXAMPLES\n\n");
	printf("\tsd main.dg\n");
	printf("\tsd -help\n");
	printf("\n");
	
	printf("WEBSITE:        https://smalldragon.org\n");
	printf("\n");
	
	printf("GITHUB:         https://github.com/pointbazaar/smalldragon\n");
	printf("\n");
	
	printf("AUTHOR:         alex23667@gmail.com\n");
	printf("\n");
	
	printf("REPORTING BUGS: alex23667@gmail.com\n");
}
