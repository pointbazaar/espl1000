#ifndef FLAGS
#define FLAGS

#include <stdbool.h>
#include <inttypes.h>

struct Flags {
	
	bool debug;         //-debug
	bool test;          //-test
	bool help;          //-help
	bool version;       //-version
	bool emit_headers;  //-h
	
	//gcc passthrough flags
	bool debug_symbols; //-g
	bool werror;        //-Werror
	
	//used to tell if we should run gcc
	bool has_main_fn;
	
	uint8_t count_filenames;
	uint8_t capacity_filenames;
	char** filenames;
};

struct Flags* makeFlags(int argc, char** argv);
struct Flags* makeFlags2();

void freeFlags(struct Flags* flags);

#endif
