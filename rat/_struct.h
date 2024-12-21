#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "rat.h"

enum RAT_REG_STATUS {
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

// only used to allocate the rat.
// the implementation should use rat_capacity(rat)
// which respects the target architecture
#define RAT_CAPACITY_MAXIMUM 32

	char* note[RAT_CAPACITY_MAXIMUM];

	//who occupies it (which temp)
	//uint32_t occupant[RAT_CAPACITY_MAXIMUM];

	// which temporaries are occupying the register.
	// occupant[r8][3] == true
	// means that t3 is stored in r8
	// occupant[r8][4] == true
	// means that t4 is also stored in r8
	bool* occupant[RAT_CAPACITY_MAXIMUM];

	// number of temporaries
	// that this RAT keeps track of
	size_t ntemps;

	enum RAT_REG_STATUS status[RAT_CAPACITY_MAXIMUM];

#undef RAT_CAPACITY_MAXIMUM

	//X: r26/r27
	//Y: r28/r29
	//Z: r30/r31

	// which machine architecture is this table for
	enum RAT_ARCH arch;
};
