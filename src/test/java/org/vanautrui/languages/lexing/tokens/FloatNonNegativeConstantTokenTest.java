package org.vanautrui.languages.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.utils.CharacterList;

public class FloatNonNegativeConstantTokenTest {

    @Test
    public void test1() throws Exception {

        CharacterList list = new CharacterList("2038.4");

        FloatNonNegativeConstantToken token = new FloatNonNegativeConstantToken(list);

        Assert.assertEquals("2038.4", token.getContents());
    }

    @Test
    public void test_zero() throws Exception {

        CharacterList list = new CharacterList("0.0");
        FloatNonNegativeConstantToken token = new FloatNonNegativeConstantToken(list);
        Assert.assertEquals("0.0", token.getContents());
    }

    @Test
    public void test_negative() throws Exception {

        CharacterList list = new CharacterList("-5.0");
        try {
            FloatNonNegativeConstantToken token = new FloatNonNegativeConstantToken(list);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }

}
