package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonLoopStatementNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.DragonMethodCallNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.statements.IDragonStatementNode;

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
        }catch (Exception e1){
            //maybe there is a loop statement
            this.statementNode=new DragonLoopStatementNode(copy);
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.statementNode.toSourceCode();
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        statementNode.doTypeCheck(asts,currentClass,currentMethod);
    }


}
