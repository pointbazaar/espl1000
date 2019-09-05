package org.vanautrui.languages.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.utils.CharacterList;

public class CharConstantTokenTest {

    @Test
    public void test_can_lex_char() throws Exception {

        CharacterList list = new CharacterList("'x'");

        CharConstantToken token = new CharConstantToken(list);

        Assert.assertEquals("x", token.getContents());
    }

    @Test
    public void test_can_reject_char() throws Exception {

        CharacterList list = new CharacterList("'xy'");

        try {
            CharConstantToken token = new CharConstantToken(list);
            Assert.fail();
        }catch (Exception e){

        }
    }

}
