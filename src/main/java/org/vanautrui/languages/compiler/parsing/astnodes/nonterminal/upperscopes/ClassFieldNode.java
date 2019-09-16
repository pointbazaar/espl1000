package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.AccessModifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.TypeIdentifierNode;

public class ClassFieldNode implements IASTNode {

    public AccessModifierNode access;

    public TypeIdentifierNode type;

    public IdentifierNode name;

    public ClassFieldNode(TokenList tokens,boolean debug) throws Exception {
        if(debug){
            System.out.println("try to parse "+this.getClass().getSimpleName()+" from "+tokens.toSourceCodeFragment());
        }

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
