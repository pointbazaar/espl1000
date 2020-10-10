#ifndef FLAGS
#define FLAGS

#include <stdbool.h>

struct Flags {
	bool debug:1;
	bool test:1;
	bool avr:1;
	bool help:1;
	bool version:1;
	bool clean:1;
	bool stdout:1;
	//----------
	char** gcc_flags;
	int gcc_flags_count;
};

struct Flags* makeFlags(int argc, char** argv);

void freeFlags(struct Flags* flags);

#endif
