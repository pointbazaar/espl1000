package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.tokens.SymbolToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonAccessModifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeNode;

import java.util.ArrayList;
import java.util.List;

public class DragonMethodNode implements IDragonASTNode {

    public DragonAccessModifierNode access;

    public DragonTypeNode type;

    public DragonMethodNameNode methodName;

    public List<DragonDeclaredArgumentNode> arguments=new ArrayList<>();

    public List<DragonStatementNode> statements= new ArrayList<>();

    public DragonMethodNode(DragonTokenList tokens)throws Exception{

        System.out.println("try parse DragonMethodNode");

        //TODO: consider the 2 alternative ways a method can be declored

        DragonTokenList copy = tokens.copy();

        this.access=new DragonAccessModifierNode(copy);

        this.type = new DragonTypeNode(copy);

        this.methodName = new DragonMethodNameNode(copy);

        System.out.println("AFTER access,type,methodname: current state of copy:");
        System.out.println(copy.toSourceCodeFragment());
        System.out.println();

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

        //TODO: parse arguments

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

        System.out.println("AFTER '(){': current state of copy:");
        System.out.println(copy.toSourceCodeFragment());
        System.out.println();

        //TODO: parse statements

        boolean success_statements=true;
        while (success_statements){
            try{
                this.statements.add(new DragonStatementNode(copy));
            }catch (Exception e){
                success_statements=false;
            }
        }

        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        //TODO
        return "";
    }
}
