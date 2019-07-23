package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.lexing.tokens.ClassToken;
import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class DragonClassNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonIdentifierNode name;

    public List<DragonClassFieldNode> fieldNodeList=new ArrayList<>();

    public List<DragonMethodNode> methodNodeList=new ArrayList<>();



    public DragonClassNode(DragonTokenList tokens) throws Exception{
        System.out.println("try parse DragonClassNode");

        DragonTokenList copy = tokens.copy();

        this.access=new DragonAccessModifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new ClassToken());

        this.name=new DragonIdentifierNode(copy);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

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

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }


    @Override
    public String toSourceCode() {
        String result="";

        result+=fieldNodeList.stream().map(node->node.toSourceCode()).collect(Collectors.joining(" "));

        result+=methodNodeList.stream().map(node->node.toSourceCode()).collect(Collectors.joining(" "));

        return result;
    }
}
