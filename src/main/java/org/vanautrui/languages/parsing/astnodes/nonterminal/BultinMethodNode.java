package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.tokens.utils.Token;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.IASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.IdentifierNode;

import java.util.Arrays;
import java.util.List;

public class BultinMethodNode implements IASTNode {

    public final static List<String> builtin_methods = Arrays.asList(
            "print", "println"
    );

    public IdentifierNode methodname;

    public BultinMethodNode(TokenList tokens) throws Exception {

        Token token = tokens.get(0);

        if (token instanceof IdentifierToken) {
            IdentifierToken identifierToken = (IdentifierToken) token;
            if (builtin_methods.contains(identifierToken.getContents())) {
                this.methodname = new IdentifierNode(tokens);
            }
        }
    }

    @Override
    public String toSourceCode() {
        return this.methodname.toSourceCode();
    }
}
