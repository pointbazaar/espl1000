package org.vanautrui.languages.parsing.astnodes.terminal;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;
import org.vanautrui.languages.parsing.astnodes.terminal.DragonIdentifierNode;

import java.util.Optional;
import java.util.Set;

public class DragonMethodNameNode implements IDragonASTNode {

    public DragonIdentifierNode methodName;

    public DragonMethodNameNode(DragonTokenList tokens) throws Exception {

        this.methodName = new DragonIdentifierNode(tokens);

    }


    @Override
    public String toSourceCode() {
        return this.methodName.toSourceCode();
    }

}
