package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.utils.IToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;

public class SimpleTypeNode implements ITypeNode {

  public final String typeName;

  public SimpleTypeNode(TokenList tokens)throws Exception{
    IToken token = tokens.get(0);
    if (token instanceof TypeIdentifierToken) {
      this.typeName = ((TypeIdentifierToken) token).getContents();
      tokens.consume(1);
    } else {
      throw new Exception("could not read simple type identifier");
    }
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
