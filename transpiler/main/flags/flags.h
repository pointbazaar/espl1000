#ifndef FLAGS
#define FLAGS

#include <stdbool.h>
#include <inttypes.h>

struct Flags {
	
	bool debug;
	bool test;
	bool avr;
	bool help;
	bool version;
	bool clean;
	bool no_typecheck;
	
	bool debug_symbols; //-g
	bool werror; //-Werror
	
	//used to tell if we should run gcc
	bool has_main_fn;
	
	uint8_t count_filenames;
	uint8_t capacity_filenames;
	char** filenames;
};

struct Flags* makeFlags(int argc, char** argv);

void freeFlags(struct Flags* flags);

#endif
