/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.vanautrui.languages.parsing;

import org.vanautrui.languages.model.AmandaAST;
import org.vanautrui.languages.model.astnodes.AmandaClassNode;
import org.vanautrui.languages.model.tokens.AmandaToken;
import org.vanautrui.languages.model.tokens.ClassToken;
import org.vanautrui.languages.model.tokens.IdentifierToken;

import java.util.Iterator;
import java.util.List;

public class AmandaParser {
    public AmandaAST parse(List<AmandaToken> tokens){
        System.out.println("Parser started");
        //TODO
        AmandaAST result=new AmandaAST();

        Iterator<AmandaToken> iter =  tokens.iterator();

        while (iter.hasNext()){
            AmandaToken token = iter.next();

            if(token instanceof ClassToken){
                if(iter.hasNext()){

                    AmandaToken next = iter.next();
                    if(next instanceof IdentifierToken) {
                        result.classNodeList.add(new AmandaClassNode(next.getContents()));
                    }
                }
            }
        }

        return result;
    }
}
