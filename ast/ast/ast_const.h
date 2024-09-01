#ifndef AST_CONST_H
#define AST_CONST_H

#include <stdbool.h>

#include "../ast_declare.h"

struct ConstValue {
	struct ASTNode super;

	union {
		bool m1_bool_const;
		int32_t m2_int_const;
		char m3_char_const;
		uint32_t m5_hex_const;
		//6 means binconst, saved in hex const
	} ptr;

	uint8_t kind; // 1 .. 6
};

struct StringConst {
	struct ASTNode super;
	char* value;
};

#endif
