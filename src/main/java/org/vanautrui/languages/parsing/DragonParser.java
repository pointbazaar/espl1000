/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.vanautrui.languages.parsing;

import org.vanautrui.languages.model.DragonAST;
import org.vanautrui.languages.model.astnodes.DragonClassNode;
import org.vanautrui.languages.model.tokens.AmandaToken;
import org.vanautrui.languages.model.tokens.ClassToken;
import org.vanautrui.languages.model.tokens.IdentifierToken;

import java.util.Iterator;
import java.util.List;

public class DragonParser {
    public DragonAST parse(List<AmandaToken> tokens){
        System.out.println("Parser started");
        //TODO
        DragonAST result=new DragonAST();

        Iterator<AmandaToken> iter =  tokens.iterator();

        while (iter.hasNext()){
            AmandaToken token = iter.next();

            if(token instanceof ClassToken){
                if(iter.hasNext()){

                    AmandaToken next = iter.next();
                    if(next instanceof IdentifierToken) {
                        result.classNodeList.add(new DragonClassNode(next.getContents()));
                    }
                }
            }
        }

        return result;
    }
}
