package org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.dragoncompiler.lexing.utils.TokenList;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.OperatorToken;
import org.vanautrui.languages.dragoncompiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.dragoncompiler.parsing.IASTNode;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.dragoncompiler.parsing.astnodes.terminal.VariableNode;

public class AssignmentStatementNode implements IASTNode, IStatementNode {

    public VariableNode variableNode;

    public ExpressionNode expressionNode;

    public AssignmentStatementNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        this.variableNode=new VariableNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new OperatorToken("="));

        this.expressionNode=new ExpressionNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return " "+variableNode.toSourceCode()+" = "+this.expressionNode.toSourceCode()+"; ";
    }
}
