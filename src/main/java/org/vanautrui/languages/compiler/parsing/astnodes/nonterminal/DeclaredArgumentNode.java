package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.TypeIdentifierNode;

public class DeclaredArgumentNode implements IASTNode {

    public String type;

    public String name;

    public DeclaredArgumentNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        this.type = new TypeIdentifierNode(copy).typeName;
        this.name = new IdentifierNode(copy).name;

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.type+" "+this.name;
    }

}
