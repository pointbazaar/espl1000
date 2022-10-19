#ifndef AST_CONST_H
#define AST_CONST_H

#include "../ast_declare.h"

struct BoolConst   { struct ASTNode super; bool     value; };

struct CharConst   { struct ASTNode super; char     value; };

struct IntConst    { struct ASTNode super; int32_t  value; };

struct HexConst    { struct ASTNode super; uint32_t value; };

struct BinConst    { struct ASTNode super; uint32_t value; };

struct ConstValue {
	struct ASTNode super;

	union {
		struct BoolConst*   m1_bool_const;
		struct IntConst*    m2_int_const;
		struct CharConst*   m3_char_const;
		struct HexConst*    m5_hex_const;
		struct BinConst*    m6_bin_const;
	} ptr;

	uint8_t kind; // 1 .. 6
};

struct StringConst { struct ASTNode super; char*    value; };

#endif
