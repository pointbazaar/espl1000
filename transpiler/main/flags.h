#ifndef FLAGS
#define FLAGS

#include <stdbool.h>

//enabel debug output
#define FDEBUG "-debug"

//for testing smalldragon
#define FTEST  "-test"

//for compiling for avr (using 'avr-gcc', without stdlib #includes)
#define FAVR   "-avr"

struct Flags {
	bool debug;
	bool test;
	bool avr;
	char** gcc_flags;
	int gcc_flags_count;
};

struct Flags* makeFlags();

void freeFlags(struct Flags* flags);

#endif
