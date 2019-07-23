package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class DragonClassNode implements IDragonASTNode {

    public List<DragonClassFieldNode> fieldNodeList=new ArrayList<>();

    public List<DragonMethodNode> methodNodeList=new ArrayList<>();

    public DragonIdentifierNode name;

    public DragonClassNode(List<DragonToken> tokens) throws Exception{
        
        List<DragonToken> copy = new ArrayList<>(tokens);

        boolean success_field=true;
        while(success_field) {

            try {
                this.fieldNodeList.add(new DragonClassFieldNode(copy));
            } catch (Exception e) {
                success_field = false;
            }
        }

        boolean success_method=true;
        while(success_method) {

            try {
                this.methodNodeList.add(new DragonMethodNode(copy));
            } catch (Exception e) {
                success_method = false;
            }
        }

        this.name=new DragonIdentifierNode(copy);

        tokens.clear();
        tokens.addAll(copy);
    }


    @Override
    public String toSourceCode() {
        String result="";

        result+=fieldNodeList.stream().map(node->node.toSourceCode()).collect(Collectors.joining(" "));

        result+=methodNodeList.stream().map(node->node.toSourceCode()).collect(Collectors.joining(" "));

        return result;
    }
}
