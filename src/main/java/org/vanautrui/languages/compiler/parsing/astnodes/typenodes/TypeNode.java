package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;

public class TypeNode implements IASTNode {

    public final ITypeNode typenode;

    public TypeNode(TokenList tokens) throws Exception {

        TokenList copy = tokens.copy();

        ITypeNode candidate;
        if(copy.size()>1 && copy.get(0).tokenEquals(new SymbolToken("("))){
            candidate=new SubroutineTypeNode(copy);
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

}
