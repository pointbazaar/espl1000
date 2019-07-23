
package org.vanautrui.languages.parsing.astnodes;


import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class DragonAST implements IDragonASTNode {

    public List<DragonClassNode> classNodeList=new ArrayList<>();

    public DragonAST(DragonTokenList tokens)throws Exception {
        System.out.println("try parse DragonAST");

        //List<DragonToken> copy = new ArrayList<>(tokens);


        boolean success_class = true;
        while (success_class) {

            try {
                this.classNodeList.add(new DragonClassNode(tokens));
            } catch (Exception e) {
                success_class = false;
            }
        }

        if(this.classNodeList.size()==0){
            throw new Exception("could not read any classes");
        }
    }

    @Override
    public String toSourceCode() {
        return this.classNodeList
                .stream()
                .map(node->node.toSourceCode())
                .collect(Collectors.joining(" "));
    }
}
