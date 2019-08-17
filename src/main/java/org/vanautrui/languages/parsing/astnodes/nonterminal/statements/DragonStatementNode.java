package org.vanautrui.languages.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IDragonStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.controlflow.DragonWhileStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonStatementNode implements IDragonASTNode {

    //TODO: add more statement types

    //can be method call ,loop statement, while statement, ...
    public IDragonStatementNode statementNode;

    public DragonStatementNode(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonStatementNode");

        DragonTokenList copy = tokens.copy();

        try {
            this.statementNode = new DragonMethodCallNode(copy);
            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));
        }catch (Exception e1){
            //maybe there is a loop statement
            try {
                this.statementNode = new DragonLoopStatementNode(copy);
            }catch (Exception e2){
                try {
                    this.statementNode = new DragonAssignmentStatementNode(copy);
                }catch (Exception e3){
                    this.statementNode=new DragonWhileStatementNode(copy);
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
