package org.vanautrui.languages.compiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;

public class ArrowTokenTest {

  @Test
  public void test_constructor() throws Exception {

    CharacterList list = new CharacterList("->");

    ArrowToken amt = new ArrowToken(list);

    Assert.assertTrue(amt.is_functional);

    Assert.assertEquals(0, list.size());
  }

  @Test
  public void test_constructor_2() throws Exception {

    CharacterList list = new CharacterList("~>");

    ArrowToken amt = new ArrowToken(list);

    Assert.assertTrue(!amt.is_functional);

    Assert.assertEquals(0, list.size());
  }
}
