package org.vanautrui.languages.compiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;

public class IntegerNonNegativeConstantTokenTest {

    @Test
    public void test1() throws Exception {

        CharacterList list = new CharacterList("2038");

        IntegerNonNegativeConstantToken token = new IntegerNonNegativeConstantToken(list);

        Assert.assertEquals("2038", token.getContents());
    }

    @Test
    public void test_zero() throws Exception {

        CharacterList list = new CharacterList("0");
        IntegerNonNegativeConstantToken token = new IntegerNonNegativeConstantToken(list);
        Assert.assertEquals("0", token.getContents());
    }

    @Test
    public void test_negative() throws Exception {

        CharacterList list = new CharacterList("-5");
        try {
            IntegerNonNegativeConstantToken token = new IntegerNonNegativeConstantToken(list);
            Assert.fail();
        }catch (Exception e){
            //pass
        }
    }

}
