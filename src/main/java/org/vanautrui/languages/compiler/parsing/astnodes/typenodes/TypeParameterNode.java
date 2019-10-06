package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.lexing.tokens.TypeParameterIdentifierToken;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped.IBasicAndWrappedTypeNode;

public class TypeParameterNode implements IBasicAndWrappedTypeNode, ITypeNode {

  public final int typeParameterIndex;

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (!(o instanceof TypeParameterNode)) return false;
    TypeParameterNode that = (TypeParameterNode) o;
    return this.toSourceCode().equals(that.toSourceCode());
  }

  public TypeParameterNode(TokenList tokens)throws Exception{
    IToken token = tokens.get(0);
    if (token instanceof TypeParameterIdentifierToken) {
      this.typeParameterIndex = ((TypeParameterIdentifierToken)token).type_parameter_number;
      tokens.consume(1);
    } else {
      throw new Exception("could not read type parameter node");
    }
  }

  @Override
  public String toSourceCode() {
    return "?T"+this.typeParameterIndex;
  }

  @Override
  public String getTypeName() {
    return this.toSourceCode();
  }
}
