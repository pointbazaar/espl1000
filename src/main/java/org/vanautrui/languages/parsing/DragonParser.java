package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;

public class DragonParser {
    public DragonAST parse(DragonTokenList tokens) throws Exception {



        DragonAST result = new DragonAST(tokens);

        //TODO


        return result;
    }
}
