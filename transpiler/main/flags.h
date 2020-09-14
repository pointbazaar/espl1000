#ifndef FLAGS
#define FLAGS

#include <stdbool.h>

struct Flags {
	bool debug;
	bool test;
	bool avr;
	bool help;
	bool version;
	bool clean;
	bool stdout;
	//----------
	char** gcc_flags;
	int gcc_flags_count;
};

struct Flags* makeFlags(int argc, char** argv);

void freeFlags(struct Flags* flags);

#endif
