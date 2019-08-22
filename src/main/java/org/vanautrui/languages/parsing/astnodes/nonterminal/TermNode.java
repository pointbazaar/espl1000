package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
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

    public TermNode(DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);
		try{
			this.termNode=new FloatConstantNode(copy);
		}catch(Exception e0){
		    try{
		        this.termNode=new IntegerConstantNode(copy);
		    }catch (Exception e1){
		        try {
		            this.termNode=new StringConstantNode(copy);
		        }catch (Exception e2){
		            try {
		                DragonTokenList copy2=new DragonTokenList(copy);

		                copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));
		                this.termNode=new ExpressionNode(copy2);
		                copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

		                copy.set(copy2);
		            }catch (Exception e3){
		                try {
		                    this.termNode=new MethodCallNode(copy);
		                }catch (Exception e4){
							try{
								this.termNode=new BoolConstantNode(copy);
							}catch (Exception e5){
		                    	this.termNode = new VariableNode(copy);
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
		if(this.termNode instanceof BoolConstantNode){
			return ((BoolConstantNode)termNode).toSourceCode();
		}else if(this.termNode instanceof FloatConstantNode){
            FloatConstantNode node=(FloatConstantNode)termNode;
            return node.toSourceCode();
        }
        if(this.termNode instanceof IntegerConstantNode){
            IntegerConstantNode integerConstantNode=(IntegerConstantNode)termNode;
            return integerConstantNode.toSourceCode();
        }
        if(this.termNode instanceof StringConstantNode){
            StringConstantNode stringConstantNode = (StringConstantNode)this.termNode;
            return stringConstantNode.toSourceCode();
        }
        if(this.termNode instanceof ExpressionNode) {
            ExpressionNode expressionNode = (ExpressionNode) this.termNode;
            return "("+expressionNode.toSourceCode()+")";
        }else if(this.termNode instanceof VariableNode) {
            VariableNode variableNode = (VariableNode) this.termNode;
            return variableNode.toSourceCode();
        }else if(this.termNode instanceof MethodCallNode){
            MethodCallNode methodCallNode = (MethodCallNode) this.termNode;
            return methodCallNode.toSourceCode();
        }else{
            //return "ERROR";
            throw new RuntimeException("error in toSourceCode() in DragonTermNode");
        }
    }
}
