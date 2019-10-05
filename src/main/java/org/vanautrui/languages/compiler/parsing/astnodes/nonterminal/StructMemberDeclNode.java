package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal;

import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.terminal.IdentifierNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.TypeNode;

public class StructMemberDeclNode implements IASTNode {

  public final TypeNode type;
  public final String name;

  public StructMemberDeclNode(TokenList tokens, boolean debug)throws Exception{
    if(debug){
      System.out.println("try to parse "+this.getClass().getSimpleName()+" from '"+tokens.toSourceCodeFragment()+"'");
    }

    TokenList copy = tokens.copy();

    this.type = new TypeNode(copy);
    this.name = new IdentifierNode(copy).name;

    tokens.set(copy);
  }

  @Override
  public String toSourceCode() {
    return this.type.toSourceCode()+" "+this.name;
  }
}
