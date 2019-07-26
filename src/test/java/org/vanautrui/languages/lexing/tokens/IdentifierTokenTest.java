package org.vanautrui.languages.lexing.tokens;

import org.junit.Assert;
import org.junit.Test;
import org.vanautrui.languages.lexing.CharacterList;

public class IdentifierTokenTest {

    @Test
    public void test1() throws Exception {

        CharacterList list = new CharacterList("main { pub");

        IdentifierToken identifierToken = new IdentifierToken(list);

        Assert.assertEquals("main", identifierToken.getContents());
    }
}
