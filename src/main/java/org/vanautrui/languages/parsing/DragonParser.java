
package org.vanautrui.languages.parsing;

import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.parsing.astnodes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.DragonClassNode;

import java.util.List;

public class DragonParser {
    public DragonAST parse(List<DragonToken> tokens) throws Exception{

        System.out.println("PHASE: PARSING");

        DragonAST result=new DragonAST();

        //TODO


        while (tokens.size()>0){

            try{
                result.classNodeList.add(new DragonClassNode(tokens));
                continue;
            }catch (Exception e){
                //pass
            }

            throw new Exception("tried to parse, but could not .");
        }

        return result;
    }
}
