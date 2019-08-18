package org.vanautrui.languages.parsing.astnodes.terminal;

import org.simpleframework.xml.Attribute;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import org.vanautrui.languages.lexing.tokens.StringConstantToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.IDragonTermNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonStringConstantNode implements IDragonASTNode, IDragonTermNode {

    @Attribute
    public String str;

    public DragonStringConstantNode(DragonTokenList tokens) throws Exception {

        DragonToken token = tokens.get(0);

        if (token instanceof StringConstantToken) {
            this.str = ((StringConstantToken) token).getContents();
            tokens.consume(1);
        } else {
            throw new Exception("could not read stringConstant node");
        }

    }

    @Override
    public String toSourceCode() {
        return this.str;
    }
}
