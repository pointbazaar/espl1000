package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;

public class BasicTypeWrappedNode implements IASTNode, ITypeNode {

    public final IBasicAndWrappedTypeNode typenode;

    public BasicTypeWrappedNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        IBasicAndWrappedTypeNode candidate;
        if(copy.size()>1 && copy.get(0).tokenEquals(new SymbolToken("("))){
            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));
            candidate=new SubroutineTypeNode(copy);
            copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));
        }else{
            candidate=new SimpleTypeNode(copy);
        }
        this.typenode=candidate;
        tokens.set(copy);
    }

    public String getTypeName(){
        return this.toSourceCode();
    }

    @Override
    public String toSourceCode() {
        return this.typenode.toSourceCode();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof BasicTypeWrappedNode)) return false;
        BasicTypeWrappedNode basicTypeWrappedNode = (BasicTypeWrappedNode) o;
        return this.toSourceCode().equals(basicTypeWrappedNode.toSourceCode());
    }
}
