#include <iostream>
#include <vector>
#include <string>

#include "../commandline/TokenList.hpp"
#include "../commandline/Token.hpp"
#include "../commandline/TokenKeys.hpp"
#include "Term.hpp"
#include "CharConst.hpp"
#include "IntConst.hpp"
#include "BoolConst.hpp"
#include "FloatConst.hpp"
#include "Expr.hpp"
#include "Variable.hpp"
#include "statements/MethodCall.hpp"

using namespace std;

struct Term* makeTerm(struct Expr* expr){
	struct Term* res = (struct Term*)malloc(sizeof(struct Term));
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;

	res->m5 = expr;
	return res;
}

struct Term* makeTerm(TokenList* tokens, bool debug) {

	struct Term* res = (struct Term*)malloc(sizeof(struct Term));
	res->m1 = NULL;
	res->m2 = NULL;
	res->m3 = NULL;
	res->m4 = NULL;
	res->m5 = NULL;
	res->m6 = NULL;
	res->m7 = NULL;

	if(debug){
		cout << "Term(...)" << endl;
		cout << "from " << tokens->code() << endl;
	}

	TokenList copy = TokenList(*tokens);

	try {
		TokenList copy2 = TokenList(copy);

		copy2.expect(Token(LPARENS));
		res->m5 = makeExpr(&copy2,debug);
		copy2.expect(Token(RPARENS));

		copy.set(copy2);
	} catch (string e0) {
		try {
			res->m2 = new IntConst(&copy,debug);
		} catch (string e1) {
			try {
				//a string constant is syntatic sugar.
				//in the parsing stage it is converted to an array of char constants
				//inline the stringConstant and its syntatic sugar
				Token token = tokens->get(0);
				//TODO: re-enable this later on
				/*
				if (token.kind == STRINGCONSTANT) {

					string content = token.getContents();
					TokenList tks = TokenList(vector(), tokens.relPath);
					tks.push_back(BaseToken(LBRACKET));
					for (int i = 0; i < content.size(); i++) {
						tks.push_back(BaseToken(CHARCONSTANT,content.at(i)));
						if (i < content.size() - 1) {
							tks.push_back(BaseToken(COMMA));
						}
					}
					tks.push_back(BaseToken(RBRACKET));
					this->termNode = ArrayConstantNode(tks);

					copy.consume(1);
				} else {
				*/
					throw string("could not read stringConstant syntatic sugar");
				//}
			} catch (string e2) {
				try {
					res->m7 = new FloatConst(&copy,debug);
				} catch (string e3) {
					try {
						res->m4 = new MethodCall(copy,debug);
					} catch (string e4) {
						try {
							res->m1 = makeBoolConst(copy,debug);
						} catch (string e5) {
							try {
								res->m6 = new Variable(copy,debug);
							} catch (string e6) {
								res->m3 = new CharConst(copy,debug);
							}
						}
					}
				}
			}
		}
	}
	tokens->set(copy);

	return res;
}

