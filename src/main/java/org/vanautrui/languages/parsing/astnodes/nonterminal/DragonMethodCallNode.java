package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonStringConstantNode;

import java.util.ArrayList;
import java.util.List;

public class DragonMethodCallNode implements IDragonASTNode {

    public DragonIdentifierNode identifierMethodName;

    public List<DragonStringConstantNode> argumentList = new ArrayList<>();

    public DragonMethodCallNode(DragonTokenList tokens) throws Exception {

        //System.out.println("try parse DragonMethodCallNode");

        DragonTokenList copy = tokens.copy();

        this.identifierMethodName = new DragonIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        try {
            this.argumentList.add(new DragonStringConstantNode(copy));
        } catch (Exception e) {
            //pass
        }

        boolean success_argument = true;
        while (success_argument) {

            try {
                copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
                this.argumentList.add(new DragonStringConstantNode(copy));
            } catch (Exception e) {
                success_argument = false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

        //System.out.println("success");
        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        //TODO
        return "";
    }
}
