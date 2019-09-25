package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.IfStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.LoopStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.ReturnStatementNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements.controlflow.WhileStatementNode;

public class StatementNode implements IASTNode {

    //can be method call ,loop statement, while statement, ...
    public IStatementNode statementNode;

    public StatementNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        try {
            this.statementNode = new MethodCallNode(copy);
            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));
        }catch (Exception e1){
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
        if(statementNode instanceof MethodCallNode){
            return this.statementNode.toSourceCode()+";";
        }else {
            return this.statementNode.toSourceCode();
        }
    }
}
