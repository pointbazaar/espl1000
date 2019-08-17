package org.vanautrui.languages.parsing.astnodes.nonterminal;

import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.lexing.tokens.utils.DragonToken;
import org.vanautrui.languages.lexing.tokens.OperatorToken;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonAST;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonClassNode;
import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

import java.util.Optional;
import java.util.Set;

public class DragonOperatorNode implements IDragonASTNode {

    //same as the token

    public String operator;

    public DragonOperatorNode(DragonTokenList tokens){

        DragonTokenList copy = new DragonTokenList(tokens);

        DragonToken token = copy.get(0);

        if(token instanceof OperatorToken){
            this.operator=((OperatorToken)token).operator;
            copy.consume(1);
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return " "+operator+" ";
    }
}
