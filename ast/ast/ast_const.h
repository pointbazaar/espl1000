#ifndef AST_CONST_H
#define AST_CONST_H

#include "../ast_declare.h"

struct BoolConst   { bool     value; };

struct CharConst   { char     value; };

struct FloatConst  { float    value; };

struct IntConst    { int32_t  value; };

struct HexConst    { uint32_t value; };

struct BinConst    { uint32_t value; };

struct StringConst { char*    value; };

#endif
