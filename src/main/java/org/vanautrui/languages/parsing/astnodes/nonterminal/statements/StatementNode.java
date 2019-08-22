package org.vanautrui.languages.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;

public class StatementNode implements IASTNode {

    //TODO: add more statement types

    //can be method call ,loop statement, while statement, ...
    public IStatementNode statementNode;

    public StatementNode(TokenList tokens) throws Exception {
        //System.out.println("try parse DragonStatementNode");

        TokenList copy = tokens.copy();

        try {
            this.statementNode = new MethodCallNode(copy);
            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));
        }catch (Exception e1){
            //maybe there is a loop statement
            try {
                this.statementNode = new LoopStatementNode(copy);
            }catch (Exception e2){
                try {
                    this.statementNode = new AssignmentStatementNode(copy);
                }catch (Exception e3){
                    try {
                        this.statementNode = new WhileStatementNode(copy);
                    }catch (Exception e4){
                        try {
                            this.statementNode = new IfStatementNode(copy);
                        }catch (Exception e5){
                            this.statementNode = new ReturnStatementNode(copy);
                        }
                    }
                }
            }
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.statementNode.toSourceCode();
    }
}
