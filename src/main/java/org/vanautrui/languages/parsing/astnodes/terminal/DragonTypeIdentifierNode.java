package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import org.vanautrui.languages.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonTypeIdentifierNode implements IDragonASTNode {

    public TypeIdentifierToken typeName;

    public DragonTypeIdentifierNode(DragonTokenList tokens) throws Exception {

        DragonToken token = tokens.get(0);

        if (token instanceof TypeIdentifierToken) {
            this.typeName = ((TypeIdentifierToken) token);
            tokens.consume(1);

        } else {
            throw new Exception("could not read type identifier");
        }
    }


    @Override
    public String toSourceCode() {
        return this.typeName.getContents();
    }

}
