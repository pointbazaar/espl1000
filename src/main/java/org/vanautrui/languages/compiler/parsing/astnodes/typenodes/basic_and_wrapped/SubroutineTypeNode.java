package org.vanautrui.languages.compiler.parsing.astnodes.typenodes.basic_and_wrapped;

import org.vanautrui.languages.compiler.lexing.tokens.ArrowToken;
import org.vanautrui.languages.compiler.lexing.tokens.SymbolToken;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class SubroutineTypeNode implements IBasicAndWrappedTypeNode {

  public final IBasicAndWrappedTypeNode returnType;

  public final boolean has_side_effects;

  public List<BasicTypeWrappedNode> argumentTypes=new ArrayList<>();

  public SubroutineTypeNode(TokenList tokens)throws Exception{

    TokenList copy = tokens.copy();

    copy.expectAndConsumeOtherWiseThrowException(new SymbolToken("("));

    boolean sucess_argument_types=true;
    try {
      this.argumentTypes.add(new BasicTypeWrappedNode(copy));
    }catch (Exception e){
      sucess_argument_types=false;
    }
    while (sucess_argument_types){
      try {
        TokenList copy2 = copy.copy();

        copy2.expectAndConsumeOtherWiseThrowException(new SymbolToken(","));
        this.argumentTypes.add(new BasicTypeWrappedNode(copy2));

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

  @Override
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
                      .map(BasicTypeWrappedNode::getTypeName)
                      .collect(Collectors.joining(","))
                      + ")" + ((this.has_side_effects) ? "~>" : "->") + "("+returnType.getTypeName()+")";
    }else {
      return
              "("
                      + argumentTypes
                      .stream()
                      .map(BasicTypeWrappedNode::getTypeName)
                      .collect(Collectors.joining(","))
                      + ")" + ((this.has_side_effects) ? "~>" : "->") + returnType.getTypeName();
    }
  }

  @Override
  public boolean equals(Object o) {
    if (this == o) return true;
    if (!(o instanceof SubroutineTypeNode)) return false;
    SubroutineTypeNode that = (SubroutineTypeNode) o;
    return this.toSourceCode().equals(that.toSourceCode());
  }

}
