package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;

public class SimpleTypeNode implements IBasicAndWrappedTypeNode {

  public final String typeName;

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (!(o instanceof SimpleTypeNode)) return false;
    SimpleTypeNode that = (SimpleTypeNode) o;
    return this.toSourceCode().equals(that.toSourceCode());
  }

  public SimpleTypeNode(TokenList tokens)throws Exception{
    IToken token = tokens.get(0);
    if (token instanceof TypeIdentifierToken) {
      this.typeName = ((TypeIdentifierToken) token).getContents();
      tokens.consume(1);
    } else {
      throw new Exception("could not read simple type identifier");
    }
  }

  public SimpleTypeNode(String typeName) {
    this.typeName=typeName;
  }

  @Override
  public String toSourceCode() {
    return this.typeName;
  }

  @Override
  public String getTypeName() {
    return this.typeName;
  }
}
