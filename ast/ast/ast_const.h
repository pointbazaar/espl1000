#ifndef AST_CONST_H
#define AST_CONST_H

#include "../ast_declare.h"

struct BoolConst   { struct ASTNode super; bool     value; };

struct CharConst   { struct ASTNode super; char     value; };

struct FloatConst  { struct ASTNode super; float    value; };

struct IntConst    { struct ASTNode super; int32_t  value; };

struct HexConst    { struct ASTNode super; uint32_t value; };

struct BinConst    { struct ASTNode super; uint32_t value; };

struct StringConst { struct ASTNode super; char*    value; };

#endif
