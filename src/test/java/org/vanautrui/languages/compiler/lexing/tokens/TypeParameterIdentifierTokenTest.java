package org.vanautrui.languages.compiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;

import static org.junit.Assert.assertEquals;

public class TypeParameterIdentifierTokenTest {

  @Test
  public void test_simple_type_param() throws Exception {
    CharacterList list = new CharacterList("?T0");

    TypeParameterIdentifierToken tk = new TypeParameterIdentifierToken(list);

    assertEquals(0,tk.type_parameter_number);
  }

  @Test
  public void test_invalid() throws Exception {
    CharacterList list = new CharacterList("??T0");
    try {
      TypeParameterIdentifierToken tk = new TypeParameterIdentifierToken(list);
      Assert.fail();
    }catch (Exception e){
      //
    }
  }
}
