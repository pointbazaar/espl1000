#pragma once

#include <stdbool.h>

struct TokenList;
struct AddressOf;
struct Term;

struct AddressOf* makeAddresOf(struct Term* term);
struct AddressOf* makeAddressOf2(struct TokenList* tokens);
