package org.vanautrui.languages.parsing.astnodes;

import org.vanautrui.languages.parsing.astnodes.nonterminal.upperscopes.DragonMethodNode;

public interface IExpressionComputable {

    //every computable expression has a type
    //sometimes this needs to be type-checked at compile time
    //this is what this interface is for
    public String getType(DragonMethodNode methodNode) throws Exception;
}
