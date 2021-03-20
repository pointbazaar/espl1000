#ifndef TC_CTX_H
#define TC_CTX_H

#include <inttypes.h>

struct TCCtx {
	
	struct ST* st;
	struct Method* currentFn;
	
	uint32_t tcErrCount;
};

#endif
