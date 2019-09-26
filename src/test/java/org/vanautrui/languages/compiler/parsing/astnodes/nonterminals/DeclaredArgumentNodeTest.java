package org.vanautrui.languages.compiler.parsing.astnodes.nonterminals;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.Lexer;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;
import org.vanautrui.languages.compiler.parsing.astnodes.nonterminal.DeclaredArgumentNode;

import static org.junit.Assert.assertEquals;

public class DeclaredArgumentNodeTest {

  @Test
  public void test_parse_declared_argument()throws Exception{
    final String type="(PInt)~>PInt";
    final String source=type+" subr";
    Lexer lexer = new Lexer();
    TokenList list = lexer.lexCodeTestMode(source);

    DeclaredArgumentNode node = new DeclaredArgumentNode(list);

    assertEquals(node.type, type);
    assertEquals("subr",node.name);
    assertEquals(0,list.size());
  }
}
