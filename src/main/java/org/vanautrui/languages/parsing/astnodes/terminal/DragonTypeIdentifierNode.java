package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonTypeIdentifierNode implements IDragonASTNode {

    public TypeIdentifierToken typeName;

    public DragonTypeIdentifierNode(DragonTokenList tokens) throws Exception {

        DragonToken token = tokens.get(0);

        if (token instanceof TypeIdentifierToken) {
            this.typeName = ((TypeIdentifierToken) token);
            tokens.consume(1);

        } else {
            throw new Exception("could not read type identifier");
        }
    }


    @Override
    public String toSourceCode() {
        return this.typeName.getContents();
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //check that the type is defined somewhere
        //so there should exist a class with that type

        for(DragonAST ast : asts){
            for(DragonClassNode classNode : ast.classNodeList){
                if(classNode.name.typeName.getContents().equals(this.typeName.getContents())){
                    return;
                }
            }
        }

        if(this.typeName.getContents().equals("Void")){
            return;
        }

        throw new Exception("could not find class for type: '"+this.typeName.getContents()+"'");
    }
}
