package org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes;

import org.vanautrui.languages.compiler.lexing.tokens.KeywordToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.ITypeNode;
import org.vanautrui.languages.compiler.parsing.astnodes.typenodes.simple.SimpleTypeNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public final class StructDeclNode implements IASTNode, ITypeNode {

  public final SimpleTypeNode structType;
  public final List<StructMemberDeclNode> structMembersList=new ArrayList<>();

  public StructDeclNode(TokenList tokens, boolean debug) throws Exception {

    if(debug){
      System.out.println("try to parse "+this.getClass().getSimpleName()+" from '"+tokens.toSourceCodeFragment()+"'");
    }

    TokenList copy = tokens.copy();

    copy.expectAndConsumeOtherWiseThrowException(new KeywordToken("struct"));

    this.structType = new SimpleTypeNode(copy);

    copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("{"));

    // parse arguments
    boolean success_member = true;
    try {
      this.structMembersList.add(new StructMemberDeclNode(copy,debug));
    } catch (Exception e) {
      success_member=false;
    }
    while (success_member) {
      try {
        copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
        this.structMembersList.add(new StructMemberDeclNode(copy,debug));
      } catch (Exception e) {
        success_member = false;
      }
    }

    copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("}"));

    tokens.set(copy);
  }

  @Override
  public String getTypeName() {
    return this.structType.getTypeName();
  }

  @Override
  public String toSourceCode() {
    return "struct "+this.getTypeName()+"{"+this.structMembersList.stream().map(member->member.toSourceCode()).collect(Collectors.joining(","))+"}";
  }
}
