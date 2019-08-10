package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.parsing.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonTypeIdentifierNode;

import java.util.Optional;
import java.util.Set;

public class DragonDeclaredArgumentNode implements IDragonASTNode {

    public DragonTypeIdentifierNode type;

    public DragonIdentifierNode name;

    public DragonDeclaredArgumentNode(DragonTokenList tokens) throws Exception {

        DragonTokenList copy = tokens.copy();

        this.type = new DragonTypeIdentifierNode(copy);

        this.name = new DragonIdentifierNode(copy);

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.type.toSourceCode()+" "+this.name.toSourceCode();
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {

        type.doTypeCheck(asts,currentClass,currentMethod);

    }
}
