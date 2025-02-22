#pragma once

struct TokenList;
struct StringConst;

// @returns NULL on error
struct StringConst* makeStringConst(struct TokenList* tokens);
