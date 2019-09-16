package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.statements;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.compiler.parsing.astnodes.ITermNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.ExpressionNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class MethodCallNode implements IASTNode, IStatementNode, IExpressionComputable , ITermNode {

    public final String methodName;

    public List<ExpressionNode> argumentList = new ArrayList<>();

    public MethodCallNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        this.methodName = new IdentifierNode(copy).name;

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        boolean success_argument = true;
        try {
            this.argumentList.add(new ExpressionNode(copy));
        } catch (Exception e) {
            success_argument=false;
        }
        while (success_argument) {
            try {
                TokenList copy2=new TokenList(copy);
                copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
                this.argumentList.add(new ExpressionNode(copy2));

                copy.set(copy2);
            } catch (Exception e) {
                success_argument = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.methodName
                +"("+this.argumentList.stream().map(
                ExpressionNode::toSourceCode
                ).collect(Collectors.joining(","))
                +")"
                +";"
        ;
    }

}
