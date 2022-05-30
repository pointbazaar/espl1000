#ifndef FLAGS
#define FLAGS

#include <stdbool.h>
#include <inttypes.h>

struct Flags {
	
	bool debug;         //-debug
	bool help;          //-help
	bool version;       //-version
	bool emit_headers;  //-h

    bool has_main_fn;

    uint8_t count_filenames;
    uint8_t capacity_filenames;
    char** filenames;

};

struct Flags* makeFlags(int argc, char** argv);
struct Flags* makeFlags2();

void freeFlags(struct Flags* flags);

#endif
