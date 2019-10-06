package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;

public class ArrayTypeNode implements IASTNode, ITypeNode {


  public final TypeNode type;

  public ArrayTypeNode(TokenList tokens)throws Exception{

    TokenList copy1 = tokens.copy();

      copy1.expectAndConsumeOtherWiseThrowException(new SymbolToken("["));
      this.type = new TypeNode(copy1);
      copy1.expectAndConsumeOtherWiseThrowException(new SymbolToken("]"));


    tokens.set(copy1);
  }

  @Override
  public String toSourceCode() {
    return "["+this.type.toSourceCode()+"]";
  }
}
