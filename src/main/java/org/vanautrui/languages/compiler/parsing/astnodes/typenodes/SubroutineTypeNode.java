package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.vanautrui.languages.compiler.lexing.tokens.ArrowToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.IASTNode;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

class SubroutineTypeNode implements ITypeNode {

  public final ITypeNode returnType;

  public final boolean has_side_effects;

  public List<TypeNode> argumentTypes=new ArrayList<>();

  public SubroutineTypeNode(TokenList tokens)throws Exception{

    TokenList copy = tokens.copy();

    copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

    boolean sucess_argument_types=true;
    try {
      this.argumentTypes.add(new TypeNode(copy));
    }catch (Exception e){
      sucess_argument_types=false;
    }
    while (sucess_argument_types){
      try {
        TokenList copy2 = copy.copy();

        copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
        this.argumentTypes.add(new TypeNode(copy2));

        copy.set(copy2);
      }catch (Exception e){
        sucess_argument_types=false;
      }
    }

    copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));

    if(copy.head() instanceof ArrowToken){
      ArrowToken arrow = (ArrowToken) copy.head();
      this.has_side_effects=!arrow.is_functional;
      copy.consume(1);
    }else{
      throw new Exception("expected an arrow token here");
    }

    if(copy.head().tokenEquals(new SymbolToken("("))){
      //return type is subroutine
      copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));
      this.returnType = new SubroutineTypeNode(copy);
      copy.expectAndConsumeOtherWiseThrowException(new SymbolToken(")"));
    }else {
      this.returnType = new SimpleTypeNode(copy);
    }

    tokens.set(copy);
  }

  public String getTypeName(){
    return this.toSourceCode();
  }

  @Override
  public String toSourceCode() {
    if(this.returnType instanceof SubroutineTypeNode){
      return
              "("
                      + argumentTypes
                      .stream()
                      .map(IASTNode::toSourceCode)
                      .collect(Collectors.joining(","))
                      + ")" + ((this.has_side_effects) ? "~>" : "->") + "("+returnType.toSourceCode()+")";
    }else {
      return
              "("
                      + argumentTypes
                      .stream()
                      .map(IASTNode::toSourceCode)
                      .collect(Collectors.joining(","))
                      + ")" + ((this.has_side_effects) ? "~>" : "->") + returnType.toSourceCode();
    }
  }
}
