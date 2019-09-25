package org.vanautrui.languages.compiler.parsing.astnodes.typenodes;

import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.tokens.TypeIdentifierToken;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;
import org.vanautrui.languages.compiler.lexing.utils.TokenList;

import static org.junit.Assert.assertEquals;

public class SimpleTypeNodeTest {

  @Test
  public void test_typenode_parsing()throws Exception{
    TokenList list = new TokenList();
    list.add(new TypeIdentifierToken(new CharacterList("MyType")));

    TypeNode node = new TypeNode(list);
    assertEquals("MyType",node.getTypeName());
    assertEquals(0,list.size());
  }
}
