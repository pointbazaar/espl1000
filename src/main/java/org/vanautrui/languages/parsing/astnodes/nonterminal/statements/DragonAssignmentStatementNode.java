package org.vanautrui.languages.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonVariableNode;

import java.util.Optional;
import java.util.Set;

public class DragonAssignmentStatementNode implements IDragonASTNode, IDragonStatementNode {

    public DragonVariableNode variableNode;

    public DragonExpressionNode expressionNode;

    public DragonAssignmentStatementNode(DragonTokenList tokens) throws Exception {

        DragonTokenList copy = tokens.copy();

        this.variableNode=new DragonVariableNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("="));

        this.expressionNode=new DragonExpressionNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));

        tokens.set(copy);
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        if(!variableNode.getType(currentMethod.get()).equals(expressionNode.getType(currentMethod.get()))){
            throw new Exception("with an assignment, both sides have to have the same type. here, a value of type "+expressionNode.getType(currentMethod.get())+" was assigned to a value of type "+variableNode.getType(currentMethod.get()));
        }
    }

    @Override
    public String toSourceCode() {
        return " "+variableNode.toSourceCode()+" = "+this.expressionNode.toSourceCode()+" ";
    }
}
