package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

import java.util.Optional;

public class DeclaredArgumentNode implements IASTNode {

    public final TypeNode type;

    public final Optional<String> name;

    public DeclaredArgumentNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        this.type = new TypeNode(copy);
        this.name = Optional.of(new IdentifierNode(copy).name);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.type.toSourceCode()+" "+this.name;
    }

}
