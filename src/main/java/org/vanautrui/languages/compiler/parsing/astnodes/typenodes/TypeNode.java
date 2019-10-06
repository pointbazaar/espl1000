package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.BasicTypeWrappedNode;

public class TypeNode implements IASTNode {

  public ITypeNode type;

  public TypeNode(TokenList tokens)throws Exception{

    TokenList copy = tokens.copy();

    try{
      this.type=new ArrayTypeNode(copy);
    }catch (Exception e){
      try {
        this.type = new TypeParameterNode(copy);
      }catch (Exception e2){
        this.type = new BasicTypeWrappedNode(copy);
      }
    }

    tokens.set(copy);
  }

  @Override
  public String toSourceCode() {
    return this.type.toSourceCode();
  }

  public String getTypeName() {
    return this.toSourceCode();
  }
}
