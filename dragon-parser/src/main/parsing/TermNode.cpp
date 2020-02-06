
//standard headers
#include <vector>
#include <string>
#include <variant>

//project headers
#include "../commandline/TokenList.hpp"
#include "../commandline/BaseToken.hpp"
#include "../commandline/TokenKeys.hpp"
#include "TermNode.hpp"
//#include "FloatConstNode.hpp"
#include "CharConstNode.hpp"
#include "IntConstNode.hpp"
#include "BoolConstNode.hpp"
#include "ExpressionNode.hpp"
#include "VariableNode.hpp"
#include "statements/MethodCallNode.hpp"

using namespace std;

TermNode::TermNode(ExpressionNode expr){
	this->termNode=expr;
}

TermNode::TermNode(TokenList tokens) {

	TokenList copy = TokenList(tokens);

	try {
		//TODO: re-insert later on
		//this->termNode = FloatConstNode(copy);
		throw "later";
	} catch (string e0) {
		try {
			this->termNode = IntConstNode(copy);
		} catch (string e1) {
			try {
				//a string constant is syntatic sugar.
				//in the parsing stage it is converted to an array of char constants
				//inline the stringConstant and its syntatic sugar
				BaseToken token = tokens.get(0);
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
					throw "could not read stringConstant syntatic sugar";
				//}
			} catch (string e2) {
				try {
					TokenList copy2 = TokenList(copy);

					copy2.expectAndConsumeOtherWiseThrowException(BaseToken(LPARENS));
					this->termNode = ExpressionNode(copy2);
					copy2.expectAndConsumeOtherWiseThrowException(BaseToken(RPARENS));

					copy.set(copy2);
				} catch (string e3) {
					try {
						this->termNode = MethodCallNode(copy);
					} catch (string e4) {
						try {
							this->termNode = BoolConstNode(copy);
						} catch (string e5) {
							try {
								this->termNode = VariableNode(copy);
							} catch (string e6) {
								//TODO: re-enable later
								/*
								try {
									this->termNode = ArrayConstantNode(copy);
								} catch (string e7) {
									*/
									this->termNode = CharConstNode(copy);
								//}
							}
						}
					}
				}
			}
		}
	}
	tokens.set(copy);
}

