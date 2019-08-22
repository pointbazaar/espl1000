package org.vanautrui.languages.parsing.astnodes.terminal;

import org.vanautrui.languages.lexing.tokens.*;
import org.vanautrui.languages.lexing.collections.DragonTokenList;
import org.vanautrui.languages.parsing.IDragonASTNode;
import org.vanautrui.languages.parsing.astnodes.IDragonTermNode;

public class DragonBoolConstantNode implements IDragonASTNode, IDragonTermNode {

    //this can also have a negative value if it is preceded by a '-' operator token

    public boolean value;

    public DragonBoolConstantNode(DragonTokenList tokens) throws Exception {

        DragonTokenList copy = new DragonTokenList(tokens);

        if(copy.get(0) instanceof BoolConstantToken){
            BoolConstantToken tk = (BoolConstantToken)copy.get(0);
			this.value=tk.value;
			copy.consume(1);
		} else {
            throw new Exception("could not read Bool Constant node");
        }

        tokens.set(copy);
    }

    @Override
    public String toSourceCode() {
        return this.value+"";
    }

}
