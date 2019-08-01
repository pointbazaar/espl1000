package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeNode;

public class DragonDeclaredArgumentNode implements IDragonASTNode {

    public DragonTypeNode type;

    public DragonIdentifierNode name;

    public DragonDeclaredArgumentNode(DragonTokenList tokens) throws Exception {

        DragonTokenList copy = tokens.copy();

        this.type = new DragonTypeNode(copy);

        this.name = new DragonIdentifierNode(copy);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.type.toSourceCode()+" "+this.name.toSourceCode();
    }
}
