package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

public class TypeNodeTest {

  @Test
  public void test_type_parsing()throws Exception{
    final String source = "(PInt,NInt,MyType)~>((PInt)->PInt)";

    Lexer lexer = new Lexer();
    TokenList list = lexer.lexCodeTestMode(source);

    TypeNode node = new TypeNode(list);

    assertTrue(node.typenode instanceof SubroutineTypeNode);
    assertEquals(source,node.getTypeName());
    assertEquals(0,list.size());
  }

  @Test
  public void test_type_parsing_complicated()throws Exception{
    //a subroutine which returns a function which returns a function
    final String source = "(PInt,NInt,MyType) ~> ( (PInt)->((PInt)->PInt) )".replaceAll(" ","");

    Lexer lexer = new Lexer();
    TokenList list = lexer.lexCodeTestMode(source);

    TypeNode node = new TypeNode(list);

    assertTrue(node.typenode instanceof SubroutineTypeNode);
    SubroutineTypeNode mytype = (SubroutineTypeNode) node.typenode;

    assertTrue(mytype.has_side_effects);
    assertEquals(3, mytype.argumentTypes.size());

    SubroutineTypeNode myreturnedtype = (SubroutineTypeNode) mytype.returnType;



    assertEquals(source,node.getTypeName());
    assertEquals(0,list.size());
  }

  @Test
  public void test_type_parsing_simple_type()throws Exception{
    final String source = "PInt";

    Lexer lexer = new Lexer();
    TokenList list = lexer.lexCodeTestMode(source);

    TypeNode node = new TypeNode(list);

    assertEquals(source,node.getTypeName());
    assertTrue(node.typenode instanceof SimpleTypeNode);
    assertEquals(0,list.size());
  }
}
