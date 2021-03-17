#ifndef TYPECHECKING_UTILS_H
#define TYPECHECKING_UTILS_H

#include <stdbool.h>

bool is_primitive_type(struct Type* type);

bool is_integer_type(struct Type* type);

bool is_bool_type(struct Type* type);

bool is_char_type(struct Type* type);

bool is_malloc(struct Expr* expr);

#endif
