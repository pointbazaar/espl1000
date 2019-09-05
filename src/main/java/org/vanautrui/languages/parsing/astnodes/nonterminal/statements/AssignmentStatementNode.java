package org.vanautrui.languages.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.lexing.tokens.OperatorToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.parsing.astnodes.terminal.VariableNode;

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
