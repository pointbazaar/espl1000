package org.vanautrui.languages.compiler.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.compiler.lexing.utils.CharacterList;

public class AccessModifierTokenTest {

    @Test
    public void test_constructor() throws Exception {

        CharacterList list = new CharacterList("public lala");

        AccessModifierToken amt = new AccessModifierToken(list);

        Assert.assertTrue(amt.is_public);

        Assert.assertEquals(5, list.size());
    }

    @Test
    public void test_constructor_3() throws Exception {

        CharacterList list = new CharacterList("private lala");

        AccessModifierToken amt = new AccessModifierToken(list);

        Assert.assertTrue(!amt.is_public);

        Assert.assertEquals(5, list.size());
    }

    @Test
    public void test_constructor_2() throws Exception {

        CharacterList list = new CharacterList("pulic");

        try {
            AccessModifierToken amt = new AccessModifierToken(list);
            Assert.fail();
        } catch (Exception e) {
            //pass
        }

    }
}
