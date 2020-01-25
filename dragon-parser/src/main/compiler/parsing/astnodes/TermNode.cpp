
//standard headers
#include <vector>
#include <set>

//project headers
#include "TermNode.hpp"

class TermNode {

public:
	ITermNode termNode;

	TermNode(ExpressionNode expr){
		this.termNode=expr;
	}

	TermNode(TokenList tokens) {

		TokenList copy = new TokenList(tokens);

		try {
			this.termNode = new FloatConstNode(copy);
		} catch (string e0) {
			try {
				this.termNode = new IntConstNode(copy);
			} catch (string e1) {
				try {
					//a string constant is syntatic sugar.
					//in the parsing stage it is converted to an array of char constants
					//inline the stringConstant and its syntatic sugar
					IToken token = tokens.get(0);
					if (token instanceof StringConstantToken) {

						String content = token.getContents();
						TokenList tks = new TokenList(new ArrayList<>(), tokens.relPath);
						tks.add(new LBracketToken());
						for (int i = 0; i < content.length(); i++) {
							tks.add(new CharConstantToken(content.charAt(i)));
							if (i < content.length() - 1) {
								tks.add(new CommaToken());
							}
						}
						tks.add(new RBracketToken());
						this.termNode = new ArrayConstantNode(tks);

						copy.consume(1);
					} else {
						throw "could not read stringConstant syntatic sugar";
					}
				} catch (string e2) {
					try {
						TokenList copy2 = new TokenList(copy);

						copy2.expectAndConsumeOtherWiseThrowException(new LParensToken());
						this.termNode = new ExpressionNode(copy2);
						copy2.expectAndConsumeOtherWiseThrowException(new RParensToken());

						copy.set(copy2);
					} catch (string e3) {
						try {
							this.termNode = new MethodCallNode(copy);
						} catch (string e4) {
							try {
								this.termNode = new BoolConstNode(copy);
							} catch (string e5) {
								try {
									this.termNode = new VariableNode(copy);
								} catch (string e6) {
									try {
										this.termNode = new ArrayConstantNode(copy);
									} catch (string e7) {
										this.termNode = new CharConstNode(copy);
									}
								}
							}
						}
					}
				}
			}
		}
		tokens.set(copy);
	}

};