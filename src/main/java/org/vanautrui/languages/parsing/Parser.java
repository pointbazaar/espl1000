package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.collections.TokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

public class Parser {
    public AST parse(TokenList tokens) throws Exception {

        AST result = new AST(tokens);
        return result;
    }
}
