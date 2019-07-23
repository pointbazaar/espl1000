package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;

import java.util.ArrayList;
import java.util.List;

public class DragonMethodNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeNode type;

    public DragonMethodNameNode methodName;

    public List<DragonArgumentNode> arguments=new ArrayList<>();

    public List<DragonStatementNode> statements= new ArrayList<>();

    public DragonMethodNode(DragonTokenList tokens)throws Exception{

        System.out.println("try parse DragonMethodNode");

        //TODO: consider the 2 alternative ways a method can be declored

        DragonTokenList copy = tokens.copy();

        this.access=new DragonAccessModifierNode(copy);

        this.type = new DragonTypeNode(copy);

        this.methodName = new DragonMethodNameNode(copy);

        DragonToken token = copy.get(0);

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        //TODO: parse arguments

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        //TODO: parse statements

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        //TODO
        return "";
    }
}
