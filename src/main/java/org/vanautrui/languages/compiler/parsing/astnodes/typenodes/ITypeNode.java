package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.parsing.IASTNode;

public interface ITypeNode extends IASTNode {

  //classes implementing this interface are also used to communicate type information throughout the compiler.
  //so it is not just for parsing.

  public String getTypeName();
}
