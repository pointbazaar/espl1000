package org.vanautrui.languages.parsing.astnodes.terminal;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.tokens.AccessModifierToken;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonAccessModifierNode implements IDragonASTNode {

    //for xml
    @Attribute
    public String getAccess(){
        return this.toSourceCode();
    }

    public boolean is_public;

    public DragonAccessModifierNode(DragonTokenList tokens) throws Exception {

        DragonToken token1 = tokens.get(0);

        if (token1 instanceof AccessModifierToken) {
            this.is_public = ((AccessModifierToken) token1).is_public;
            tokens.consume(1);
        } else {
            //otherwise, it is just public. no access modifier is also an access modifier
            this.is_public = true;
        }
    }

    @Override
    public String toSourceCode() {
        return (is_public) ? "public" : "private";
    }

}
