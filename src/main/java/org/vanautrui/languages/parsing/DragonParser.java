
package org.vanautrui.languages.parsing;

import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;

public class DragonParser {
    public DragonAST parse(DragonTokenList tokens) throws Exception{

        System.out.println("PHASE: PARSING");

        DragonAST result=new DragonAST(tokens);

        //TODO



        return result;
    }
}
