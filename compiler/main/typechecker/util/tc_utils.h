#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "ast/ast_declare.h"

bool is_primitive_type(struct Type* type);

bool is_integer_type(struct Type* type);

bool is_bool_type(struct Type* type);

bool is_char_type(struct Type* type);

bool is_pointer_type(struct Type* type);

uint32_t max_indices_allowed(struct Type* type);
