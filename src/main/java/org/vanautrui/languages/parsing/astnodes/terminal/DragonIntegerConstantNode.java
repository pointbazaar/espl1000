package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.DragonToken;
import org.vanautrui.languages.lexing.tokens.IntegerConstantToken;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonIntegerConstantNode implements IDragonASTNode {

    public long value;

    public DragonIntegerConstantNode(DragonTokenList tokens) throws Exception {

        DragonToken token = tokens.get(0);

        if (token instanceof IntegerConstantToken) {
            this.value = ((IntegerConstantToken) token).value;
            tokens.consume(1);
        } else {
            throw new Exception("could not read stringConstant node");
        }

    }

    @Override
    public String toSourceCode() {
        return this.value+"";
    }

    @Override
    public void doTypeCheck(Set<DragonAST> asts, Optional<DragonClassNode> currentClass, Optional<DragonMethodNode> currentMethod) throws Exception {
        //an integer is always typesafe, nothing to do here
        return;
    }
}
