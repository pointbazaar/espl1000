package org.vanautrui.languages.parsing.astnodes.nonterminal.statements;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.ElementList;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonVariableNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

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
        //TODO: check that the expression has the same type as the variable
    }

    @Override
    public String toSourceCode() {
        return " "+variableNode.toSourceCode()+" = "+this.expressionNode.toSourceCode()+" ";
    }
}
