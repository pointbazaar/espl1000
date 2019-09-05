package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.TypeIdentifierNode;

public class DeclaredArgumentNode implements IASTNode {

    public TypeIdentifierNode type;

    public IdentifierNode name;

    public DeclaredArgumentNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        this.type = new TypeIdentifierNode(copy);

        this.name = new IdentifierNode(copy);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.type.toSourceCode()+" "+this.name.toSourceCode();
    }

}
