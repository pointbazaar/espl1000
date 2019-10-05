package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.StructMemberDeclNode;

import static org.junit.Assert.assertEquals;

public class StructMemberDeclNodeTest {

  @Test
  public void test_can_parse_struct_member()throws Exception{
    final String type="(PInt)~>PInt";
    final String source=type+" subr";
    Lexer lexer = new Lexer();
    TokenList list = lexer.lexCodeTestMode(source);

    StructMemberDeclNode node = new StructMemberDeclNode(list,false);

    assertEquals(node.type.getTypeName(), type);
    assertEquals("subr",node.name);
    assertEquals(0,list.size());
  }
}
