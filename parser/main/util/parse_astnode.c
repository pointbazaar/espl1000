#include "ast/ast_declare.h"

#include "parse_astnode.h"

#include "token/list/TokenList.h"
#include "token/TokenKeys.h"
#include "token/token/token.h"

void parse_astnode(struct TokenList* tknList, struct ASTNode* node) {

	node->line_num = list_head(tknList)->line_num;
	node->annotations = 0;

	struct Token* tk = list_head(tknList);

	while (
	    list_size(tknList) > 1 && tk->kind > _ANNOT_START_ && tk->kind < _ANNOT_END_) {
		node->annotations |= 1 << (tk->kind - _ANNOT_START_);

		list_consume(tknList, 1);
		tk = list_head(tknList);
	}
}
