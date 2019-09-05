package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.lexing.tokens.CharConstantToken;
import org.vanautrui.languages.compiler.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.*;

public class TermNode implements IASTNode, IExpressionComputable {

    //can be one of these:

    //(keyword constant means true|false|null|this

    /*

    integer constant
    string constant
    keyword constant
    '(' expression ')'
    unaryOp term

     */
    public ITermNode termNode;

    //TODO: insert the oher alternatives

    public TermNode(TokenList tokens)throws Exception{

        TokenList copy = new TokenList(tokens);
		try{
			this.termNode=new FloatConstNode(copy);
		}catch(Exception e0){
		    try{
		        this.termNode=new IntConstNode(copy);
		    }catch (Exception e1){
		        try {
		        	//a string constant is syntatic sugar.
					//in the parsing stage it is converted to an array of char constants

					//inline the stringConstant and its syntatic sugar

					IToken token = tokens.get(0);

					if (token instanceof StringConstantToken) {

						String content = ((StringConstantToken) token).getContents();
						TokenList tks = new TokenList();
						tks.add(new SymbolToken("["));
						for(int i=0;i<content.length();i++){
							tks.add(new CharConstantToken(content.charAt(i)));
							if(i<content.length()-1){
								tks.add(new SymbolToken(","));
							}
						}
						tks.add(new SymbolToken("["));
						this.termNode=new ArrayConstantNode(tks);

						tokens.consume(1);
					} else {
						throw new Exception("could not read stringConstant syntatic sugar");
					}



		        }catch (Exception e2){
		            try {
		                TokenList copy2=new TokenList(copy);

		                copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));
		                this.termNode=new ExpressionNode(copy2);
		                copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

		                copy.set(copy2);
		            }catch (Exception e3){
		                try {
		                    this.termNode=new MethodCallNode(copy);
		                }catch (Exception e4){
							try{
								this.termNode=new BoolConstNode(copy);
							}catch (Exception e5){
							    try {
                                    this.termNode = new VariableNode(copy);
                                }catch (Exception e6){
							    	try {
										this.termNode = new ArrayConstantNode(copy);
									}catch (Exception e7){
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


    @Override
    public String toSourceCode() {
        return termNode.toSourceCode();
    }
}
