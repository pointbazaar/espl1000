package org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.lexing.utils.TokenList;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.AccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.TypeIdentifierNode;

public class ClassFieldNode implements IASTNode {

    public AccessModifierNode access;

    public TypeIdentifierNode type;

    public IdentifierNode name;

    public ClassFieldNode(TokenList tokens) throws Exception {
        //System.out.println("try parse DragonClassFieldNode");

        TokenList copy = tokens.copy();

        this.access = new AccessModifierNode(copy);

        this.type = new TypeIdentifierNode(copy);

        this.name = new IdentifierNode(copy);

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
