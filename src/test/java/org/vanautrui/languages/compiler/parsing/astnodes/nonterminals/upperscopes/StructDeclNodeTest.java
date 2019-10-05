package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals.upperscopes;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.upperscopes.StructDeclNode;

import static org.junit.Assert.assertEquals;

public class StructDeclNodeTest {

  @Test
  public void test_can_parse_empty_struct_decl() throws Exception {

    TokenList list = (new Lexer()).lexCodeTestMode("struct MyStruct{}");

    StructDeclNode node = new StructDeclNode(list,false);

    assertEquals(0,node.structMembersList.size());
    assertEquals("MyStruct",node.getTypeName());
  }

  @Test
  public void test_will_not_parse_invalid_typename_for_struct() throws Exception {

    TokenList list = (new Lexer()).lexCodeTestMode("struct myStruct{}");

    try {
      StructDeclNode node = new StructDeclNode(list, false);
      Assert.fail();
    }catch (Exception e){
      //
    }
  }

  @Test
  public void test_rejects_struct_with_subroutine_type() throws Exception {

    TokenList list = (new Lexer()).lexCodeTestMode("struct ()~>PInt{PInt a}");

    try {
      StructDeclNode node = new StructDeclNode(list, false);
      Assert.fail();
    }catch (Exception e){
      //
    }
  }

  @Test
  public void test_can_parse_struct_with_1_member() throws Exception {

    TokenList list = (new Lexer()).lexCodeTestMode("struct MyStruct{PInt a}");

    StructDeclNode node = new StructDeclNode(list,false);

    assertEquals(1,node.structMembersList.size());
    assertEquals("a",node.structMembersList.get(0).name);
    assertEquals("PInt",node.structMembersList.get(0).type.getTypeName());
    assertEquals("MyStruct",node.getTypeName());
  }

  @Test
  public void test_can_parse_struct_with_2_members() throws Exception {

    TokenList list = (new Lexer()).lexCodeTestMode("struct MyStruct{PInt a,MyStruct b}");

    StructDeclNode node = new StructDeclNode(list,false);

    assertEquals(2,node.structMembersList.size());

    assertEquals("a",node.structMembersList.get(0).name);
    assertEquals("PInt",node.structMembersList.get(0).type.getTypeName());

    assertEquals("b",node.structMembersList.get(1).name);
    assertEquals("MyStruct",node.structMembersList.get(1).type.getTypeName());

    assertEquals("MyStruct",node.getTypeName());
  }
}
