#ifndef STR_AST_H
#define STR_AST_H

#include "../ast.h"

//this header file declares all subroutines
//for providing string representations
//of AST nodes. Meaning to convert them
//to their source code form.

//all char* returned are heap-allocated

char* strBoolConst(struct BoolConst* bc);	

char* strCharConst(struct CharConst* cc);

char* strFloatConst(struct FloatConst* fc);

char* strIdentifier(struct Identifier* id);

char* strIntConst(struct IntConst* ic);

char* strHexConst(struct HexConst* hc);

char* strArrayType(struct ArrayType* at);

char* strBasicTypeWrapped(struct BasicTypeWrapped* btw);

char* strSimpleType(struct SimpleType* st);

char* strSubrType(struct SubrType* st);

char* strType(struct Type* t);

char* strTypeParam(struct TypeParam* t);

#endif
