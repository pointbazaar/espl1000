package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

public class DragonMethodCallNode implements IDragonASTNode {

    public DragonIdentifierNode identifierMethodName;

    public List<DragonStringConstantNode> argumentList = new ArrayList<>();

    public DragonMethodCallNode(DragonTokenList tokens) throws Exception {

        //System.out.println("try parse DragonMethodCallNode");

        DragonTokenList copy = tokens.copy();

        this.identifierMethodName = new DragonIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        boolean success_argument = true;
        try {
            this.argumentList.add(new DragonStringConstantNode(copy));
        } catch (Exception e) {
            success_argument=false;
        }
        while (success_argument) {
            try {
                copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
                this.argumentList.add(new DragonStringConstantNode(copy));
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
                        dragonStringConstantNode -> dragonStringConstantNode.toSourceCode()
                ).collect(Collectors.joining(","))
                +")"
                +";"
        ;
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //TODO: check that the method is called on an object
        //which is actually declared and initialized
        //and is in scope

        //for static method calls, check that the class exists
    }


}
