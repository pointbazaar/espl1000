#pragma once

#include <stdbool.h>

#include "ast/ast_declare.h"

struct TokenList;

struct Type* makeType2(struct TokenList* tokens);
struct Type* makeType_1(struct BasicType* typeNode);
struct Type* makeType_2(struct TypeParam* typeNode);
struct Type* makeType_3(struct ArrayType* typeNode);
struct Type* makeType_4(struct SimpleType* typeNode);
