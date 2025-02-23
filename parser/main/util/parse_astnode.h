#pragma once

struct TokenList;
struct ASTNode;

void parse_astnode(struct TokenList* tknList, struct ASTNode* node);
