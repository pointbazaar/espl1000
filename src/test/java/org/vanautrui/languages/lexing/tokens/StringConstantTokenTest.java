package org.vanautrui.languages.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.utils.CharacterList;

public class StringConstantTokenTest {

    @Test
    public void test_can_retrieve_contents() throws Exception {

        CharacterList list = new CharacterList("\"hi\"");

        StringConstantToken token = new StringConstantToken(list);

        Assert.assertEquals("hi", token.getContents());
    }

    @Test
    public void test_can_represent_multiline_string() throws Exception {

        CharacterList list = new CharacterList("\"hi\n\nhi\"");

        StringConstantToken token = new StringConstantToken(list);

        Assert.assertEquals("hi\n\nhi", token.getContents());
    }

}
