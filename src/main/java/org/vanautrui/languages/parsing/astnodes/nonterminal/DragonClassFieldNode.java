package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeNode;

public class DragonClassFieldNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeNode type;

    public DragonIdentifierNode name;

    public DragonClassFieldNode(DragonTokenList tokens) throws Exception {
        //System.out.println("try parse DragonClassFieldNode");

        DragonTokenList copy = tokens.copy();

        this.access = new DragonAccessModifierNode(copy);

        this.type = new DragonTypeNode(copy);

        this.name = new DragonIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(";"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.access.toSourceCode()
                +" "+this.type.toSourceCode()
                +" "+this.name.toSourceCode()
                +";";
    }
}
