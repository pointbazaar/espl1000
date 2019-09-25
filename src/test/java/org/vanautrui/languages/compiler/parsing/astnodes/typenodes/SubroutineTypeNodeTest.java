package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;

import static org.junit.Assert.assertEquals;

public class SubroutineTypeNodeTest {

  @Test
  public void test_subroutine_type_parsing_subroutine_with_side_effects()throws Exception{
    final String source="(PInt,MyType)~>PInt";
    Lexer lexer = new Lexer();
    TokenList list = lexer.lexCodeTestMode(source);

    TypeNode node = new TypeNode(list);
    assertEquals(source,node.getTypeName());
    assertEquals(0,list.size());
  }

  @Test
  public void test_subroutine_type_parsing_subroutine_without_side_effects()throws Exception{
    Lexer lexer = new Lexer();
    final String source="(PInt)->PInt";
    TokenList list = lexer.lexCodeTestMode(source);

    TypeNode node = new TypeNode(list);
    assertEquals(source,node.getTypeName());
    assertEquals(0,list.size());
  }

  @Test
  public void test_subroutine_type_parsing_subroutine_with_subroutine_argument()throws Exception{
    final String source="(PInt,(PInt)->PInt)->PInt";
    Lexer lexer = new Lexer();
    TokenList list = lexer.lexCodeTestMode(source);

    TypeNode node = new TypeNode(list);
    assertEquals(source,node.getTypeName());
    assertEquals(0,list.size());
  }
}
