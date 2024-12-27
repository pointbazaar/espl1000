#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "rat.h"

enum RAT_REG_STATUS {
	// register belongs to different architecture
	// that is not the arch this RAT belongs to.
	REG_INVALID_ARCH,
	REG_OCCUPIED, //reg occupied by temporary
	REG_RESERVED, //reg reserved, cannot be allocated
	REG_FREE, //reg can be allocated
};

struct RAT {
	//struct RAT should be opaque outside of it's
	//implementation file

	//Register Allocation Table
	//allocation of the registors used inside a function,
	//mapping temporaries to actual registers

	char* note[SD_REGISTER_END];

	//who occupies it (which temp)
	//uint32_t occupant[RAT_CAPACITY_MAXIMUM];

	// which temporaries are occupying the register.
	// occupant[r8][3] == true
	// means that t3 is stored in r8
	// occupant[r8][4] == true
	// means that t4 is also stored in r8
	bool* occupant[SD_REGISTER_END];

	// number of temporaries
	// that this RAT keeps track of
	size_t ntemps;

	enum RAT_REG_STATUS status[SD_REGISTER_END];

#undef RAT_CAPACITY_MAXIMUM

	//X: r26/r27
	//Y: r28/r29
	//Z: r30/r31

	// which machine architecture is this table for
	enum RAT_ARCH arch;
};
