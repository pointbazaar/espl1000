package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.IDragonTermNode;
import org.vanautrui.languages.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIntegerConstantNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonVariableNode;

import java.util.Optional;
import java.util.Set;

public class DragonTermNode implements IDragonASTNode, IExpressionComputable {

    //can be one of these:

    //(keyword constant means true|false|null|this

    /*

    integer constant
    string constant
    keyword constant
    '(' expression ')'
    unaryOp term

     */
    public IDragonTermNode termNode;

    //TODO: insert the oher alternatives

    public DragonTermNode (DragonTokenList tokens)throws Exception{

        DragonTokenList copy = new DragonTokenList(tokens);

        try{
            this.termNode=new DragonIntegerConstantNode(copy);
        }catch (Exception e1){
            try {
                this.termNode=new DragonStringConstantNode(copy);
            }catch (Exception e2){
                try {
                    copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));
                    this.termNode=new DragonExpressionNode(copy);
                    copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));
                }catch (Exception e3){
                    this.termNode=new DragonVariableNode(copy);
                }
            }
        }

        tokens.set(copy);
    }


    @Override
    public String toSourceCode() {

        if(this.termNode instanceof DragonIntegerConstantNode){
            DragonIntegerConstantNode integerConstantNode=(DragonIntegerConstantNode)termNode;
            return integerConstantNode.toSourceCode();
        }
        if(this.termNode instanceof DragonStringConstantNode){
            DragonStringConstantNode stringConstantNode = (DragonStringConstantNode)this.termNode;
            return stringConstantNode.toSourceCode();
        }
        if(this.termNode instanceof DragonExpressionNode) {
            DragonExpressionNode expressionNode = (DragonExpressionNode) this.termNode;
            return expressionNode.toSourceCode();
        }else if(this.termNode instanceof DragonVariableNode){
            DragonVariableNode variableNode = (DragonVariableNode) this.termNode;
            return variableNode.toSourceCode();
        }else{
            //return "ERROR";
            throw new RuntimeException("error in toSourceCode() in DragonTermNode");
        }
    }

    @Override
    public String getType(DragonMethodNode methodNode) throws Exception {
        return this.termNode.getType(methodNode);
    }
}
