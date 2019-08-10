package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IdentifierToken;
import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.*;

import java.util.Optional;
import java.util.Set;

public class DragonIdentifierNode implements IDragonASTNode {

    //this identifies a variable

    public IdentifierToken name;

    public DragonIdentifierNode(DragonTokenList tokens) throws Exception {

        //List<DragonToken> copy = new ArrayList<>(tokens);

        //Pattern p = Pattern.compile(IdentifierToken.regex_alphanumeric_identifier);

        DragonToken token = tokens.get(0);

        if (token instanceof IdentifierToken) {
            this.name = ((IdentifierToken) token);
            tokens.consume(1);

        } else {
            throw new Exception("could not read identifier");
        }

    }

    @Override
    public String toSourceCode() {
        return this.name.getContents();
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //TODO: it should check that the identifier is
        //declared in method scope or class scope.
        //so there should be some declaration of it
        //TODO: also check that the identifier is not duplicate declaration

        //identifiers can only be used within a class or method so
        //there should be a context
        if(!currentClass.isPresent()){
            throw new Exception("identifier not in class scope");
        }

        for(DragonClassFieldNode fieldNode : currentClass.get().fieldNodeList){
            if(fieldNode.name.name.equals(this.name.getContents())){
                //found the identifier declared here
                return;
            }
        }

        //it is not declared in class scope, it should be declared in
        //method scope

        DragonMethodNode methodNode = currentMethod.get();

        for(DragonDeclaredArgumentNode arg : methodNode.arguments){
            if(arg.name.name.equals(this.name.getContents())){
                return;
            }
        }

        //TODO: search if identifier is declared as a variable
        //by a statement above its usage in the method
        //this should maybe be done in another implementation
        //of this method in another node class,
        //perhaps in MethodNode.doTypeCheck();

        throw new Exception("could not find declaration for usage of Identifier '"+this.name.getContents()+"'");
    }
}
