package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassFieldNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonIdentifierNode implements IDragonASTNode {

    //this identifies a variable

    public String name;

    public DragonIdentifierNode(DragonTokenList tokens) throws Exception {

        //List<DragonToken> copy = new ArrayList<>(tokens);

        //Pattern p = Pattern.compile(IdentifierToken.regex_alphanumeric_identifier);

        DragonToken token = tokens.get(0);

        if (token instanceof IdentifierToken) {
            this.name = ((IdentifierToken) token).getContents();
            tokens.consume(1);

        } else {
            throw new Exception("could not read identifier");
        }

    }

    @Override
    public String toSourceCode() {
        return this.name;
    }

}
