package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;

public class DragonMethodNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeNode type;

    public DragonMethodNameNode methodName;

    public List<DragonArgumentNode> arguments=new ArrayList<>();

    public List<DragonStatementNode> statements= new ArrayList<>();

    public DragonMethodNode(List<DragonToken> tokens)throws Exception{

        //TODO: consider the 2 alternative ways a method can be declored
    }

    //TODO

    @Override
    public String toSourceCode() {
        //TODO
        return "";
    }
}
