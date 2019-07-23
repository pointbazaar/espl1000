/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.vanautrui.languages.parsing;

import org.vanautrui.languages.parsing.astnodes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.DragonClassNode;
import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.ClassToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;

import java.util.Iterator;
import java.util.List;

public class DragonParser {
    public DragonAST parse(List<DragonToken> tokens){

        System.out.println("PHASE: PARSING");

        //TODO
        DragonAST result=new DragonAST();

        Iterator<DragonToken> iter =  tokens.iterator();

        while (iter.hasNext()){
            DragonToken token = iter.next();

            if(token instanceof ClassToken){
                if(iter.hasNext()){

                    DragonToken next = iter.next();
                    if(next instanceof IdentifierToken) {
                        result.classNodeList.add(new DragonClassNode(next.getContents()));
                    }
                }
            }
        }

        return result;
    }
}
