package org.vanautrui.languages.dragoncompiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.dragoncompiler.lexing.utils.CharacterList;

public class BoolConstantTokenTest {

    @Test
    public void test_true() throws Exception {

        CharacterList list = new CharacterList("true");

        BoolConstantToken token = new BoolConstantToken(list);

        Assert.assertEquals("true", token.getContents());
    }

    @Test
    public void test_false() throws Exception {

        CharacterList list = new CharacterList("false");
        BoolConstantToken token = new BoolConstantToken(list);
        Assert.assertEquals("false", token.getContents());
    }

    @Test
    public void test_not_capitalized() throws Exception {

        CharacterList list = new CharacterList("True");
        try {
            IntegerNonNegativeConstantToken token = new IntegerNonNegativeConstantToken(list);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }

}
