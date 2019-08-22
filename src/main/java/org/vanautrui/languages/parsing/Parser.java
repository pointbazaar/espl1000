package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.AST;

public class Parser {
    public AST parse(DragonTokenList tokens) throws Exception {



        AST result = new AST(tokens);

        //TODO


        return result;
    }
}
