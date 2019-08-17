package org.vanautrui.languages.parsing.astnodes.nonterminal.statements;

import org.simpleframework.xml.Attribute;
import org.simpleframework.xml.ElementList;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.IDragonTermNode;
import org.vanautrui.languages.parsing.astnodes.IExpressionComputable;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonExpressionNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

public class DragonMethodCallNode implements IDragonASTNode, IDragonStatementNode, IExpressionComputable , IDragonTermNode {

    @Attribute
    public String getMethodName(){
        return this.identifierMethodName.name.getContents();
    }

    public DragonIdentifierNode identifierMethodName;

    @ElementList
    public List<DragonExpressionNode> argumentList = new ArrayList<>();

    public DragonMethodCallNode(DragonTokenList tokens) throws Exception {

        //System.out.println("try parse DragonMethodCallNode");

        DragonTokenList copy = tokens.copy();

        this.identifierMethodName = new DragonIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        boolean success_argument = true;
        try {
            this.argumentList.add(new DragonExpressionNode(copy));
        } catch (Exception e) {
            success_argument=false;
        }
        while (success_argument) {
            try {
                copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
                this.argumentList.add(new DragonExpressionNode(copy));
            } catch (Exception e) {
                success_argument = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));

        //System.out.println("success");
        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.identifierMethodName.toSourceCode()
                +"("+this.argumentList.stream().map(
                DragonExpressionNode::toSourceCode
                ).collect(Collectors.joining(","))
                +")"
                +";"
        ;
    }


    @Override
    public String getType(DragonMethodNode methodNode) throws Exception {
        //TODO: return the real type. for now it returns only 'Void'
        //because for now we are compiling only void fuctions

        return "Void";
    }
}
