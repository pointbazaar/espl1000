
package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.astnodes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.DragonClassNode;

import java.util.List;

public class DragonParser {
    public DragonAST parse(DragonTokenList tokens) throws Exception{

        System.out.println("PHASE: PARSING");

        DragonAST result=new DragonAST(tokens);

        //TODO



        return result;
    }
}
