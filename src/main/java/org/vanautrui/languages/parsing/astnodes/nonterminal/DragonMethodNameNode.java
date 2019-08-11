package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;

import java.util.Optional;
import java.util.Set;

public class DragonMethodNameNode implements IDragonASTNode {

    @Attribute
    public String getName(){
        return this.methodName.name.getContents();
    }

    public DragonIdentifierNode methodName;

    public DragonMethodNameNode(DragonTokenList tokens) throws Exception {

        this.methodName = new DragonIdentifierNode(tokens);

    }


    @Override
    public String toSourceCode() {
        return this.methodName.toSourceCode();
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //method names should not be duplicate in a class
        //this may change in another version of dragon

        if(!currentClass.isPresent()){
            throw new Exception("method name can only exist in a class context");
        }

        DragonClassNode classNode = currentClass.get();

        long count = classNode.methodNodeList.stream().filter(mNode -> mNode.methodName.methodName.name.getContents().equals(this.methodName.name.getContents())).count();

        if(count>1){
            throw new Exception("duplicate declaration of method '"+this.methodName.name.getContents()+"' ");
        }
    }
}
