#ifndef FLAGS
#define FLAGS

#include <stdbool.h>

//enabel debug output
#define FDEBUG "-debug"

//for testing smalldragon
#define FTEST  "-test"

//for displaying help
#define FHELP  "-help"

//for compiling for avr (using 'avr-gcc', without stdlib #includes)
#define FAVR   "-avr"

struct Flags {
	bool debug;
	bool test;
	bool avr;
	bool help;
	char** gcc_flags;
	int gcc_flags_count;
};

struct Flags* makeFlags(int argc, char** argv);

void freeFlags(struct Flags* flags);

#endif
