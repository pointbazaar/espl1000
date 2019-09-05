package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.ITermNode;
import org.vanautrui.languages.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.MethodCallNode;
import org.vanautrui.languages.parsing.astnodes.terminal.*;

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
		            this.termNode=new StringConstNode(copy);
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
