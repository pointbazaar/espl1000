#ifndef TC_CTX_H
#define TC_CTX_H

#include <inttypes.h>

struct TCCtx {
	
	struct ST* st;
	struct Method* currentFn;
	
	//are we typechecking inside a try-block?
	uint16_t depth_inside_try_stmt;
	uint16_t depth_inside_loop;
	
	uint32_t tcErrCount;
	
	char*    current_filename;
	uint32_t current_line_num;
};

#endif
