#ifndef PARSE_ASTNODE_H
#define PARSE_ASTNODE_H

struct TokenList;
struct ASTNode;

void parse_astnode(struct TokenList* tknList, struct ASTNode* node);

#endif
